#ifndef I_UNIT_FACTORY_H
#define I_UNIT_FACTORY_H

#include "races.h"
#include "units/unit_levels.h"
#include "units/unit_types.h"
#include "forward/i_unit_forward.h"

class IUnitFactory
{
public:
    IUnitFactory() = default;
    virtual ~IUnitFactory() = default;

    virtual IUnitShared createUnit(Race _race, UnitLevel _level, uint16_t _partySize = 1) = 0;
    virtual IUnitShared createUnit(UnitType _type, uint16_t _partySize = 1) = 0;
};

#endif // I_UNIT_FACTORY_H
