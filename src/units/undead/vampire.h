#ifndef VAMPIRE_H
#define VAMPIRE_H

#include "units/unit.h"

class Vampire
        : public Unit
{
public:
    Vampire(uint16_t _partySize = 1);

    AttackResult attack(IUnitShared &_enemyUnit) override;

private:
    // How much of dealt damage will be taken as self heal
    float m_healPercentOfDamage = 10.0;
};

#endif // VAMPIRE_H
