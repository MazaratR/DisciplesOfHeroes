#ifndef ARCHER_H
#define ARCHER_H

#include "units/unit.h"

class Archer
        : public Unit
{
public:
    Archer(uint16_t _partySize = 1);
};

#endif // ARCHER_H
