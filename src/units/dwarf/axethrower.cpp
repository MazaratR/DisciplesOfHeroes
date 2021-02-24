#include "axethrower.h"

Axethrower::Axethrower(uint16_t _partySize)
    : Unit(UnitType::Axethrower, Race::Dwarf, UnitLevel::Level_2, _partySize)
{
    m_stats.armor = 8;
    m_stats.damageMelee.set(5,7);
    m_stats.damageRange.set(8,12);
    m_stats.ammo.set(20);
    m_stats.health.set(40);
    m_stats.speed = 5;


    m_cost[ResourceType::Gold] = 500;
}
