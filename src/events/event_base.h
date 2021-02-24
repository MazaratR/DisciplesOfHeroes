#ifndef EVENT_BASE_H
#define EVENT_BASE_H

#include <functional>
#include "events/event_types.h"

class EventBase
{
protected:
    using EventEndFunc = std::function<void()>;

public:
    EventBase(EventType _type,
              EventEndFunc _onEventEnd = EventEndFunc())
        : m_type(_type)
        , m_onEventEnd(_onEventEnd)
    {}
    virtual ~EventBase() = default;

    EventType type() const { return m_type; }
    void onEventEnd() const {
        if (m_onEventEnd)
            m_onEventEnd();
    }

protected:
    EventType m_type;
    EventEndFunc m_onEventEnd; // Func to call in the end of event
};

#endif // EVENT_BASE_H
