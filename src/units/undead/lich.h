#ifndef LICH_H
#define LICH_H

#include "units/unit.h"

class Lich
        : public Unit
{
public:
    Lich(uint16_t _partySize = 1);
};

#endif // LICH_H
