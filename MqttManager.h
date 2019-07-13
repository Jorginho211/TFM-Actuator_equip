#ifndef MqttManager_h_
#define MqttManager_h_

    #include <WiFi.h>
    #include <WiFiClientSecure.h>
    #include <PubSubClient.h>
    #include "WorkersList.h"

    class MqttManager {
        public:
            static void init(char *ssid, char *wifiPassword, char *server, int port, char *clientName, char *username, char *mqttPassword);
            static void init(char *ssid, char *wifiPassword, IPAddress ip, IPAddress dns, IPAddress gateway, char *server, int port, char *clientName, char *username, char *mqttPassword);
            static void loadCACert(char *ca);
            static bool connect();
            static bool connectWiFi();
            static bool disconnect();
            static void setEquipmentTopic(char *topic, void (*callback)(uint8_t));
            static void setWorkersTopic(char *topic, void (*callback)(WorkersList*));
            static void requestEquipment(uint8_t major, uint8_t minor);
            static void requestWorkers(uint8_t major, uint8_t minor);
            static bool loop();
        private:
            static WiFiClientSecure wifiClient;
            static PubSubClient client;

            static char *equipmentTopic;
            static char *workersTopic;

            static char *clientName;
            static char *username;
            static char *password;

            static char *wifiSsid;
            static char *wifiPassword;

            static IPAddress *ip;
            static IPAddress *dns;
            static IPAddress *gateway;

            static void (*equipmentCallback)(uint8_t equipmentNeeded);
            static void (*workersCallback)(WorkersList *list);

            static void mqttCallback(char *topic, byte *payload, unsigned int length);
    };
#endif