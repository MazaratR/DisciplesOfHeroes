#ifndef MONK_H
#define MONK_H

#include "units/unit.h"

class Monk
        : public Unit
{
public:
    Monk(uint16_t _partySize = 1);
};

#endif // MONK_H
