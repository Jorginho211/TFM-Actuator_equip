#ifndef Settings_h_
#define Settings_h_

    #define DEBUG 1

    //----------------------------------------------------------------------------------------
    // HELPERS
    //----------------------------------------------------------------------------------------
    #define STR_HELPER(x) #x
    #define STR(x) STR_HELPER(x)
    //----------------------------------------------------------------------------------------


    //----------------------------------------------------------------------------------------
    // MQTT
    //----------------------------------------------------------------------------------------
    #define MQTT_SERVER "10.42.0.1"
    #define MQTT_PORT 8883
    #define MQTT_CLIENT_NAME "PLACE_" STR(MAJOR_ID) "_" STR(MINOR_ID)
    #define MQTT_USER ""
    #define MQTT_PASS ""
    #define MQTT_PLACE_EQUIPMENT "place/" STR(MAJOR_ID) "/" STR(MINOR_ID) "/equipment"
    #define MQTT_PLACE_WORKERS "place/" STR(MAJOR_ID) "/" STR(MINOR_ID) "/workers"
    //----------------------------------------------------------------------------------------


    //----------------------------------------------------------------------------------------
    // WIFI
    //----------------------------------------------------------------------------------------
    #define WIFI_SSID "EQUIP"
    #define WIFI_PASS ""
    #define WIFI_IP {10, 42, 0, 101}
    #define WIFI_DNS {8, 8, 8, 8}
    #define WIFI_GATEWAY {10, 42, 0, 1}
    //----------------------------------------------------------------------------------------


    //----------------------------------------------------------------------------------------
    // PLACE BEACON (UUID -> ALL beacons place, (Major,Minor) -> This beacon place)
    //----------------------------------------------------------------------------------------
    #define PLACE_UUID "fffffffe-ffff-fffe-ffff-fffefffffffd"
    #define MAJOR_ID 1
    #define MINOR_ID 1
    //----------------------------------------------------------------------------------------


    //----------------------------------------------------------------------------------------
    // EQUIPMENT BEAACONS
    //----------------------------------------------------------------------------------------
    #define EQUIPMENT_UUID "fffffffe-ffff-fffe-ffff-fffefffffffe"
    //----------------------------------------------------------------------------------------


    //----------------------------------------------------------------------------------------
    // CA
    //----------------------------------------------------------------------------------------
    #define CA  "-----BEGIN CERTIFICATE-----\n" \
                "MIIDcjCCAlqgAwIBAgIJAIqOrgkz6LAyMA0GCSqGSIb3DQEBCwUAME4xCzAJBgNV\n" \
                "BAYTAkVTMQswCQYDVQQIDAJFUzELMAkGA1UEBwwCRVMxCzAJBgNVBAoMAmNhMQsw\n" \
                "CQYDVQQLDAJjYTELMAkGA1UEAwwCY2EwHhcNMTkwODA5MTUwODEyWhcNMjQwODA3\n" \
                "MTUwODEyWjBOMQswCQYDVQQGEwJFUzELMAkGA1UECAwCRVMxCzAJBgNVBAcMAkVT\n" \
                "MQswCQYDVQQKDAJjYTELMAkGA1UECwwCY2ExCzAJBgNVBAMMAmNhMIIBIjANBgkq\n" \
                "hkiG9w0BAQEFAAOCAQ8AMIIBCgKCAQEA0S/uzVO2O4hB6/Dp6sF+qXa8F4T7nHE1\n" \
                "T2ojeLRpipLvu8psbLVdDFHDgTImefPPpPmd0gUh0kPxBFbgAdiG7LvLiRWmVhaT\n" \
                "Igoms6XAAnEBaW3XwF92IsO8M9vq6IAQgsx2zb23uVyFX1CY1bRXNY+OGvE7CtgH\n" \
                "ksJXgmm0p+4JPBWholMupBdbS81Sy5bShjsoD/KEz0eKgl0XQH2nxQYlSYzjno6b\n" \
                "zfTl+qvP1ntIFdeCTxW/+3c0AQDL6wj3Pi/z2Pt+sK+rsR5t+2T8SgrQIZHJ2tO4\n" \
                "bQdIAaHjqxonDBQCA7B/6GytyWmZ2GJ4WjVh+cksLsVkjtW7bfwuEQIDAQABo1Mw\n" \
                "UTAdBgNVHQ4EFgQUAJYd84p/PH8bIZpbku12NyRxcRIwHwYDVR0jBBgwFoAUAJYd\n" \
                "84p/PH8bIZpbku12NyRxcRIwDwYDVR0TAQH/BAUwAwEB/zANBgkqhkiG9w0BAQsF\n" \
                "AAOCAQEASLI8m7nfU8y6yPSH9a4zkBJG0zSGwW83KTv4nO5YW4GVx+MLCWZVu1FU\n" \
                "3ohgJOz3gTridBpJgTG1kuEwYwstjCeLANJjMEIKamoQVEZMFYl0YoxBQbIDtcsL\n" \
                "vBKSEX46ug8YVSG5xFjhjc2/A+uXXXtwFPBhqZ97vevRbMrz59mzlqY2w+RNlnJV\n" \
                "Es2hIjKcmLQr5f4mBO48NIE7QggvGnRN51Z/GRBWzKvdR/lQ3ajXAAUykZBHT61T\n" \
                "tAU2RKL1HcmTxMUmYvMFvKd+jvwT9hUIGBLaJz/4wTsuEMM6S8L0eCU1OjWkjVxC\n" \
                "TIvBAilwUhY+CWnbCzlwjwGJPdc3xg==\n" \
                "-----END CERTIFICATE-----\n" \
    //----------------------------------------------------------------------------------------
#endif