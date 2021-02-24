#include "axemaster.h"

Axemaster::Axemaster(uint16_t _partySize)
    : Unit(UnitType::Axemaster, Race::Dwarf, UnitLevel::Level_3, _partySize)
{
    m_stats.armor = 10;
    m_stats.damageMelee.set(9,12);
    m_stats.health.set(70);
    m_stats.speed = 6;

    m_cost[ResourceType::Gold] = 700;
}
