#ifndef EVENT_NOTIFY_H
#define EVENT_NOTIFY_H

#include "events/event_base.h"

class EventNotify
        : public EventBase
{
public:
    EventNotify(const std::string& _text)
        : EventBase(EventType::Notify)
        , m_text(_text)
    {}

public:
    std::string m_text;
};

#endif // EVENT_NOTIFY_H
