#ifndef EVENT_BATTLE_H
#define EVENT_BATTLE_H

#include "events/event_base.h"

#include "forward/i_hero_forward.h"

class EventBattle
        : public EventBase
{
public:
    EventBattle(IHeroShared _attacker,
                IHeroShared _defender,
                EventEndFunc _onEventEnd)
        : EventBase(EventType::Battle, _onEventEnd)
        , m_defender(_defender)
        , m_attacker (_attacker)
    {}

public:
    IHeroShared m_defender;
    IHeroShared m_attacker;
};

#endif // EVENT_BATTLE_H
