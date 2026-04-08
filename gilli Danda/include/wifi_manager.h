/**
 * @file wifi_manager.h
 * @brief This header file defines the WifiManager class, which provides methods for managing Wi-Fi connections on an ESP32 device. The class includes functions to connect to a Wi-Fi network, scan for available networks, handle user requests for Wi-Fi connection, and connect to a default Wi-Fi network using stored credentials. The implementation of these methods is expected to be in the corresponding source file (wifi_manager.cpp).
 * @author JGPS
 * @date 2025-03-09
 */

#ifndef WIFI_MANAGER_H
#define WIFI_MANAGER_H
#include "config.h"

class WifiManager{
    public:
    static bool connectWifi(const char*, const char*);
    static void scanWifiNetworks();
    static bool handleUserWifiConnectionRequest();
    static bool connectDefaultWifi();
    static void storeWifiCredentials(const char* SSID, const char* password);
    static bool openCaptivePortalWithParams(const char* pId, const char* pLabel, const char* pDefault, const int psize, String& paramValue);
};

#endif