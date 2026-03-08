/**
 * @file circularBufferSensorData.h
 * @brief This header file defines the CircularBuffer class and the SensorData structure used to manage the sensor data in a circular buffer format. The CircularBuffer class provides methods to insert new sensor data, retrieve the stored data in order, get the count of samples, and reset the buffer. The SensorData structure contains the timestamp and magnitude of the sensor readings.
 */

#ifndef CIRCULARBUFFERSENSORDATA_H
#define CIRCULARBUFFERSENSORDATA_H
#include "config.h"
#include <Arduino.h>

struct SensorData {
  unsigned long readTime;
  float magnitude;
};
class CircularBuffer
{
private:
    SensorData dataBuffer[BUFFER_SIZE];
    SensorData res[BUFFER_SIZE];
    size_t head;
    bool isFull;

public:
    CircularBuffer();  // constructor

    // function to insert data in buffer
    void insertData(float magnitude);

    // function to get the sensor data in order
    SensorData* getData();

    // function to get total number of samples
    size_t getSampleCount();

    // function to reset buffer
    void resetBuffer();
};

#endif