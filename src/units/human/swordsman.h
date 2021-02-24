#ifndef SWORDSMAN_H
#define SWORDSMAN_H

#include "units/unit.h"

class Swordsman
        : public Unit
{
public:
    Swordsman(uint16_t _partySize = 1);
};

#endif // SWORDSMAN_H
