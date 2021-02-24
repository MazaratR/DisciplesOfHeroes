#ifndef UNITFACTORY_H
#define UNITFACTORY_H

#include <map>

#include "races.h"
#include "units/unit_levels.h"
#include "units/unit_types.h"

#include "interfaces/i_unit_factory.h"
#include "interfaces/i_unit_creator.h"

#include "forward/i_unit_forward.h"


class UnitFactory
        : public IUnitFactory
{
    using IUnitCreatorShared = std::shared_ptr<IUnitCreator>;
    using UnitCreatorMap = std::map<UnitType,IUnitCreatorShared>;
    using RaceAndLevelToTypeMap = std::map<std::pair<Race, UnitLevel>,UnitType>;

public:
    UnitFactory();

    IUnitShared createUnit(Race _race, UnitLevel _level, uint16_t _partySize = 1) override;
    IUnitShared createUnit(UnitType _type, uint16_t _partySize = 1) override;

private:
    void initCreatorMap();
    void initConvertationMap();

private:
    UnitCreatorMap m_unitCreatorMap;
    RaceAndLevelToTypeMap m_raceAndLevelToTypeMap;
};

#endif // UNITFACTORY_H
