#include "sys/time.h"
#include "IBeaconTransmitter_ESP32.h"
#include "IBeaconProvider_ESP32.h"
#include <WiFiClientSecure.h>
#include <PubSubClient.h>

#define MQTT_SERVER "10.42.0.1"
#define MQTT_PORT 8883

#define PLACE_UUID "fffffffe-ffff-fffe-ffff-fffefffffffd"
#define MAJOR_ID 1
#define MINOR_ID 1
#define EQUIPMENT_UUID "fffffffe-ffff-fffe-ffff-fffefffffffe"

#define STR_HELPER(x) #x
#define STR(x) STR_HELPER(x)
#define PLACE_EQUIPMENT_MQTT "place/" STR(MAJOR_ID) "/" STR(MINOR_ID) "/equipment"

uint8_t episNeeded = 3;

IBeaconTransmitter* iBeaconTransmitter;
IBeaconProvider *iBeaconProvider;

unsigned long lastTimeActivate;

WiFiClientSecure espClient;
PubSubClient client(espClient);

const char* caCert =
"-----BEGIN CERTIFICATE-----\n" \
"MIIDcjCCAlqgAwIBAgIJANKAZDMg41XXMA0GCSqGSIb3DQEBCwUAME4xCzAJBgNV\n" \
"BAYTAkVTMQswCQYDVQQIDAJFUzELMAkGA1UEBwwCRVMxCzAJBgNVBAoMAkVTMQsw\n" \
"CQYDVQQLDAJjYTELMAkGA1UEAwwCY2EwHhcNMTkwNzA3MTQ1MTAwWhcNMTkwODA2\n" \
"MTQ1MTAwWjBOMQswCQYDVQQGEwJFUzELMAkGA1UECAwCRVMxCzAJBgNVBAcMAkVT\n" \
"MQswCQYDVQQKDAJFUzELMAkGA1UECwwCY2ExCzAJBgNVBAMMAmNhMIIBIjANBgkq\n" \
"hkiG9w0BAQEFAAOCAQ8AMIIBCgKCAQEA0S/uzVO2O4hB6/Dp6sF+qXa8F4T7nHE1\n" \
"T2ojeLRpipLvu8psbLVdDFHDgTImefPPpPmd0gUh0kPxBFbgAdiG7LvLiRWmVhaT\n" \
"Igoms6XAAnEBaW3XwF92IsO8M9vq6IAQgsx2zb23uVyFX1CY1bRXNY+OGvE7CtgH\n" \
"ksJXgmm0p+4JPBWholMupBdbS81Sy5bShjsoD/KEz0eKgl0XQH2nxQYlSYzjno6b\n" \
"zfTl+qvP1ntIFdeCTxW/+3c0AQDL6wj3Pi/z2Pt+sK+rsR5t+2T8SgrQIZHJ2tO4\n" \
"bQdIAaHjqxonDBQCA7B/6GytyWmZ2GJ4WjVh+cksLsVkjtW7bfwuEQIDAQABo1Mw\n" \
"UTAdBgNVHQ4EFgQUAJYd84p/PH8bIZpbku12NyRxcRIwHwYDVR0jBBgwFoAUAJYd\n" \
"84p/PH8bIZpbku12NyRxcRIwDwYDVR0TAQH/BAUwAwEB/zANBgkqhkiG9w0BAQsF\n" \
"AAOCAQEALvgNYcpEH5WmeYGYiyXqqmtW9ss+YVu/bIB8tJZnw2yh7ORKev1857bQ\n" \
"rCC53TkmNPLthAEoWeAfHd971SzP+wPGThbzZfUGY+DfvHVqq5ZGvOG1ZSVUQTqh\n" \
"PAZJ9V1CZgJTkcJvb+tICT41K82MiaYfcI9qUwgJ6ewLEQqYx0nonhnDxOOp698H\n" \
"DYW5r8SPTHJ3sCrNsWKQnBua768F73qq29UhWEggt5dw07q/A2suCxAuahqvQDr1\n" \
"UVFQf4vi0wWx7odtl7SRbk5bQD42LZHFjRlWcF7Xnk09fSrIe9VRf/dSEEUdesse\n" \
"gKzpCYwLc8J5yo9efKVX6byXX+s/7w==\n" \
"-----END CERTIFICATE-----";

void setup() {
  Serial.begin(115200);
  setupWifi();
  setupMQTT();

  //Transmite un iBeacon
  IBeacon* iBeacon = new IBeacon(PLACE_UUID, MAJOR_ID, MINOR_ID, -80, 0);
  iBeaconTransmitter = new IBeaconTransmitter_ESP32(iBeacon);

  //Obtiene los iBeacons del entorno
  iBeaconProvider = new IBeaconProvider_ESP32();

  //Se inicia el envio del beacon del area
  iBeaconTransmitter->start();

  lastTimeActivate = 0;

  pinMode(2,OUTPUT);
}

void setupWifi(){
  IPAddress ip(10, 42, 0, 101);
  IPAddress dns(8, 8, 8, 8);
  IPAddress gateway(10, 42, 0, 1);

  WiFi.config(ip, dns, gateway);
  WiFi.begin("EQUIP", "");
 
  printf("Conectando a rede.");
  while (WiFi.status() != WL_CONNECTED) {
      printf(".");
      delay(500);
  }
  printf("\n");

  espClient.setCACert(caCert);
}

void callback(char* topic, byte* payload, unsigned int length) {
  printf("topic: %s\n", topic);
  printf("payload: %s\n", payload);
}

void setupMQTT(){
  client.setServer(MQTT_SERVER, MQTT_PORT);
  client.setCallback(callback);
}

void reconnectMQTT() {
  while(!client.connected()){
    if(!client.connect("PLACE", "", "")) continue;

    client.subscribe(PLACE_EQUIPMENT_MQTT);
  }

  client.loop();
}

void loop() {
  reconnectMQTT();
  manageBluetooth();
}


void manageBluetooth() {
  //Se inicia un escaneo con la duracion del mismo
  iBeaconProvider->start(1);

  //Se recogen los resultados
  IBeaconList *iBeacons = iBeaconProvider->getResults();

  struct timeval now;
  gettimeofday(&now, NULL);

  //Se imprimen los resultados y se checkean para ver si hai un operario valido
  printf("--------------------------------------------------------------------------------\n");
  for(int i = 0; i < iBeacons->size; i++){
    IBeacon *iBeacon = iBeacons->list[i];
    printf("UUID: %s\tMAJOR: %d\tMINOR: %d\tTX_POWER: %d\tRSSI: %d\tDISTANCE: %lf\n", iBeacon->uuid.c_str(), iBeacon->major, iBeacon->minor, iBeacon->txPower, iBeacon->rssi, iBeacon->getDistance());
    if(checkIBeacon(iBeacon)) {
      lastTimeActivate = now.tv_sec;
    }
  }
  printf("--------------------------------------------------------------------------------\n\n");
  
  //Si pasaron 5 segundos sin volverse a detectar un operario valido se desconecta el actuador
  if(now.tv_sec - lastTimeActivate > 5){
    digitalWrite(2, LOW);
  }
}

bool checkIBeacon(IBeacon *iBeacon){
  if(iBeacon->uuid == EQUIPMENT_UUID || iBeacon->uuid == PLACE_UUID || iBeacon->getDistance() > 5 || (episNeeded & iBeacon->minor) != episNeeded) {
    return false;
  }

  digitalWrite(2, HIGH);

  return true;
}
