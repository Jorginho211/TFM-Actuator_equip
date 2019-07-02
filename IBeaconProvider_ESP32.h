#ifndef IBeaconProvider_ESP32_h_
#define IBeaconProvider_ESP32_h_

#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEScan.h>
#include <BLEAdvertisedDevice.h>

#include "IBeaconProvider.h"

class IBeaconProvider_ESP32: public IBeaconProvider  {
    private:
        BLEScan* pBLEScan;
        IBeacon* parseBeacon(uint8_t* data, uint8_t length);
    public:
        IBeaconProvider_ESP32();
        void start(uint32_t time_s);
        IBeaconList* getResults();
        void stop();
};

#endif