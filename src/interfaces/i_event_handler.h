#ifndef I_EVENT_HANDLER_H
#define I_EVENT_HANDLER_H

#include <memory>

class EventBase;

class IEventHandler
{
public:
    IEventHandler() = default;
    virtual ~IEventHandler() = default;

    virtual void acceptEvent(std::shared_ptr<EventBase>&& _event) = 0;
    virtual void eventEnds() = 0;
};

#endif // I_EVENT_HANDLER_H
