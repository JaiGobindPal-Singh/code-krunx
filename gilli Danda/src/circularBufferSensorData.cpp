#include "circularBufferSensorData.h"

// structure of sensor data
struct SensorData
{
    unsigned long readTime;
    float magnitude;
};
// CircularBuffer will handle and store the sensor data
class CircularBuffer
{
private:
    SensorData dataBuffer[BUFFER_SIZE];
    SensorData res[BUFFER_SIZE]; // this array will contain data in proper order
    int head = 0;                // starting pointer of buffer
    bool isFull = false;         // flag to check array is full
public:
    // function to insert data in buffer
    void insertData(float magnitude)
    {
        // storing the data
        dataBuffer[head].readTime = millis();
        dataBuffer[head].magnitude = magnitude;

        // updating head
        head++;
        if (head >= BUFFER_SIZE)
        {
            head = 0;      // move head to 0 if end reached and overwrite previous data
            isFull = true; // set isFull to true
        }
    }
    // function to get the sensor data in order
    SensorData *getData()
    {
        int j = 0; // res pointer
        // condition to check if data is demanded before 3s
        if (!isFull)
        {
            for (int node = 0; node < head; node++)
            {
                res[node] = dataBuffer[node];
            }
        }
        else
        {
            // storing the data at right size of circular buffer
            for (int i = head; i < BUFFER_SIZE; i++)
            {
                res[j] = dataBuffer[i];
                j++;
            }
            // storing the data at left size of circular buffer
            for (int k = 0; k < head; k++)
            {
                res[j] = dataBuffer[k];
                j++;
            }
        }
        return res;
    }
    // function to get total number of data samples available
    int getSampleCount()
    {
        return isFull ? BUFFER_SIZE : head;
    }
    void resetBuffer()
    {
        head = 0;
        isFull = false;
    }
};
