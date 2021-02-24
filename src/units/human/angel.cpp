#include "angel.h"

Angel::Angel(uint16_t _partySize)
    : Unit(UnitType::Angel, Race::Human, UnitLevel::Level_4, _partySize)
{
    m_stats.armor = 12;
    m_stats.damageMelee.set(12,16);
    m_stats.health.set(100);
    m_stats.speed = 10;

    m_cost[ResourceType::Gold] = 1000;
//    m_cost[ResourceType::Gem] = 1;
}
