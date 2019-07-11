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
                "MIIDcjCCAlqgAwIBAgIJANKAZDMg41XXMA0GCSqGSIb3DQEBCwUAME4xCzAJBgNV\n" \
                "BAYTAkVTMQswCQYDVQQIDAJFUzELMAkGA1UEBwwCRVMxCzAJBgNVBAoMAkVTMQsw\n" \
                "CQYDVQQLDAJjYTELMAkGA1UEAwwCY2EwHhcNMTkwNzA3MTQ1MTAwWhcNMTkwODA2\n" \
                "MTQ1MTAwWjBOMQswCQYDVQQGEwJFUzELMAkGA1UECAwCRVMxCzAJBgNVBAcMAkVT\n" \
                "MQswCQYDVQQKDAJFUzELMAkGA1UECwwCY2ExCzAJBgNVBAMMAmNhMIIBIjANBgkq\n" \
                "hkiG9w0BAQEFAAOCAQ8AMIIBCgKCAQEA0S/uzVO2O4hB6/Dp6sF+qXa8F4T7nHE1\n" \
                "T2ojeLRpipLvu8psbLVdDFHDgTImefPPpPmd0gUh0kPxBFbgAdiG7LvLiRWmVhaT\n" \
                "Igoms6XAAnEBaW3XwF92IsO8M9vq6IAQgsx2zb23uVyFX1CY1bRXNY+OGvE7CtgH\n" \
                "ksJXgmm0p+4JPBWholMupBdbS81Sy5bShjsoD/KEz0eKgl0XQH2nxQYlSYzjno6b\n" \
                "zfTl+qvP1ntIFdeCTxW/+3c0AQDL6wj3Pi/z2Pt+sK+rsR5t+2T8SgrQIZHJ2tO4\n" \
                "bQdIAaHjqxonDBQCA7B/6GytyWmZ2GJ4WjVh+cksLsVkjtW7bfwuEQIDAQABo1Mw\n" \
                "UTAdBgNVHQ4EFgQUAJYd84p/PH8bIZpbku12NyRxcRIwHwYDVR0jBBgwFoAUAJYd\n" \
                "84p/PH8bIZpbku12NyRxcRIwDwYDVR0TAQH/BAUwAwEB/zANBgkqhkiG9w0BAQsF\n" \
                "AAOCAQEALvgNYcpEH5WmeYGYiyXqqmtW9ss+YVu/bIB8tJZnw2yh7ORKev1857bQ\n" \
                "rCC53TkmNPLthAEoWeAfHd971SzP+wPGThbzZfUGY+DfvHVqq5ZGvOG1ZSVUQTqh\n" \
                "PAZJ9V1CZgJTkcJvb+tICT41K82MiaYfcI9qUwgJ6ewLEQqYx0nonhnDxOOp698H\n" \
                "DYW5r8SPTHJ3sCrNsWKQnBua768F73qq29UhWEggt5dw07q/A2suCxAuahqvQDr1\n" \
                "UVFQf4vi0wWx7odtl7SRbk5bQD42LZHFjRlWcF7Xnk09fSrIe9VRf/dSEEUdesse\n" \
                "gKzpCYwLc8J5yo9efKVX6byXX+s/7w==\n" \
                "-----END CERTIFICATE-----\n"
    //----------------------------------------------------------------------------------------
#endif