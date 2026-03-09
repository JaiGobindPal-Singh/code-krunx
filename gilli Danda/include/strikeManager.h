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