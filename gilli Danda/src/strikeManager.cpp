#include "strikeManager.h"


/**
 * @brief Detects a strike based on the magnitude of the sensor reading and the time since the last detected strike. A strike is detected if the magnitude exceeds the defined STRIKE_THRESHOLD and if the time since the last detected strike exceeds the RINGING_THRESHOLD to prevent multiple detections from a single strike.
 * 
 * @param {float} magnitude - The magnitude of the sensor reading to be evaluated for strike detection.
 * @return {bool} Returns true if a strike is detected, otherwise returns false.
 */
bool StrikeManager::detectStrike(const float magnitude)
{
    unsigned long currentTime = millis();

    //checking if magnitude exceeds the strike threshold and if the time since last strike is greater than ringing threshold to prevent multiple detections from a single strike
    if (magnitude > STRIKE_THRESHOLD &&
        (currentTime - lastStrike) > RINGING_THRESHOLD)
    {
        lastStrike = currentTime;
        return true;
    }
    return false;
}