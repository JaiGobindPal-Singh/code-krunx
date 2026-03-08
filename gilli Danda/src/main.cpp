#include <Arduino.h>
#include "config.h"
#include "circularBufferSensorData.h"
#include "strikeManager.h"


//definitions in config.h
// #define SDA_PIN 21              //SDA pin
// #define SCL_PIN 22              //SCL pin
// #define HTTP_TIMEOUT 1000       //timeout for http

// #define SAMPLE_RATE_MS 10       //delay between the data
// #define RINGING_THRSHOLD 200    //ringing threshold to prevent sending the data repedetly
// #define LED_BUILTIN 5           //default led
// #define STRIKE_THRESHOLD 5     //strike threshold
// #define STRIKE_DELAY 100         //strike delay threshold

void setup() {
pinMode(5, OUTPUT);
Serial.begin(115200);
}

void loop() {

}

