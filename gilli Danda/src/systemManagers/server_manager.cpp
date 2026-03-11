#include "server_manager.h"
#include "wifi_manager.h"
#include <HTTPClient.h>
#include "storage_manager.h"

// server adress
String ServerManager::serverAdress = "";

/**
 * @brief returns the current server adress
 * @return {String} - the current server adress
 */
String ServerManager::getServer()
{
    return serverAdress;
}

/**
 * @brief tests the server connection by sending a GET request to a test endpoint and checking for a successful response.
 * @return {bool} - Returns true if the server is reachable and responds successfully, false otherwise.
 */
bool ServerManager::testServer()
{
    if (ServerManager::serverAdress == "")
    {
        return false; // No server address set
    }
    // checking wifi status and trying to connect if not connected
    if (WiFi.status() != WL_CONNECTED)
    {
        Serial.println("wifi not connected");
        Serial.println("attempting to connect to wifi with stored credentials...");

        if (!WifiManager::connectDefaultWifi())
        {
            Serial.println("wifi connection failed");
            return false;
        }
        Serial.println("wifi connected");
    }

    // creating http client and setting timeout for http operations
    HTTPClient httpClient;
    httpClient.setTimeout(HTTP_TIMEOUT);

    // constructing the full server url for testing
    bool testSuccess = false;
    String serverUrl = ServerManager::serverAdress + "test-endpoint/";
    Serial.println("testing server connection with url: " + serverUrl);

    // initializing http request
    httpClient.begin(serverUrl);
    httpClient.addHeader("Content-Type", "application/json");

    // sending GET request to test the server connection
    int httpResponseCode = httpClient.GET();

    // setting test success to true if we get HTTP 200 OK response from the server
    if (httpResponseCode == HTTP_CODE_OK)
    {
        testSuccess = true;
    }

    // clearing up http client resources and returning the test result
    httpClient.end();
    return testSuccess;
}

/**
 * @brief Sets the server address to the provided value and tests the connection to the new server. It trims the input, updates the server address, and then calls testServer() to verify connectivity.
 * @param {String} prompt - The new server address to set.
 * @return {bool} - Returns true if the server address is set successfully and the connection test passes, false otherwise.
 */
bool ServerManager::setServer(String prompt)
{
    // trimming the prompt to remove any leading or trailing whitespace
    prompt.trim();

    // setting the server address to the provided prompt
    ServerManager::serverAdress = prompt;
    delay(10);

    // testing the server connection with the new address and returning the result
    bool connectionStatus = ServerManager::testServer();
    if (!connectionStatus)
    {
        return false;
    }
    return true;
}

/**
 * @brief Handles the user request to update the webserver information by prompting for a new server address, setting it, and testing the connection.
 * @return {bool} - Returns true if the server address is updated successfully and the connection test passes, false otherwise.
 */
bool ServerManager::handleUserWebserverUpdationRequest()
{
    clearSerialInputBuffer(); // clearing input buffer
    String tempServerAdress;
    // prompting user for webserver adress
    Serial.println("enter the webServer adress");
    unsigned long startWait = millis();
    while (Serial.available() == 0)
    {
        if (millis() - startWait > 30000)
        { // 30 second timeout
            Serial.println("Timed Out.");
            return false;
        }
        yield(); // Let the ESP32 handle background tasks (Wi-Fi, etc.)
    }
    tempServerAdress = Serial.readStringUntil('\n'); // reading the input
    tempServerAdress.trim();
    Serial.println("entered webserver adress " + tempServerAdress);

    //setting the webserver adress
    if(ServerManager::setServer(tempServerAdress)){
        Serial.println("server setup done");
        //saving details in storage for future use
        StorageManager::saveKeyValueToStorage("serverAdress", tempServerAdress);
        return true;
    }
    Serial.println("invalid url or server is not reachable");
    return false;
}

/**
 * @brief Connects to the default webserver by retrieving the server address from storage and testing the connection. It checks if the server address is available in storage, and if so, it attempts to set the server and test the connection.
 * @return {bool} - Returns true if the server address is retrieved successfully from storage, set correctly, and the connection test passes; false otherwise.
 */
bool ServerManager::connectDefaultWebserver(){

    //loading the webserver details from storage
    String webserver = StorageManager::getValueFromStorage("serverAdress");
    webserver.trim();

    //checking if the webserver details are available in storage
    if(webserver == "-1"){
        Serial.println("default webserver not found");
        return false;
    }

    //connecting to server and returning the connection status
    if(!ServerManager::setServer(webserver)){
        Serial.println("webserver not available");
        return false;
    };
    Serial.println("Webserver connected");
    return true;
}