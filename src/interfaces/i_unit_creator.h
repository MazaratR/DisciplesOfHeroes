#ifndef I_UNIT_CREATOR_H
#define I_UNIT_CREATOR_H

#include "forward/i_unit_forward.h"

class IUnitCreator
{
public:
    IUnitCreator() = default;
    virtual ~IUnitCreator() = default;

    virtual IUnitShared create(uint16_t _partySize) = 0;
};

#endif // I_UNIT_CREATOR_H
