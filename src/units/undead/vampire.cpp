#include "vampire.h"
#include "attack_result.h"

Vampire::Vampire(uint16_t _partySize)
    : Unit(UnitType::Vampire, Race::Undead, UnitLevel::Level_2, _partySize)
{
    m_stats.armor = 5;
    m_stats.damageMelee.set(7,12);
    m_stats.health.set(30);
    m_stats.speed = 7;

    m_cost[ResourceType::Gold] = 400;
}

AttackResult Vampire::attack(IUnitShared &_enemyUnit)
{
    // Feed on enemy
    AttackResult result = Unit::attack(_enemyUnit);

    if (result.defender.damageTaken)
    {
        uint16_t healAmount = result.defender.damageTaken * (m_healPercentOfDamage / 100.);
        result.attacker.healAmount = Unit::heal(healAmount);
    }

    return result;
}
