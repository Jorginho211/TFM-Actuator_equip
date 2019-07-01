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

struct timeval now;
unsigned long lastTime;
unsigned long lastTimeActivate;
bool iBeaconMode = false; 

void setup() {
  Serial.begin(115200);
  //connect();

  //Transmite un iBeacon
  IBeacon* iBeacon = new IBeacon(PLACE_UUID, MAJOR_ID, MINOR_ID, -80, 0);
  iBeaconTransmitter = new IBeaconTransmitter_ESP32(iBeacon);

  //Obtiene los iBeacons del entorno
  iBeaconProvider = new IBeaconProvider_ESP32();

  lastTime = 0;

  pinMode(2,OUTPUT);
}


void loop() {
  manageBluetooth();
}


void manageBluetooth() {
  gettimeofday(&now, NULL);

  if(now.tv_sec - lastTime <= 2){
    return;
  }

  if(lastTime > 0){
    iBeaconMode = !iBeaconMode;
  }

  if(iBeaconMode){
    if(lastTime > 0)
      iBeaconProvider->stop();

    iBeaconTransmitter->start();

    if(lastTime > 0){
      IBeaconList *iBeacons = iBeaconProvider->getResults();
      struct timeval now2;
      gettimeofday(&now2, NULL);

      printf("--------------------------------------------------------------------------------\n");
      for(int i = 0; i < iBeacons->size; i++){
        IBeacon *iBeacon = iBeacons->list[i];
        printf("UUID: %s\tMAJOR: %d\tMINOR: %d\tTX_POWER: %d\tRSSI: %d\tDISTANCE: %lf\n", iBeacon->uuid.c_str(), iBeacon->major, iBeacon->minor, iBeacon->txPower, iBeacon->rssi, iBeacon->getDistance());
        if(checkIBeacon(iBeacon)) {
          lastTimeActivate = now2.tv_sec;
        }
      }
      printf("--------------------------------------------------------------------------------\n\n");

      if(now2.tv_sec - lastTimeActivate > 5){
        digitalWrite(2, LOW);
      }
    }
  }
  else {
    if(lastTime > 0)
      iBeaconTransmitter->stop();
    iBeaconProvider->start();
  }

  lastTime = now.tv_sec;
}

bool checkIBeacon(IBeacon *iBeacon){
  if(iBeacon->uuid == EQUIPMENT_UUID || iBeacon->uuid == PLACE_UUID || iBeacon->getDistance() > 5 || (episNeeded & iBeacon->minor) != episNeeded) {
    return false;
  }

  
  digitalWrite(2, HIGH);

  return true;
}
