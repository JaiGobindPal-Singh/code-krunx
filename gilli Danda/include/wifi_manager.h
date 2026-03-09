#ifndef WIFI_MANAGER_H
#define WIFI_MANAGER_H
#include "config.h"

class WifiManager{
    public:
    static bool connectWifi(const char*, const char*);
    static void scanWifiNetworks();
    static bool handleUserWifiConnectionRequest();
    static bool connectDefaultWifi();
};

#endif