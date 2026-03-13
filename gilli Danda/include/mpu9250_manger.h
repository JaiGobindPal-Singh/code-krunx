/**
 * @file mpu9250_manager.h
 * @brief This header file defines the MPU9250Manager class, which provides methods for initializing and updating the MPU9250 sensor. The class includes a static member variable to hold the linear acceleration value calculated from the sensor readings. The implementation of these methods is expected to be in the corresponding source file (mpu9250_manager.cpp).
 * @author JGPS
 * @date 2025-03-13
 */
#ifndef MPU9250_MANAGER_H
#define MPU9250_MANAGER_H
#include "config.h"
#include <MPU9250_WE.h>
#define MPU9250_ADDR 0x68


class MPU9250Manager {
  public:
  static bool init();
  static void update();
  private:
  static MPU9250_WE* mpu;
  static float linearAcceleration;

};
#endif