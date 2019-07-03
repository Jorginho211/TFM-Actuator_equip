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
    uint8_t n = 3; //N puede ser un valor entre 2 y 4 dependiendo de las condiciones

    double distance = pow(10, ((this->txPower - this->rssi)/(10.0 * n)));
    return distance;
}