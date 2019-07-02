#ifndef IBeaconProvider_h_
#define IBeaconProvider_h_

#include "IBeacon.h"

class IBeaconList {
    public:
        IBeacon *list[256];
        uint8_t size = 0;
};

class IBeaconProvider {
    protected:
        IBeaconList *iBeacons;
    public:
        virtual void start(uint32_t time_s) {};
        virtual IBeaconList* getResults() {};
        virtual void stop() {}
};

#endif