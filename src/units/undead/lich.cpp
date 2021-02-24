#include "lich.h"

Lich::Lich(uint16_t _partySize)
    : Unit(UnitType::Lich, Race::Undead, UnitLevel::Level_3, _partySize)
{
    m_stats.armor = 5;
    m_stats.damageMelee.set(5,9);
    m_stats.damageRange.set(10,15);
    m_stats.ammo.set(25);
    m_stats.health.set(40);
    m_stats.speed = 5;

    m_cost[ResourceType::Gold] = 700;
}
