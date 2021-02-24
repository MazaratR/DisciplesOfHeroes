#ifndef I_BATTLE_MANAGER_H
#define I_BATTLE_MANAGER_H

#include "units/unit_position.h"

#include "forward/i_hero_forward.h"
#include "forward/battle_queue_element_forward.h"

class BattleResult;
struct AttackResult;

class IBattleManager
{
public:
    IBattleManager() = default;
    virtual ~IBattleManager() = default;

    virtual void setPartisipants(IHeroShared& _heroOnLeft, IHeroShared& _heroOnRight) = 0;
    virtual void surrender() = 0;
    virtual void skipMove() = 0;
    virtual AttackResult processAttack(UnitPosition _defenderPos, bool _attackedUnitOnLeft) = 0;
    virtual const BattleQueueSharedVec& getBattleQueueVec() const = 0;

};

#endif // I_BATTLE_MANAGER_H
