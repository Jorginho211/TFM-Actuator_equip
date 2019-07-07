#include "sys/time.h"
#include "IBeaconTransmitter_ESP32.h"
#include "IBeaconProvider_ESP32.h"
#include "Settings.h"
#include <WiFiClientSecure.h>
#include <PubSubClient.h>
#include <SPIFFS.h>

uint8_t episNeeded = 3;

IBeaconTransmitter* iBeaconTransmitter;
IBeaconProvider *iBeaconProvider;

unsigned long lastTimeActivate;

WiFiClientSecure espClient;
PubSubClient client(espClient);

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
  WiFi.config(IPAddress(WIFI_IP), IPAddress(WIFI_DNS), IPAddress(WIFI_GATEWAY));
  WiFi.begin(WIFI_SSID, WIFI_PASS);
 
  printf("Conectando a rede.");
  while (WiFi.status() != WL_CONNECTED) {
      printf(".");
      delay(500);
  }
  printf("\n");

  SPIFFS.begin(true);
  File ca = SPIFFS.open("/ca.crt", "r");
  if(!ca) {
    printf("Fallo ao abrir o certificado!\n");
    return;
  }

  espClient.loadCACert(ca, ca.size());
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
    if(!client.connect(MQTT_CLIENT_NAME, MQTT_USER, MQTT_USER)) continue;

    client.subscribe(MQTT_PLACE_EQUIPMENT);
    client.subscribe(MQTT_PLACE_WORKERS);
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
