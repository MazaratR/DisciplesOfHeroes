#ifndef EVENT_ARRIVAL_AT_TOWN_H
#define EVENT_ARRIVAL_AT_TOWN_H

#include "events/event_base.h"

#include "forward/i_hero_forward.h"
#include "forward/i_town_forward.h"

class ITown;

class EventArrivalAtTown
        : public EventBase
{
public:
    EventArrivalAtTown(ITownShared& _town,
                       IHeroShared& _visitor)
        : EventBase(EventType::ArrivalAtTown)
        , m_town(_town)
        , m_visitor(_visitor)
    {}

public:
    ITownShared m_town;
    IHeroShared m_visitor;
};


#endif // EVENT_ARRIVAL_AT_TOWN_H
