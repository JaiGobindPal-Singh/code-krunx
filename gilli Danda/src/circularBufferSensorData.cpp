#include "circularBufferSensorData.h"

//initializing the static members of CircularBuffer class
SensorData CircularBuffer::dataBuffer[BUFFER_SIZE] = {};
SensorData CircularBuffer::res[BUFFER_SIZE] = {};
size_t CircularBuffer::head = 0;
bool CircularBuffer::isFull = false;


/**
 * @brief Inserts a new sensor data into the circular buffer. If the buffer is full, it overwrites the oldest data.
 *
 * @param {float} magnitude - The magnitude of the sensor reading to be inserted into the buffer.
 */
void CircularBuffer::insertData(float magnitude)
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

/**
 * @brief Retrieves the stored sensor data in order. If the buffer is not full, it returns the data from the beginning to the current head. If the buffer is full, it returns the data starting from the head to the end of the buffer, followed by the data from the beginning to the head.
 *
 * @return {SensorData*} A pointer to an array of SensorData structures containing the stored sensor data in order.
 */
SensorData *CircularBuffer::getData()
{
    size_t j = 0; // res pointer

    // condition to check if data is demanded before buffer is full
    if (!isFull)
    {
        for (size_t node = 0; node < head; node++)
        {
            res[j++] = dataBuffer[node];
        }
    }
    else
    {
        // storing the data at right size of head in circular buffer
        for (size_t i = head; i < BUFFER_SIZE; i++)
        {
            res[j++] = dataBuffer[i];
            
        }
        // storing the data at left size of head in circular buffer
        for (size_t k = 0; k < head; k++)
        {
            res[j++] = dataBuffer[k];
        }
    }
    return res;
}

/**
 * @brief Gets the total number of samples currently stored in the buffer. If the buffer is full, it returns the maximum buffer size; otherwise, it returns the current head position, which indicates the number of samples stored.
 *
 * @return {size_t} The total number of samples currently stored in the buffer.
 */
size_t CircularBuffer ::getSampleCount()
{
    return isFull ? BUFFER_SIZE : head;
}

/**
 * @brief Resets the circular buffer by setting the head back to 0 and marking the buffer as not full. This effectively clears the buffer and allows new data to be inserted from the beginning.
 *
 */
void CircularBuffer::resetBuffer()
{
    head = 0;
    isFull = false;
}
