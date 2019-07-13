#include "sys/time.h"
#include "IBeaconTransmitter_ESP32.h"
#include "IBeaconProvider_ESP32.h"
#include "Settings.h"
#include "MqttManager.h"
#include "WorkersList.h"
#include "FileSystemManager.h"

uint8_t episNeeded = 3;
WorkersList *allowedWorkers;

IBeaconTransmitter* iBeaconTransmitter;
IBeaconProvider *iBeaconProvider;

unsigned long lastTimeActivate;

void equipmentMqttCallback(uint8_t eNeeded){
  episNeeded = eNeeded;
  FileSystemManager::writeEquipmentNeeded(episNeeded);
  printf("Equipment: %d\n", episNeeded);
}

void workerMqttCallback(WorkersList *list){
  if(allowedWorkers != NULL){
    allowedWorkers->dispose();
    delete allowedWorkers;
  }

  allowedWorkers = list;
  FileSystemManager::writeWorkersAllowed(allowedWorkers);

  allowedWorkers->toString();
}

void setup() {
  Serial.begin(115200);
  FileSystemManager::init();
  allowedWorkers = FileSystemManager::readAllowedWorkers();
  uint8_t* epis = FileSystemManager::readEquipmentNeeded();
  
  //Inicializando MQTT
  MqttManager::init(WIFI_SSID, WIFI_PASS, IPAddress(WIFI_IP), IPAddress(WIFI_DNS), IPAddress(WIFI_GATEWAY), MQTT_SERVER, MQTT_PORT, MQTT_CLIENT_NAME, MQTT_USER, MQTT_PASS);
  MqttManager::loadCACert(CA);
  MqttManager::setEquipmentTopic(MQTT_PLACE_EQUIPMENT, equipmentMqttCallback);
  MqttManager::setWorkersTopic(MQTT_PLACE_WORKERS, workerMqttCallback);

  if(allowedWorkers == NULL){
    allowedWorkers = new WorkersList();
  }

  if(epis == NULL){
    episNeeded = -1; //Al ser unsigned se pone en el ultimo valor positivo
  }
  else {
    episNeeded = *epis;
  }

  //Se piden por MQTT al iniciar los parametros iniciales
  MqttManager::requestEquipment(MAJOR_ID, MINOR_ID);
  MqttManager::requestWorkers(MAJOR_ID, MINOR_ID);

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

void loop() {
  MqttManager::loop();
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
  if(iBeacon->uuid == EQUIPMENT_UUID || iBeacon->uuid == PLACE_UUID || iBeacon->getDistance() > 5 || (episNeeded & iBeacon->minor) != episNeeded || !allowedWorkers->contains(iBeacon->uuid)) {
    return false;
  }

  digitalWrite(2, HIGH);

  return true;
}
