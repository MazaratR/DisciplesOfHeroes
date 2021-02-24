#ifndef EVENT_BATTLE_END_H
#define EVENT_BATTLE_END_H

#include "events/event_base.h"
#include "battle_result.h"

class EventBattleEnd
        : public EventBase
{
public:
    EventBattleEnd(const BattleResult& _result)
        : EventBase(EventType::BattleEnd)
        , m_result(_result)
    {}

public:
    BattleResult m_result;
};

#endif // EVENT_BATTLE_END_H
