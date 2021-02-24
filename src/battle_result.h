#ifndef BATTLE_RESULT_H
#define BATTLE_RESULT_H

#include "interfaces/i_hero.h"
#include "forward/i_hero_forward.h"

struct Participant
{
    void clear(){
        hero.reset();
        losses.clear();
    }

    mutable IHeroShared hero;
    IUnitSharedVec losses;
};

struct BattleResult
{
    void clear()
    {
        armyOnLeft.clear();
        armyOnRight.clear();
    }

    Participant armyOnLeft;
    Participant armyOnRight;
};

#endif // BATTLE_RESULT_H
