#include "skeleton.h"

Skeleton::Skeleton(uint16_t _partySize)
    : Unit(UnitType::Skeleton, Race::Undead, UnitLevel::Level_1, _partySize)
{
    m_stats.armor = 1;
    m_stats.damageMelee.set(3,6);
    m_stats.health.set(10);
    m_stats.speed = 4;

    m_cost[ResourceType::Gold] = 200;
}
