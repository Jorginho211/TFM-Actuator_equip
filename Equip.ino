#include "sys/time.h"
#include "IBeaconTransmitter_ESP32.h"
#include "IBeaconProvider_ESP32.h"

#define PLACE_UUID "fffffffe-ffff-fffe-ffff-fffefffffffd"
#define MAJOR_ID 1
#define MINOR_ID 1
#define EQUIPMENT_UUID "fffffffe-ffff-fffe-ffff-fffefffffffe"

uint8_t episNeeded = 3;

IBeaconTransmitter* iBeaconTransmitter;
IBeaconProvider *iBeaconProvider;

unsigned long lastTimeActivate;

void setup() {
  Serial.begin(115200);
  //connect();

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
