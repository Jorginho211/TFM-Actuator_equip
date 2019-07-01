#ifndef IBeaconTransmitter_ESP32_h_
#define IBeaconTransmitter_ESP32_h_

#include "sdkconfig.h"
#include <esp_gatt_defs.h>
#include <string>

#include "BLEDevice.h"
#include "BLEUtils.h"
#include "BLEBeacon.h"

#include "IBeaconTransmitter.h"

class IBeaconTransmitter_ESP32: public IBeaconTransmitter {
    private:
        BLEAdvertising *pAdvertising;
    public:
        IBeaconTransmitter_ESP32(IBeacon *iBeacon);
        void start();
        void stop();
};

#endif
