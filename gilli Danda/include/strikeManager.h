#ifndef STRIKEMANAGER_H
#define STRIKEMANAGER_H
#include "config.h"


class StrikeManager{
    private:
    unsigned long lastStrike = 0;
    public:
    bool detectStrike(const float);
};

#endif