#include "wifi_manager.h"
#include <WiFi.h>
#include <WiFiManager.h> 
#include "storage_manager.h"

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
        if (WiFi.status() == WL_CONNECT_FAILED || i > WIFI_CONNECT_TIMEOUT * 2)
        {
            Serial.println("Error: Connection failed. Possible incorrect SSID/password OR Timeout");
            return false;
        }
    }
    return true;
}

/** DEPRECIATED
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

/**  DEPRECIATED
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
    if(!WifiManager::connectWifi(wifi_ssid.c_str(), wifi_password.c_str())){
        Serial.println("wifi connection failed");
        return false;
    }

    // saving the wifi credentials in storage for future use
    StorageManager::saveKeyValueToStorage("wifi_ssid", wifi_ssid);
    StorageManager::saveKeyValueToStorage("wifi_pass", wifi_password);
    Serial.println("wifi connected");
    return true;
}

/**
 * @brief Connects to the default Wi-Fi network using credentials stored in the ESP32's storage. It retrieves the SSID and password from storage, attempts to connect to the Wi-Fi network.
 * @return {bool} - Returns true if the connection is successful, false otherwise.
 */
bool WifiManager:: connectDefaultWifi(){

    // retrieving wifi credentials from storage
    String SSID = StorageManager::getValueFromStorage("wifi_ssid");
    SSID.trim();
    String password = StorageManager::getValueFromStorage("wifi_pass");
    password.trim();

    // checking if the wifi credentials are available in storage
    if(SSID == "-1" || password == "-1"){
        Serial.println("no saved wifi details found");
        return false;
    }

    // connecting to wifi with the retrieved credentials and returning the connection status
    if(!WifiManager::connectWifi(SSID.c_str(),password.c_str())){
        Serial.println("wifi connection failed");
        return false;
    }
    Serial.println("wifi connected");
    return true;
}

/**
 * @brief Stores the Wi-Fi credentials (SSID and password) in the ESP32's storage for future use. This allows the device to automatically connect to the Wi-Fi network in subsequent attempts without requiring user input.
 * @param {char*} SSID - The SSID of the Wi-Fi network to be stored.
 * @param {char*} password - The password for the Wi-Fi network to be stored
 * @return {void} - This function does not return any value, it just stores the credentials in storage
 */
void WifiManager:: storeWifiCredentials(const char* SSID, const char* password){
    StorageManager::saveKeyValueToStorage("wifi_ssid", String(SSID));
    StorageManager::saveKeyValueToStorage("wifi_pass", String(password));
}

/**
 * @brief Opens a captive portal to allow the user to input Wi-Fi credentials and a custom parameter. It uses the WiFiManager library to create a captive portal with a custom parameter for the server IP address. The function waits for the user to connect to the captive portal, input the required information, and then attempts to connect to the Wi-Fi network using the provided credentials. It also saves the Wi-Fi credentials in storage for future use.
 * @param {char*} pId - The ID of the custom parameter to be added to the captive portal.
 * @param {char*} pLabel - The label for the custom parameter to be displayed in the captive portal.
 * @param {char*} pDefault - The default value for the custom parameter to be displayed in the captive portal.
 * @param {int} psize - The maximum length of the custom parameter value.
 * @param {String&} paramValue - A reference to a String variable where the value of the custom parameter will be stored after the user inputs it in the captive portal.
 * @return {bool} - Returns true if the captive portal is opened successfully, the user inputs the required information, and the device connects to the Wi-Fi network; false otherwise.
 */
bool WifiManager:: openCaptivePortalWithParams(const char* pId, const char* pLabel, const char* pDefault, const int psize, String& paramValue){
    // Initialize WiFiManager
    WiFiManager wm;
    // Define the custom parameter (ID, Label, Default Value, Length)
    WiFiManagerParameter custom_server_ip(pId, pLabel, pDefault, psize);
    // Add the parameter to the portal UI
    wm.addParameter(&custom_server_ip);
    //setting portal time
    wm.setConfigPortalTimeout(CAPTIVE_PORTAL_TIMEOUT);
    //opening captive portal with the defined parameter
    if (!wm.autoConnect("Code Krunx init")) {
    Serial.println("Failed to connect, restarting...");
        return false;
    }
    //getting the wifi credentials from the captive portal and saving them in storage for future use
    String wifiName = wm.getWiFiSSID();
    String wifiPass = wm.getWiFiPass();
    WifiManager::storeWifiCredentials(wifiName.c_str(), wifiPass.c_str());

    // Save the param value into variable 
    paramValue = String(custom_server_ip.getValue());

    return true;
}
