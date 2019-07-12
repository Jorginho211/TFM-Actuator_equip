#include "FileSystemManager.h"

char *FileSystemManager::equipmentFile = "/equipment.txt";
char *FileSystemManager::workersFile = "/workers.txt";

void FileSystemManager::init(){
    SPIFFS.begin(true);
}

uint8_t* FileSystemManager::readEquipmentNeeded(){
    if(!SPIFFS.exists(FileSystemManager::equipmentFile)){
        return NULL;
    }

    File f = SPIFFS.open(FileSystemManager::equipmentFile, FILE_READ);

    uint8_t value = (uint8_t) f.read();
    uint8_t *equipmentNeeded = new uint8_t;
    *equipmentNeeded = value;

    f.close(); 

    return equipmentNeeded;
}

bool FileSystemManager::writeEquipmentNeeded(uint8_t equipmentNeeded){
    if(SPIFFS.exists(FileSystemManager::equipmentFile)){
        SPIFFS.remove(FileSystemManager::equipmentFile);
    }

    File f = SPIFFS.open(FileSystemManager::equipmentFile, FILE_WRITE);
    f.write(equipmentNeeded);
    f.close();

    return true;
}

WorkersList* FileSystemManager::readAllowedWorkers(){
    if(!SPIFFS.exists(FileSystemManager::workersFile)){
        return NULL;
    }

    File f = SPIFFS.open(FileSystemManager::workersFile, FILE_READ);

    unsigned int uuidSize = 36;
    unsigned int sizeArray = (f.size() + 1)/(uuidSize + 1); //Se suma uno por el separador ";"

    WorkersList *workerList = new WorkersList(sizeArray, uuidSize + 1); //Para el caracter '\0' de termino de cadena'
    unsigned int uuidPos = 0;
    unsigned int charPos = 0;
    
    for(int i = 0; i < f.size(); i++){
        char c = (char) f.read();

        if(c == ';'){
            workerList->workers[uuidPos][charPos] = '\0';
            printf(workerList->workers[uuidPos]);
            uuidPos++;
            charPos = 0;       
            continue;
        }

        workerList->workers[uuidPos][charPos] = c;
        charPos++;
    }
    workerList->workers[uuidPos][charPos] = '\0';
    f.close();

    return workerList;
}

bool FileSystemManager::writeWorkersAllowed(WorkersList *workers){
    if(SPIFFS.exists(FileSystemManager::workersFile)){
        SPIFFS.remove(FileSystemManager::workersFile);
    }

    if(workers->size == 0){
        return false;
    }

    File f = SPIFFS.open(FileSystemManager::workersFile, FILE_WRITE);

    for(int i = 0; i < workers->size; i++){
        for(int j = 0; workers->workers[i][j] != '\0'; j++){
            f.write(workers->workers[i][j]);
        }

        if(i < workers->size - 1){
            f.write(';');
        }
    }

    return true;
}