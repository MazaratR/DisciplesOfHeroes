#include "sniper.h"

Sniper::Sniper(uint16_t _partySize)
    : Unit(UnitType::Sniper, Race::Elf, UnitLevel::Level_2, _partySize)
{
    m_stats.armor = 5;
    m_stats.damageMelee.set(5,9);
    m_stats.damageRange.set(12,16);
    m_stats.ammo.set(25);
    m_stats.health.set(50);
    m_stats.speed = 7;

    m_cost[ResourceType::Gold] = 500;
}
