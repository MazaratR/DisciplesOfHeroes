#include "green_dragon.h"

GreenDragon::GreenDragon(uint16_t _partySize)
    : Unit(UnitType::GreenDragon, Race::Elf, UnitLevel::Level_4, _partySize)
{
    m_stats.armor = 9;
    m_stats.damageMelee.set(14,18);
    m_stats.health.set(120);
    m_stats.speed = 10;

    m_cost[ResourceType::Gold] = 1100;
//    m_cost[ResourceType::Gem] = 1;
}
