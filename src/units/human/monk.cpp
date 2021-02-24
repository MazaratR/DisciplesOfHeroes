#include "monk.h"

Monk::Monk(uint16_t _partySize)
    : Unit(UnitType::Monk, Race::Human, UnitLevel::Level_3, _partySize)
{
    m_stats.armor = 2;
    m_stats.damageMelee.set(2,5);
    m_stats.damageRange.set(12,14);
    m_stats.ammo.set(20);
    m_stats.health.set(45);
    m_stats.speed = 6;

    m_cost[ResourceType::Gold] = 700;
}
