#include "server_manager.h"
#include "wifi_manager.h"
#include <HTTPClient.h>

//server adress 
String ServerManager::serverAdress = "";

/**
 * @brief returns the current server adress
 * @return {String} - the current server adress
 */
String ServerManager::getServer(){
    return serverAdress;
}

/**
 * @brief tests the server connection by sending a GET request to a test endpoint and checking for a successful response.
 * @return {bool} - Returns true if the server is reachable and responds successfully, false otherwise.
 */
bool ServerManager::testServer(){
    if (ServerManager::serverAdress == "") {
        return false; // No server address set
    }
    //checking wifi status and trying to connect if not connected
    if(WiFi.status() != WL_CONNECTED){
        Serial.println("wifi not connected");
        Serial.println("attempting to connect to wifi with stored credentials...");

        if(!WifiManager::connectDefaultWifi()){
            Serial.println("wifi connection failed");
            return false;
        }
        Serial.println("wifi connected");
    }

    //creating http client and setting timeout for http operations
    HTTPClient httpClient;
    httpClient.setTimeout(HTTP_TIMEOUT);

    //constructing the full server url for testing
    bool testSuccess = false;
    String serverUrl = ServerManager::serverAdress + "test-endpoint/";
    Serial.println("testing server connection with url: " + serverUrl);

    //initializing http request
    httpClient.begin(serverUrl);
    httpClient.addHeader("Content-Type", "application/json");

    //sending GET request to test the server connection
    int httpResponseCode = httpClient.GET();

    //setting test success to true if we get HTTP 200 OK response from the server
    if(httpResponseCode == HTTP_CODE_OK){
        testSuccess = true;
    } 

    //clearing up http client resources and returning the test result
    httpClient.end();
    return testSuccess;
}

/**
 * @brief Sets the server address to the provided value and tests the connection to the new server. It trims the input, updates the server address, and then calls testServer() to verify connectivity.
 * @param {String} prompt - The new server address to set.
 * @return {bool} - Returns true if the server address is set successfully and the connection test passes, false otherwise.
 */
bool ServerManager::setServer(String prompt){
    //trimming the prompt to remove any leading or trailing whitespace
    prompt.trim();

    //setting the server address to the provided prompt
    ServerManager::serverAdress = prompt;
    delay(10);

    //testing the server connection with the new address and returning the result
    bool connectionStatus = ServerManager::testServer();
    if(!connectionStatus){
        return false;
    }
    return true;

}

//todo impliment other methods