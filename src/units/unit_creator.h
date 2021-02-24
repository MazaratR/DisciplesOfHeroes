#ifndef UNIT_CREATOR_H
#define UNIT_CREATOR_H

#include "interfaces/i_unit_creator.h"

template <class C>
class UnitCreator
        : public IUnitCreator
{
public:
    UnitCreator() = default;
    ~UnitCreator() = default;

    IUnitShared create(uint16_t _partySize) override
    {
        return std::make_shared<C>(_partySize);
    }
};

#endif // UNIT_CREATOR_H
