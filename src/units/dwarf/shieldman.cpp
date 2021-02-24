#include "shieldman.h"

Shieldman::Shieldman(uint16_t _partySize)
    : Unit(UnitType::Shieldman, Race::Dwarf, UnitLevel::Level_1, _partySize)
{
    m_stats.armor = 15;
    m_stats.damageMelee.set(4,7);
    m_stats.health.set(30);
    m_stats.speed = 4;

    m_cost[ResourceType::Gold] = 200;
}
