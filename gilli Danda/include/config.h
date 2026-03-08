/**
 * @file config.h
 * @brief This header file contains the configuration constants and global variables used in the project and must be imported in very beginning.
 */

#ifndef CONFIG_H
#define CONFIG_H
#include <Arduino.h>  //imported for using the arduino specific data types like uint16_t

constexpr uint16_t SDA_PIN = 21;
constexpr uint16_t SCL_PIN = 22;
constexpr uint16_t HTTP_TIMEOUT = 1000;
constexpr uint16_t SAMPLE_RATE_MS = 10;
constexpr uint16_t RINGING_THRESHOLD = 200;
constexpr uint16_t LED_BUILTIN = 5;
constexpr uint16_t STRIKE_THRESHOLD = 5;
constexpr uint16_t STRIKE_DELAY = 100;
constexpr uint16_t BUFFER_SIZE = 200;


#endif