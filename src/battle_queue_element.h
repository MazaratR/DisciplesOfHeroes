#ifndef BATTLEQUEUEELEMENT_H
#define BATTLEQUEUEELEMENT_H

#include "forward/i_unit_forward.h"

struct BattleQueueElement
{
    BattleQueueElement() = default;
    BattleQueueElement(IUnitWeak _unit)
        : unit(_unit)
    {}

    bool isDead() const;

    IUnitWeak unit;
    bool actedOnThisTurn = false;
    bool fromLeftArmy = false;
};


#endif // BATTLEQUEUEELEMENT_H
