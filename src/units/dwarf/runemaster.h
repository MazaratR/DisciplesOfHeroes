#ifndef RUNEMASTER_H
#define RUNEMASTER_H

#include "units/unit.h"

class Runemaster
        : public Unit
{
public:
    Runemaster(uint16_t _partySize = 1);
};

#endif // RUNEMASTER_H
