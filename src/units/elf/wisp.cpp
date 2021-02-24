#include "wisp.h"

Wisp::Wisp(uint16_t _partySize)
    : Unit(UnitType::Wisp, Race::Elf, UnitLevel::Level_1, _partySize)

{
    m_stats.armor = 2;
    m_stats.damageMelee.set(2,4);
    m_stats.damageRange.set(6,8);
    m_stats.ammo.set(40);
    m_stats.health.set(20);
    m_stats.speed = 5;

    m_cost[ResourceType::Gold] = 300;
}
