#include "battle_queue_element.h"
#include "interfaces/i_unit.h"

bool BattleQueueElement::isDead() const
{
    if (auto unitShared = unit.lock())
        return unitShared->partySize() < 1;

    return true;
}
