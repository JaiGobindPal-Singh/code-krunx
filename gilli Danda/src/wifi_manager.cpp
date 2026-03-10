#include "wifi_manager.h"
#include <WiFi.h>

/**
 * @brief Connects to a Wi-Fi network using the provided SSID and password.
 * @param {char*} SSID - The SSID of the Wi-Fi network to connect to.
 * @param {char*} WIFI_PASS - The password for the Wi-Fi network.
 * @return {bool} - Returns true if the connection is successful, false otherwise.
 */
bool WifiManager::connectWifi(const char *SSID, const char *WIFI_PASS)
{
    // disconnect any previous wifi connection
    WiFi.disconnect(true);
    delay(1000);

    // setting wifi mode and setSleep to false to prevent powersave wifi mode which can cause disconnection during operation
    WiFi.mode(WIFI_STA);
    WiFi.setSleep(false);

    // setting up the wifi connection
    WiFi.begin(SSID, WIFI_PASS);

    // connecting to wifi with a timeout of 90 seconds
    int i = 0;
    Serial.print("connecting to wifi ...");
    while (WiFi.status() != WL_CONNECTED)
    {
        vTaskDelay(pdMS_TO_TICKS(500));
        i++;
        // Serial.print(WiFi.status());
        Serial.print(".");
        if (WiFi.status() == WL_CONNECT_FAILED || i > 180)
        {
            Serial.println("Error: Connection failed. Possible incorrect SSID/password OR Timeout");
            return false;
        }
    }
    return true;
}

/**
 * @brief Scans for available Wi-Fi networks and prints their SSIDs and signal strengths to the serial monitor.
 * @return {void} return nothing, it just prints the available networks to the serial monitor
 */
void WifiManager::scanWifiNetworks()
{
    // partition line for better readability in serial monitor
    Serial.println("------------------------------------------------------");
    Serial.println("Scanning for WiFi networks...");
    // scanning for wifi networks and printing the results
    int n = WiFi.scanNetworks();
    if (n == 0)
    {
        Serial.println("no networks found");
    }
    else
    {
        Serial.print(n);
        Serial.println(" networks found");
        for (int i = 0; i < n; ++i)
        {
            // Print SSID and RSSI for each network found
            Serial.print(i + 1);
            Serial.print(": ");
            Serial.print(WiFi.SSID(i));
            Serial.print(" (");
            Serial.print(WiFi.RSSI(i));
            Serial.print(")");
            Serial.println((WiFi.encryptionType(i) == WIFI_AUTH_OPEN) ? " " : "*");
            delay(10);
        }
    }
}

/**
 * @brief Handles the user request to connect to a Wi-Fi network by prompting for SSID and password, and then attempting to connect using the provided credentials.it also saves the wifi credentials in storage for auto connection
 * 
 * @return {bool} - Returns true if the connection is successful, false otherwise.
 */
bool WifiManager::handleUserWifiConnectionRequest()
{
    clearSerialInputBuffer(); // clearing input buffer

    // prompting user for wifi SSID
    Serial.println("enter the wifi SSID");
    unsigned long startWait = millis();
    while (Serial.available() == 0)
    {
        if (millis() - startWait > 30000)
        {
            Serial.println("Timed Out.");
            return false;
        }
        yield(); // Let the ESP32 handle background tasks.
    } // waiting for user input
    String wifi_ssid = Serial.readStringUntil('\n'); // reading the input
    wifi_ssid.trim();
    Serial.println("SSID: " + wifi_ssid);

    clearSerialInputBuffer(); // clearing input buffer

    // prompting user for wifi password
    Serial.println("enter the wifi Password");
    startWait = millis();
    while (Serial.available() == 0)
    {
        if (millis() - startWait > 30000)
        { // 30 second timeout
            Serial.println("Timed Out.");
            return false;
        }
        yield(); // Let the ESP32 handle background tasks (Wi-Fi, etc.)
    } // waiting for user input
    String wifi_password = Serial.readStringUntil('\n'); // reading the input
    wifi_password.trim();
    Serial.println("Password: " + wifi_password);

    // connecting to wifi with the provided credentials
    if(!WifiManager:: connectWifi(wifi_ssid.c_str(), wifi_password.c_str())){
        Serial.println("wifi connection failed");
        return false;
    }

    //todo saving wifi credentials to storage for future use and auto connection
    Serial.println("wifi connected");
    return true;
}

/**
 * @brief Connects to the default Wi-Fi network using credentials stored in the ESP32's storage. It retrieves the SSID and password from storage, attempts to connect to the Wi-Fi network.
 * @return {bool} - Returns true if the connection is successful, false otherwise.
 */
bool WifiManager:: connectDefaultWifi(){
    //todo get wifi credentials from storage and connect to wifi if credentials are valid and available, return true if connected successfully, otherwise return false
    return false;
}