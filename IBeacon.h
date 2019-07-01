
#ifndef IBeacon_h_
#define IBeacon_h_

#include "sdkconfig.h"
#include <esp_gatt_defs.h>
#include <string>

class IBeacon {
       
    public:
        IBeacon();
        IBeacon(std::string uuid, uint8_t major, uint8_t minor, int8_t txPower, int8_t rssi);
        std::string uuid;
        uint8_t major;
        uint8_t minor;
        int8_t txPower;
        int8_t rssi;

        double getDistance();
        std::string toString();
};
#endif