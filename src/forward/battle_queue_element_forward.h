#ifndef BATTLE_QUEUE_ELEMENT_FORWARD_H
#define BATTLE_QUEUE_ELEMENT_FORWARD_H

#include <vector>
#include <memory>

struct BattleQueueElement;
using BattleQueueElemShared = std::shared_ptr<BattleQueueElement>;
using BattleQueueSharedVec = std::vector<BattleQueueElemShared>;

#endif // BATTLE_QUEUE_ELEMENT_FORWARD_H
