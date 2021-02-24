#include "archer.h"

Archer::Archer(uint16_t _partySize)
    : Unit(UnitType::Archer, Race::Human, UnitLevel::Level_2, _partySize)
{
    m_stats.armor = 3;
    m_stats.damageMelee.set(2,4);
    m_stats.damageRange.set(6,9);
    m_stats.ammo.set(25);
    m_stats.health.set(30);
    m_stats.speed = 5;

    m_cost[ResourceType::Gold] = 400;
}
