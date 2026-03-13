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

    /*testing
    xSemaphoreGive(httpMutex); // allow other http request
    vTaskDelete(NULL);
    */
    return;
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
        1
    );
}




//*Main Controller Methods

void setup()
{
    pinMode(LED_BUILTIN, OUTPUT); // setting the built in led pin as output for debugging and indication purposes
    Serial.begin(115200);         // begin serial
}

void loop()
{
}
