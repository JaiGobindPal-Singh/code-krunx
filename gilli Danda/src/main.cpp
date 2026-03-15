/**
 * @file main.cpp
 * @brief entry point of the program, contains the setup and loop functions for the Arduino framework
 *
 * @author JGPS
 * @date 2026-03-13
 */
#include "config.h"
#include "circularBufferSensorData.h"
#include "mpu9250_manger.h"
#include "server_manager.h"
#include "storage_manager.h"
#include "strikeManager.h"
#include "wifi_manager.h"
#include <ArduinoJson.h>
#include <WiFi.h>
#include <HTTPClient.h>

//* globlal initializations
SemaphoreHandle_t httpMutex; // thread manager

JsonDocument doc; // json document for managing the json data

String requestData; // requestData is sensor data in json string format that will be sent over http

WORKING_STATUS LED_indicator; // LED_indicator controls the LED_BUILTIN

uint16_t delayStrike = 0; // variable to control the strike delay and post data

//* Helper Methods
/**
 * @brief LEDManager is a task that manages the LED_BUILTIN to indicate the working status of the esp32. It uses the LED_indicator variable to determine the status and blinks the LED accordingly.
 *
 * @note this task runs in an infinite loop and should be created as a separate task in the setup function to run concurrently with the main loop.
 *
 * @param pvParameters not used {reserved for async parameters if needed in future}

 * @return void
 */
void LEDManager(void *pvParameters)
{
    for (;;)
    { // Infinite loop
        if (LED_indicator == WORKING_STATUS::CONNECTING)
        {

            digitalWrite(LED_BUILTIN, HIGH); // Turn the LED on
            vTaskDelay(pdMS_TO_TICKS(200));  // Wait for 200 milliseconds
            digitalWrite(LED_BUILTIN, LOW);  // Turn the LED off
            vTaskDelay(pdMS_TO_TICKS(200));  // Wait for 200 milliseconds
        }
        else if (LED_indicator == WORKING_STATUS::STABLE)
        {
            digitalWrite(LED_BUILTIN, HIGH);
            vTaskDelay(pdMS_TO_TICKS(1000));
        }
        else if (LED_indicator == WORKING_STATUS::ERROR)
        {
            digitalWrite(LED_BUILTIN, HIGH); // Turn the LED on
            vTaskDelay(pdMS_TO_TICKS(500));  // Wait for 500 milliseconds
            digitalWrite(LED_BUILTIN, LOW);  // Turn the LED off
            vTaskDelay(pdMS_TO_TICKS(500));  // Wait for 500 milliseconds
        }
    }
}

/**
 * @brief sendPostDataOverHttpAsync is a task that sends the sensor data over http in an asynchronous way. It takes the sensor data in json string format as a parameter and sends it to the server using the HTTPClient library. It also manages the response from the server and updates the LED indicator accordingly.
 * @return void
 * @note this task should be created as a separate task in the main loop or in the
 */
void sendPostDataOverHttpAsync(void *pvParameters)
{
    //*commented for temp time to check if system can handle multiple http request without mutex, if it works fine we can remove the mutex to optimize the code and prevent any possible deadlocks in future
    /*
    testing
    // only allowing one http request at a time
    if (xSemaphoreTake(httpMutex, 0) != pdTRUE)
    {
        delete (String *)pvParameters;
        vTaskDelete(NULL);
        return;
    }
    */

    // clearing memory of the parameter after copying the data to prevent memory leaks and optimize memory usage
    String data = *((String *)pvParameters);
    delete (String *)pvParameters;

    if (WiFi.status() != WL_CONNECTED)
    {
        Serial.println("wifi not connected");
        // starting the error indicator
        LED_indicator = WORKING_STATUS::ERROR;

        /*
        testing
        // allowing other http request and deleting the task
        xSemaphoreGive(httpMutex);
        vTaskDelete(NULL);
        */
        return;
    }
    HTTPClient http; // http object to manage http communications
    http.setTimeout(HTTP_TIMEOUT);

    // Initialize HTTP request and configuration
    String fullUrl = ServerManager::getServer() + "sensor-data/";
    http.begin(fullUrl.c_str());
    http.addHeader("Content-Type", "application/json");

    // Send POST request
    int httpResponseCode = http.POST(data);

    // managing response from server
    String response;
    if (httpResponseCode == HTTP_CODE_OK)
    {
        response = http.getString();
        Serial.println(response);
    }
    else
    {
        // starting the error indicator
        LED_indicator = WORKING_STATUS::ERROR;
    }
    http.end(); // free up resources
    vTaskDelete(NULL);

    /*testing
    xSemaphoreGive(httpMutex); // allow other http request
    */
}

/**
 * @brief sendDataOverHttpThread is a function that creates a task to send the sensor data over http in an asynchronous way. It takes the sensor data in json string format as a parameter and creates a task to send it using the sendPostDataOverHttpAsync function. It also manages the memory of the parameter to prevent memory leaks and optimize memory usage.
 * @param {String} param - sensor data in json string format that will be sent over http
 * @return void
 */
void sendDataOverHttpThread(String param)
{
    // creating copy of payload to send to the async task and storing result
    String *payload = new String(param);
    BaseType_t result = xTaskCreatePinnedToCore(
        sendPostDataOverHttpAsync, /*  Task function.              */
        "sending data",            /*   Name of task.              */
        8192,                      /*    Stack size of task        */
        payload,                   /*     Parameter of the task    */
        3,                         /*      Priority of the task    */
        NULL,                      /*       Task ID                */
        0                          /*       Task core              */
    );

    // freeing memory if task creation failed
    if (result != pdPASS)
    {
        delete payload;
        Serial.println("Error: Failed to create HTTP task, memory freed.");
    }
}

/**
 * @brief manageLEDThread is a function that creates a task to manage the LED_BUILTIN to indicate the working status of the esp32. It uses the LED_indicator variable to determine the status and blinks the LED accordingly. It should be called in the setup function to start the LED management task.
 * @return void
 */
void manageLEDThread()
{
    xTaskCreatePinnedToCore(
        LEDManager,    /* Task function. */
        "led manager", /* Name of task. */
        1024,          /* Stack size of task */
        NULL,          /* Parameter of the task */
        1,             /*Priority of the task */
        NULL,
        1);
}

//*Main Controller Methods
void controllerLoop(void *pvParameters)
{
    for (;;)
    {
        // updating the sensor data and calculating the linear acceleration magnitude
        MPU9250Manager::update();

        // magnitude contains the linear acceleration calculated from the sensor data
        float magnitude = MPU9250Manager::linearAcceleration;

        // if strike is detected send data
        if (StrikeManager::detectStrike(magnitude))
        {
            SensorData *liveData; // liveData will contain the sensor data in array format

            // adding a delay to send strike for more accuracy and inserting the data in buffer during the delay
            while (delayStrike < (STRIKE_DELAY / SAMPLE_RATE_MS))
            {
                delayStrike++;
                vTaskDelay(pdMS_TO_TICKS(SAMPLE_RATE_MS));
                MPU9250Manager::update();
                magnitude = MPU9250Manager::linearAcceleration;
                CircularBuffer::insertData(magnitude); // inserting the data in buffer during the delay
            }

            // retreiving the data from buffer
            liveData = CircularBuffer::getData();
            uint16_t amountOfData = CircularBuffer::getSampleCount();

            doc.clear(); // clearing previous data

            // converting data to json format for transmission
            JsonArray arr = doc["sensorData"].to<JsonArray>(); // creating the array
            for (uint16_t iter = 0; iter < amountOfData; iter++)
            {
                // Add an object to the array for each struct
                JsonObject obj = arr.add<JsonObject>();
                obj["readTime"] = liveData[iter].readTime;
                obj["magnitude"] = liveData[iter].magnitude;
            }
            requestData = "";
            serializeJson(doc, requestData);

            // send data over http async way
            sendDataOverHttpThread(requestData);
            vTaskDelay(pdMS_TO_TICKS(50)); // Giving task time to copy data

            // resetting the buffer so it can store new values
            CircularBuffer::resetBuffer();
        }

        delayStrike = 0; // resetting the delay
        vTaskDelay(pdMS_TO_TICKS(SAMPLE_RATE_MS));
    }
}

void setup()
{
    // Starting serial communication for debugging and user information
    Serial.begin(115200);

    // reserving the memory to prevent fragmentation
    requestData.reserve(16000);

    /* testing
    httpMutex = xSemaphoreCreateMutex(); // defining the mutex to manage threads
    */

    // setting up led indicator thread
    pinMode(LED_BUILTIN, OUTPUT);
    LED_indicator = WORKING_STATUS::CONNECTING;
    manageLEDThread();

    // printing available wifi networks for debugging and user information
    WifiManager::scanWifiNetworks();

    uint8_t defaultCounter = 0;

    // prompting user to choose btw custom credentials or saved credentials
    Serial.println("press <space> + <enter> to prevent using saved credentials");
    Serial.println("press <space> + <enter> to prevent using saved credentials");
    Serial.print("using saved credentials in ");
    while (defaultCounter <= 5) // 5s timer
    {
        if (Serial.available())
        {
            Serial.read();
            break;
        }
        Serial.print(String(5 - defaultCounter) + " ");
        delay(1000);
        defaultCounter++;
    }
    Serial.println();         // for output formatting
    clearSerialInputBuffer(); // clearing the input buffer

    // managing the credentials input and default connections
    if (defaultCounter <= 5)
    {
        while (!WifiManager::handleUserWifiConnectionRequest())
            ; // managing the custom wifi connection
        while (!ServerManager::handleUserWebserverUpdationRequest())
            ; // managing custom webserver connection
    }
    else
    {
        Serial.println("using saved credentials...");
        if (!WifiManager::connectDefaultWifi())
        {
            while (!WifiManager::handleUserWifiConnectionRequest())
                ; // prompting user incase wifi credentials invalid
        }
        if (!ServerManager::connectDefaultWebserver())
        {
            while (!ServerManager::handleUserWebserverUpdationRequest())
                ; // prompting user incase webserver adress is invalid
        }
    }

    // initializing I2C and MPU9250 sensor
    MPU9250Manager::init();

    // setting led indicator to stable after successful connections and initialization
    LED_indicator = WORKING_STATUS::STABLE;

    // execute the controller after core setup
    xTaskCreatePinnedToCore(
        controllerLoop,
        "sensorTask",
        12288,
        NULL,
        2,
        NULL,
        1 // Core 1 (application core)
    );
}

void loop()
{
    vTaskDelay(portMAX_DELAY); // preventing loop from execution
}
