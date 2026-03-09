/**
 * @file config.h
 * @brief This header file contains the configuration constants and global variables used in the project and must be imported in very beginning.it also contains the utility functions that are used in multiple files to avoid code repetition and make the code more modular.
 */

#ifndef CONFIG_H
#define CONFIG_H
#include <Arduino.h> //imported for using the arduino specific data types like uint16_t

//* configuration constants
//* constants must be constexpr to prevent linker errors and optimize the code

constexpr uint16_t SDA_PIN = 21;
constexpr uint16_t SCL_PIN = 22;
constexpr uint16_t HTTP_TIMEOUT = 1000;
constexpr uint16_t SAMPLE_RATE_MS = 10;
constexpr uint16_t RINGING_THRESHOLD = 200;
constexpr uint16_t LED_BUILTIN = 5;
constexpr uint16_t STRIKE_THRESHOLD = 5;
constexpr uint16_t STRIKE_DELAY = 100;
constexpr size_t BUFFER_SIZE = 200;

//* utility functions 
//* note: utility functions must be inline to prevent linker errors

/**
 *  function to clear the serial input buffer to prevent any unwanted data from being read
 */
inline void clearSerialInputBuffer()
{
    while (Serial.available() > 0)
    {
        Serial.read();
    }
}

#endif