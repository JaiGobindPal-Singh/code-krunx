/**
 * @file config.h
 * @brief This header file contains the configuration constants and global variables used in the project and must be imported in very beginning.it also contains the utility functions that are used in multiple files to avoid code repetition and make the code more modular.
 * @author JGPS
 * @date 2025-03-10
 */

#ifndef CONFIG_H
#define CONFIG_H
#include <Arduino.h> //imported for using the arduino specific data types like uint16_t

//* configuration constants
//* constants must be constexpr to prevent linker errors and optimize the code

constexpr int SDA_PIN = 21; //SDA pin for I2C communication with the sensor
constexpr int SCL_PIN = 22; //SCL pin for I2C communication with the sensor
constexpr uint16_t HTTP_TIMEOUT = 2000; //timeout for HTTP requests in milliseconds
constexpr uint8_t SAMPLE_RATE_MS = 10;  //sample rate for reading sensor data in milliseconds
constexpr uint16_t RINGING_THRESHOLD = 200; //threshold to prevent false strike detection 
constexpr uint8_t LED_BUILTIN = 5;  //built in led pin for esp32, used for debugging and indication purposes
constexpr uint16_t STRIKE_THRESHOLD = 30;    //threshold for detecting strike
constexpr uint16_t STRIKE_DELAY = 100;  //delay in ms after strike to send post request 
constexpr size_t BUFFER_SIZE = 200; // buffer size of circular buffer
constexpr uint16_t WIFI_CONNECT_TIMEOUT = 90;  //timeout in seconds for wifi connection attempts
constexpr uint16_t CAPTIVE_PORTAL_TIMEOUT = 120;  //timeout in seconds for captive portal

//* utility functions 
//* note: utility functions must be inline to prevent linker errors

/**
 *  @brief Function to clear the serial input buffer to prevent any unwanted data from being read
 *  @return void
 */
inline void clearSerialInputBuffer()
{
    while (Serial.available() > 0)
    {
        Serial.read();
    }
}

//enum class to manage the working status of the esp32 and control the led indicator accordingly
enum class WORKING_STATUS
{
    STABLE,
    CONNECTING,
    ERROR
};

/**
 * @brief absolute the float value
 * @param {float} the float value to be absoluted
 * @return {float} the absoluted value
 */
inline float absFloat(float x) {
    return x < 0 ? -x : x;
}
#endif