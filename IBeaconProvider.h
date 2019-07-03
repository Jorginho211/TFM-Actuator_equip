#ifndef IBeaconProvider_h_
#define IBeaconProvider_h_

#include "IBeacon.h"

class IBeaconList {
    public:
        IBeacon *list[256];
        uint8_t size = 0;
        
        bool contains(IBeacon *iBeacon) {
            IBeacon *iBeacon2;
            bool exists = false;
            for(uint8_t i = 0; i < this->size; i++){
                iBeacon2 = this->list[i];

                if(iBeacon2->uuid == iBeacon->uuid && iBeacon2->major == iBeacon->major && iBeacon2->minor == iBeacon->minor){
                    if(iBeacon2->rssi > iBeacon->rssi){
                        iBeacon->rssi = iBeacon2->rssi;
                    }
                    
                    exists = true;
                }
            }

            return exists;
        }
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