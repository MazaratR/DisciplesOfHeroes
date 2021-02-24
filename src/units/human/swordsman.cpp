#include "swordsman.h"

Swordsman::Swordsman(uint16_t _partySize)
    : Unit(UnitType::Swordsman, Race::Human, UnitLevel::Level_1, _partySize)
{
    m_stats.armor = 7;
    m_stats.damageMelee.set(5,8);
    m_stats.health.set(50);
    m_stats.speed = 7;

    m_cost[ResourceType::Gold] = 300;
}
