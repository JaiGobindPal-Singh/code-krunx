/**
 * @file strikeManager.h
 * @brief This header file defines the StrikeManager class, which provides a method for detecting strikes based on accelerometer data.
 * @author JGPS
 * @date 2025-03-09
 */

#ifndef STRIKEMANAGER_H
#define STRIKEMANAGER_H
#include "config.h"


class StrikeManager{
    private:
    static unsigned long lastStrike;
    public:
    static bool detectStrike(const float);
};

#endif