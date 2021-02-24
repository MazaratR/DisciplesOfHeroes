#ifndef I_UNIT_H
#define I_UNIT_H

#include <memory>

#include <string>
#include "races.h"
#include "resource_types.h"

#include "units/unit_levels.h"
#include "units/unit_stats.h"
#include "units/unit_types.h"

#include "forward/i_unit_forward.h"

struct UnitStats;
struct AttackResult;

class IUnit
{
public:
    IUnit() = default;
    virtual ~IUnit() = default;

    virtual std::string name() const = 0;
    virtual Race race() const = 0;
    virtual UnitLevel level() const = 0;
    virtual UnitType type() const = 0;
    virtual uint16_t partySize() const = 0;
    virtual void acceptUnitInParty(uint16_t _size) = 0;
    virtual void reducePartySizeBy(uint16_t _amount) = 0;

    virtual bool coveredFromTheFront() const = 0;
    virtual void setCoveredFromTheFront(bool _covered) = 0;

    virtual bool canAttack(IUnitShared& _enemyUnit) const = 0;
    virtual bool canShoot() const = 0;

    virtual AttackResult attack(IUnitShared& _enemyUnit) = 0;
    virtual uint16_t onGetDamage(uint16_t _damage) = 0;
    virtual uint16_t heal(uint16_t _healAmount) = 0;

    virtual const UnitStats& stats() const = 0;
    virtual uint16_t cost(ResourceType _type) const = 0;
};

#endif // I_UNIT_H
