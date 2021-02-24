#include "pegas.h"

Pegas::Pegas(uint16_t _partySize)
    : Unit(UnitType::Pegas, Race::Elf, UnitLevel::Level_3, _partySize)
{
    m_stats.armor = 7;
    m_stats.damageMelee.set(9,12);
    m_stats.health.set(60);
    m_stats.speed = 9;

    m_cost[ResourceType::Gold] = 700;
}
