#include "MqttManager.h"

PubSubClient MqttManager::client;
WiFiClientSecure MqttManager::wifiClient;

char *MqttManager::clientName;
char *MqttManager::username;
char *MqttManager::password;
char *MqttManager::equipmentTopic;
char *MqttManager::workersTopic;

char *MqttManager::wifiSsid;
char *MqttManager::wifiPassword;

IPAddress *MqttManager::ip;
IPAddress *MqttManager::dns;
IPAddress *MqttManager::gateway;

void (*MqttManager::equipmentCallback)(uint8_t equipmentNeeded) = NULL;
void (*MqttManager::workersCallback)(WorkersList *list) = NULL;

void MqttManager::init(char *ssid, char *wifiPassword, char *server, int port, char *clientName, char *username, char *mqttPassword) {
    MqttManager::client.setClient(MqttManager::wifiClient);
    MqttManager::client.setServer(server, port);
    MqttManager::client.setCallback(MqttManager::mqttCallback);

    MqttManager::clientName = clientName;
    MqttManager::username = username;
    MqttManager::password = mqttPassword;

    MqttManager::wifiSsid = ssid;
    MqttManager::wifiPassword = wifiPassword;
}


void MqttManager::init(char *ssid, char *wifiPassword, IPAddress ip, IPAddress dns, IPAddress gateway, char *server, int port, char *clientName, char *username, char *mqttPassword) {
    MqttManager::init(ssid, wifiPassword, server, port, clientName, username, mqttPassword);

    MqttManager::ip = &ip;
    MqttManager::dns = &dns;
    MqttManager::gateway = &gateway;
}

void MqttManager::loadCACert(char *ca){
    MqttManager:wifiClient.setCACert(ca);
}

bool MqttManager::connect(){
    if(!MqttManager::client.connected() && client.connect(MqttManager::clientName, MqttManager::username, MqttManager::password)){
        if(MqttManager::equipmentTopic != NULL){
            MqttManager::client.subscribe(MqttManager::equipmentTopic);
        }

        if(MqttManager::workersTopic != NULL){
            MqttManager::client.subscribe(MqttManager::workersTopic);
        }
    }
    else if (!MqttManager::client.connected()){
        printf("Non se puido conectar ao servidor MQTT\n");
        return false;
    }

    return true;    
}

bool MqttManager::disconnect() {
    MqttManager::client.disconnect();
    return true;
}

void MqttManager::setEquipmentTopic(char *topic, void (*callback)(uint8_t)){
    MqttManager::equipmentTopic = topic;
    MqttManager::equipmentCallback = callback;

    if(MqttManager::equipmentTopic != NULL){        
        MqttManager::client.unsubscribe(MqttManager::equipmentTopic);
    }

    MqttManager::client.subscribe(MqttManager::equipmentTopic);
}

void MqttManager::setWorkersTopic(char *topic, void (*callback)(WorkersList *list)){
    MqttManager::workersTopic = topic;
    MqttManager::workersCallback = callback;

    if(MqttManager::workersTopic != NULL){
        MqttManager::client.unsubscribe(MqttManager::workersTopic);
    }
    
    MqttManager::client.subscribe(MqttManager::workersTopic);
}

bool MqttManager::connectWiFi(){
    if(WiFi.status() == WL_CONNECTED){
        return true;
    }

    if(ip != NULL && dns != NULL && gateway != NULL){
        WiFi.config(*MqttManager::ip, *MqttManager::dns, *MqttManager::gateway);
    }
    
    WiFi.begin(MqttManager::wifiSsid, MqttManager::wifiPassword);

    printf("Conectando a rede.");
    uint8_t retry = 0;
    while (WiFi.status() != WL_CONNECTED && retry < 5) {
        printf(".");
        delay(50);
        retry++;
    }
    printf("\n");
    
    if(WiFi.status() != WL_CONNECTED){
        printf("Non se puido conectar a WiFi\n");
        return false;
    }

    return true;
}

bool MqttManager::loop() {
    if(!MqttManager::connectWiFi() || !MqttManager::connect()){
        return false;
    }

    printf("AQUI\n");
    MqttManager::client.loop();

    return true;
}

void MqttManager::mqttCallback(char *topic, byte *payload, unsigned int length){
    printf("%s\n", topic);
    if(strcmp(topic, MqttManager::equipmentTopic) == 0){
        uint8_t equipmentNeeded = (uint8_t) payload[0] - 48;
        equipmentCallback(equipmentNeeded);
    }
    else if(strcmp(topic, MqttManager::workersTopic) == 0){
        if(length == 0){
            workersCallback(new WorkersList());
            return;
        }

        unsigned int uuidSize = 36;
        unsigned int sizeArray = (length + 1)/(uuidSize + 1); //Se suma uno por el separador ";"

        WorkersList *workerList = new WorkersList(sizeArray, uuidSize + 1); //Para el caracter '\0' de termino de cadena'
        unsigned int uuidPos = 0;
        unsigned int charPos = 0;

        for(unsigned int i = 0; i < length; i++){
            if(payload[i] == ';') {
                workerList->workers[uuidPos][charPos] = '\0';
                uuidPos++;
                charPos = 0;       
                continue;
            }

            workerList->workers[uuidPos][charPos++] = (char) payload[i];
        }
        workerList->workers[uuidPos][charPos] = '\0';

        workersCallback(workerList);
    }
}