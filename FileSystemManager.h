#ifndef FileSystemManager_h_
#define FileSystemManager_h_

#include <SPIFFS.h>
#include "WorkersList.h"

class FileSystemManager {
    private:
        static char *equipmentFile;
        static char *workersFile;
    public:
        static void init();
        static uint8_t* readEquipmentNeeded();
        static WorkersList* readAllowedWorkers();
        static bool writeEquipmentNeeded(uint8_t equipmentNeeded);
        static bool writeWorkersAllowed(WorkersList *workers);
};
#endif