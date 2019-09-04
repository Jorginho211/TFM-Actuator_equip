#include "IBeacon.h"
#include <math.h>

IBeacon::IBeacon() {
    IBeacon("", 0, 0, 0, 0);
}

IBeacon::IBeacon(std::string uuid, uint8_t major, uint8_t minor, int8_t txPower, int8_t rssi){
    this->uuid = uuid;
    this->major = major;
    this->minor = minor;
    this->txPower = txPower;
    this->rssi = rssi;
}

double IBeacon::getDistance() {
    double distance = 0;
    if(this->rssi == 0){
        return -1.0;
    }

    double ratio = this->rssi * 1.0 / this->txPower;
    if(ratio < 1.0){
        distance = pow(ratio, 10);
    }
    else {
        distance = 0.89976 * pow(ratio, 7.7095) + 0.11;
    }

    return distance;
}