#ifndef IBeaconTransmitter_h_
#define IBeaconTransmitter_h_

#include "IBeacon.h"

class IBeaconTransmitter {
    protected:
        IBeacon *iBeacon;
    public:
        IBeaconTransmitter(IBeacon *iBeacon);
        virtual void start(){};
        virtual void stop(){};
};
#endif