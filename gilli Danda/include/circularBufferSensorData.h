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
    int head;
    bool isFull;

public:
    CircularBuffer();  // constructor

    // function to insert data in buffer
    void insertData(float magnitude);

    // function to get the sensor data in order
    SensorData* getData();

    // function to get total number of samples
    int getSampleCount();

    // function to reset buffer
    void resetBuffer();
};

#endif