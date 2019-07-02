#include "IBeaconProvider_ESP32.h"
#include <string.h>
IBeaconProvider_ESP32::IBeaconProvider_ESP32() {
    BLEDevice::init("");
    this->pBLEScan = BLEDevice::getScan();
    this->pBLEScan->setActiveScan(true); //active scan uses more power, but get results faster
    this->pBLEScan->setInterval(100);
    this->pBLEScan->setWindow(99);  // less or equal setInterval value

    this->iBeacons = new IBeaconList();
}

IBeacon* IBeaconProvider_ESP32::parseBeacon(uint8_t *data, uint8_t length){
    if(length != 25){ //Se comprueba si el tamaño es 25 bytes ya que un iBeacon tiene un cuerpo de 25
        return NULL;
    }

    char hexmap[] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'a', 'b', 'c', 'd', 'e', 'f'};

    IBeacon *iBeacon = new IBeacon();
    for(int i = 0; i < length; i++){
        if(i < 4){
            //Se comprueba la cabecera se corresponde con la del iBeacon
            if(i == 0 && data[i] != 0x4c || 
                i == 1 && data[i] != 0x00 ||
                i == 2 && data[i] != 0x02 ||
                i == 3 && data[i] != 0x15){
                    return NULL;
            }
        }
        else if(i < 20){
            if(i == 8 || i == 10 || i == 12 || i == 14){
                 iBeacon->uuid += "-";
            }

            //Se convierte cada byte en una cadena hexadecimal
            iBeacon->uuid += hexmap[(data[i]) >> 4];
            iBeacon->uuid += hexmap[(data[i] & 0x0F)];
        }
        else if(i < 22) {
            if(i == 20){
                iBeacon->major = data[i];
            }
            else {
                iBeacon->major = (iBeacon->major << 8) + data[i];
            }
        }
        else if(i < 24) {
            if(i == 20){
                iBeacon->minor = data[i];
            }
            else {
                iBeacon->minor = (iBeacon->major << 8) + data[i];
            }
        }
        else if( i == 24) {
            iBeacon->txPower = data[i];
        }
    }

    return iBeacon;
}

void IBeaconProvider_ESP32::start(uint32_t time_s) {
    for(uint8_t i = 0; i < this->iBeacons->size; i++){
        delete this->iBeacons->list[i];
    }

    BLEScanResults foundDevices = pBLEScan->start(time_s, true);
    this->iBeacons->size = 0;

    for(uint16_t i = 0; i < foundDevices.getCount(); i++){
        IBeacon *iBeacon = this->parseBeacon((uint8_t*)foundDevices.getDevice(i).getManufacturerData().data(), foundDevices.getDevice(i).getManufacturerData().length());
        if(iBeacon == NULL){
            continue;
        }

        iBeacon->rssi = foundDevices.getDevice(i).getRSSI();
        this->iBeacons->list[this->iBeacons->size++] = iBeacon;
    }

    pBLEScan->clearResults();
}

IBeaconList* IBeaconProvider_ESP32::getResults() {
    return this->iBeacons;
}

void IBeaconProvider_ESP32::stop() {};