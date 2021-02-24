#ifndef SNIPER_H
#define SNIPER_H

#include "units/unit.h"

class Sniper
        : public Unit
{
public:
    Sniper(uint16_t _partySize = 1);
};

#endif // SNIPER_H
