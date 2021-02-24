#include "unit_factory.h"

#include "unit_creator.h"

#include "units/human/swordsman.h"
#include "units/human/archer.h"
#include "units/human/monk.h"
#include "units/human/angel.h"

#include "units/elf/wisp.h"
#include "units/elf/sniper.h"
#include "units/elf/pegas.h"
#include "units/elf/green_dragon.h"

#include "units/dwarf/shieldman.h"
#include "units/dwarf/axethrower.h"
#include "units/dwarf/axemaster.h"
#include "units/dwarf/runemaster.h"

#include "units/undead/skeleton.h"
#include "units/undead/vampire.h"
#include "units/undead/lich.h"
#include "units/undead/bone_dragon.h"

UnitFactory::UnitFactory()
{
    initCreatorMap();
    initConvertationMap();
}

IUnitShared UnitFactory::createUnit(Race _race, UnitLevel _level, uint16_t _partySize)
{
    auto key = std::make_pair(_race, _level);
    auto it = m_raceAndLevelToTypeMap.find(key);
    if (it == m_raceAndLevelToTypeMap.end())
        return IUnitShared();

    auto type = it->second;
    return createUnit(type, _partySize);
}

IUnitShared UnitFactory::createUnit(UnitType _type, uint16_t _partySize)
{
    auto it = m_unitCreatorMap.find(_type);
    if (it == m_unitCreatorMap.end())
        return IUnitShared();

    const auto& creator = it->second;
    return creator->create(_partySize);
}

void UnitFactory::initCreatorMap()
{
    m_unitCreatorMap = {
        {UnitType::Swordsman,   std::make_shared<UnitCreator<Swordsman>>()},
        {UnitType::Archer,      std::make_shared<UnitCreator<Archer>>()},
        {UnitType::Monk,        std::make_shared<UnitCreator<Monk>>()},
        {UnitType::Angel,       std::make_shared<UnitCreator<Angel>>()},

        {UnitType::Wisp,        std::make_shared<UnitCreator<Wisp>>()},
        {UnitType::Sniper,      std::make_shared<UnitCreator<Sniper>>()},
        {UnitType::Pegas,       std::make_shared<UnitCreator<Pegas>>()},
        {UnitType::GreenDragon, std::make_shared<UnitCreator<GreenDragon>>()},

        {UnitType::Shieldman,   std::make_shared<UnitCreator<Shieldman>>()},
        {UnitType::Axethrower,  std::make_shared<UnitCreator<Axethrower>>()},
        {UnitType::Axemaster,   std::make_shared<UnitCreator<Axemaster>>()},
        {UnitType::Runemaster,  std::make_shared<UnitCreator<Runemaster>>()},

        {UnitType::Skeleton,    std::make_shared<UnitCreator<Skeleton>>()},
        {UnitType::Vampire,     std::make_shared<UnitCreator<Vampire>>()},
        {UnitType::Lich,        std::make_shared<UnitCreator<Lich>>()},
        {UnitType::BoneDragon,  std::make_shared<UnitCreator<BoneDragon>>()}
    };
}

void UnitFactory::initConvertationMap()
{
    auto setConvertation = [this](UnitType _type)
    {
        auto unit = createUnit(_type);
        m_raceAndLevelToTypeMap[std::make_pair(unit->race(), unit->level())] = unit->type();
    };

    setConvertation(UnitType::Archer);
    setConvertation(UnitType::Swordsman);
    setConvertation(UnitType::Monk);
    setConvertation(UnitType::Angel);

    setConvertation(UnitType::Wisp);
    setConvertation(UnitType::Sniper);
    setConvertation(UnitType::Pegas);
    setConvertation(UnitType::GreenDragon);

    setConvertation(UnitType::Shieldman);
    setConvertation(UnitType::Axethrower);
    setConvertation(UnitType::Axemaster);
    setConvertation(UnitType::Runemaster);

    setConvertation(UnitType::Skeleton);
    setConvertation(UnitType::Vampire);
    setConvertation(UnitType::Lich);
    setConvertation(UnitType::BoneDragon);
}
