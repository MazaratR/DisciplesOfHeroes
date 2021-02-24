#ifndef ATTACK_RESULT_H
#define ATTACK_RESULT_H

#include "forward/i_unit_forward.h"

struct AttackData
{
    IUnitShared unit;
    uint16_t damageTaken = 0;
    uint16_t healAmount  = 0;

};

struct AttackResult
{
    AttackData attacker;
    AttackData defender;
};

#endif // ATTACK_RESULT_H
