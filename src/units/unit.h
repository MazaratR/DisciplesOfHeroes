#ifndef UNIT_H
#define UNIT_H

#include "resource_types.h"
#include "interfaces/i_unit.h"
#include "forward/i_unit_forward.h"

class Unit
        : public IUnit
{
public:
    Unit(UnitType _type,
         Race _race,
         UnitLevel _level,
         uint16_t _partySize);

    std::string name()   const final { return Convert::unitTypeToText(m_type); }
    Race race()          const final { return m_race; }
    UnitLevel level()    const final { return m_level; }
    UnitType type()      const final { return m_type; }
    uint16_t partySize() const final { return m_partySize; }
    void acceptUnitInParty(uint16_t _size) override;
    void reducePartySizeBy(uint16_t _amount) override;

    bool coveredFromTheFront() const final { return m_coveredFromTheFront; }
    void setCoveredFromTheFront(bool _covered) final { m_coveredFromTheFront = _covered; };

    bool canAttack(IUnitShared& _enemyUnit) const final;
    bool canShoot() const final { return m_stats.ammo.current > 0; };

    AttackResult attack(IUnitShared &_enemyUnit) override;
    uint16_t onGetDamage(uint16_t _damage) override;
    uint16_t heal(uint16_t _healAmount) override;

    const UnitStats& stats() const override { return m_stats; }
    uint16_t cost(ResourceType _type) const override;

protected:
    UnitType m_type = UnitType::Unknown;
    Race m_race = Race::Unknown;
    UnitLevel m_level = UnitLevel::Unknown;
    UnitStats m_stats;
    uint16_t m_partySize = 0;
    ResourceMap m_cost;
    bool m_coveredFromTheFront = false;
};

#endif // UNIT_H
