#include "mpu9250_manger.h"
#include <Wire.h>

//defining static members of the class
MPU9250_WE* MPU9250Manager::mpu = new MPU9250_WE(MPU9250_ADDR);
float MPU9250Manager::linearAcceleration = 0.0;


/**
 * @brief Initializes the MPU9250 sensor by setting up the I2C communication, checking the connection, configuring the accelerometer range, and disabling filters for faster output.
 * @note it uses the default I2C pins defined in config.h for communication with the sensor. It also sets the accelerometer range to ±16g and disables the digital low-pass filter (DLPF) for faster output.
 * @return {bool} - Returns true if the sensor is initialized successfully, false otherwise.
 */
bool MPU9250Manager::init() {
    Wire.begin(SDA_PIN, SCL_PIN);
    if(!MPU9250Manager::mpu->init()){
        Serial.println("MPU9250 not connected");
        return false;
    }
    Serial.println("MPU9250 connected");

    // Set accelerometer range to ±16g
    MPU9250Manager::mpu->setAccRange(MPU9250_ACC_RANGE_16G);

    // Disable filters for faster output
    MPU9250Manager::mpu->enableAccDLPF(false);
    return true;
}

/**
 * @brief Updates the linear acceleration by reading the acceleration values from the MPU9250 sensor, calculating the magnitude of the acceleration, converting it to m/s^2, and removing the effect of gravity. The calculated linear acceleration is stored in a static member variable for later retrieval.
 * @note The method reads the acceleration values in g from the sensor, calculates the magnitude of the acceleration vector, converts it to m/s^2 by multiplying with 9.81, and then removes the effect of gravity (1g) to get the linear acceleration. The resulting linear acceleration is stored in a static member variable for later use.
 * @return void
 */
void MPU9250Manager::update(){

    //object to hold the acceleration values in g
    xyzFloat g = MPU9250Manager::mpu->getGValues();
    float magnitudeG = sqrt(g.x*g.x + g.y*g.y + g.z*g.z);

    // converting to m/s^2 and removing gravity and storing the linear acceleration in the static member variable
    MPU9250Manager::linearAcceleration = (magnitudeG - 1.0) * 9.81; 

}



