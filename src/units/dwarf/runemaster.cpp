#include "runemaster.h"

Runemaster::Runemaster(uint16_t _partySize)
    : Unit(UnitType::Runemaster, Race::Dwarf, UnitLevel::Level_4, _partySize)
{
    m_stats.armor = 10;
    m_stats.damageMelee.set(2,4);
    m_stats.damageRange.set(15,20);
    m_stats.ammo.set(15);
    m_stats.health.set(40);
    m_stats.speed = 7;

    m_cost[ResourceType::Gold] = 1000;
//    m_cost[ResourceType::Gem]  = 1;
}
