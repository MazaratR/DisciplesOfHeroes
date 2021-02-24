#include "unit.h"
#include "randomizer.h"
#include "attack_result.h"

Unit::Unit(UnitType _type, Race _race, UnitLevel _level, uint16_t _partySize)
    : m_type(_type)
    , m_race(_race)
    , m_level(_level)
    , m_partySize(_partySize)
{

}

void Unit::acceptUnitInParty(uint16_t _size)
{
    m_partySize += _size;
}

void Unit::reducePartySizeBy(uint16_t _amount)
{
    m_partySize = _amount > m_partySize ? 0
                                        : m_partySize - _amount;
}

bool Unit::canAttack(IUnitShared &_enemyUnit) const
{
    if (canShoot())
        return true;
    else
        return !m_coveredFromTheFront && !_enemyUnit->coveredFromTheFront();
}

AttackResult Unit::attack(IUnitShared& _enemyUnit)
{
    AttackResult result;

    if (!_enemyUnit)
        return result;

    uint16_t minDamage = m_partySize;
    uint16_t maxDamage = m_partySize;
    if (canShoot())
    {
        // Range attack
        minDamage *= m_stats.damageRange.min;
        maxDamage *= m_stats.damageRange.max;
        m_stats.ammo.current--;
    }
    else
    {
        // Melee attack
        minDamage *= m_stats.damageMelee.min;
        maxDamage *= m_stats.damageMelee.max;
    }

    uint16_t damageAmout = Randomizer::getRandomIntValue<uint16_t>(minDamage, maxDamage);

    result.defender.damageTaken = _enemyUnit->onGetDamage(damageAmout);

    return result;
}

uint16_t Unit::onGetDamage(uint16_t _damage)
{
    if (m_partySize < 1)
        return 0;

    uint16_t damageReduce = m_stats.armor;
    if (damageReduce >= _damage)
        _damage = 1;
    else
        _damage -= damageReduce;

    uint16_t totalHealth = m_stats.health.current + m_stats.health.max * (m_partySize - 1);
    if (totalHealth <= _damage)
    {
        m_stats.health.current = 0;
        m_partySize = 0;
    }
    else
    {
        totalHealth -= _damage;

        if (totalHealth > m_stats.health.max)
        {
            m_stats.health.current = totalHealth % m_stats.health.max;
            m_partySize = std::ceil(static_cast<float>(totalHealth) /
                                    static_cast<float>(m_stats.health.max));
        }
        else
        {
            m_stats.health.current = totalHealth;
            m_partySize = 1;
        }
    }

    return _damage;
}

uint16_t Unit::heal(uint16_t _healAmount)
{
    /// \todo Not allow to heal more then party size at the begining  of battle
    uint16_t prevTotalHealth = m_stats.health.current + m_stats.health.max * (m_partySize - 1);

    uint16_t  newTotalHealth = prevTotalHealth + _healAmount;
    m_stats.health.current = newTotalHealth % m_stats.health.max;
    m_partySize = std::ceil(static_cast<float>(newTotalHealth) /
                            static_cast<float>(m_stats.health.max));

    return newTotalHealth - prevTotalHealth;
}

uint16_t Unit::cost(ResourceType _type) const
{
    const auto& it = m_cost.find(_type);
    if (it == m_cost.end())
        return 0;
    else
        return it->second;
}
