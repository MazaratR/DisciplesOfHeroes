#include "bone_dragon.h"

BoneDragon::BoneDragon(uint16_t _partySize)
    : Unit(UnitType::BoneDragon, Race::Undead, UnitLevel::Level_4, _partySize)
{
    m_stats.armor = 10;
    m_stats.damageMelee.set(15,20);
    m_stats.health.set(100);
    m_stats.speed = 8;

    m_cost[ResourceType::Gold] = 900;
//    m_cost[ResourceType::Gem] = 900;
}
