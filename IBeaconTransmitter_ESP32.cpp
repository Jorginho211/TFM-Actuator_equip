#include "IBeaconTransmitter_ESP32.h"

IBeaconTransmitter_ESP32::IBeaconTransmitter_ESP32(IBeacon *iBeacon):IBeaconTransmitter(iBeacon) {}

void IBeaconTransmitter_ESP32::start() {
    BLEDevice::init("");
    
    this->pAdvertising = BLEDevice::getAdvertising();

    BLEBeacon oBeacon = BLEBeacon();
    oBeacon.setManufacturerId(0x4C00); // fake Apple 0x004C LSB (ENDIAN_CHANGE_U16!)
    oBeacon.setProximityUUID(BLEUUID::fromString(this->iBeacon->uuid));
    oBeacon.setMajor(this->iBeacon->major);
    oBeacon.setMinor(this->iBeacon->minor);
    oBeacon.setSignalPower(this->iBeacon->txPower);
    BLEAdvertisementData oAdvertisementData = BLEAdvertisementData();
    BLEAdvertisementData oScanResponseData = BLEAdvertisementData();

    oAdvertisementData.setFlags(0x04); // BR_EDR_NOT_SUPPORTED 0x04
    
    std::string strServiceData = "";
    
    strServiceData += (char)26;     // Len
    strServiceData += (char)0xFF;   // Type
    strServiceData += oBeacon.getData(); 
    oAdvertisementData.addData(strServiceData);
  
    this->pAdvertising->setAdvertisementData(oAdvertisementData);
    this->pAdvertising->setScanResponseData(oScanResponseData);

    this->pAdvertising->start();
}

void IBeaconTransmitter_ESP32::stop() {
    this->pAdvertising->stop();
}