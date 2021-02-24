#include "event_manager.h"
#include "interfaces/i_event_handler.h"

void EventManager::addEventToQueue(std::shared_ptr<EventBase>&& _event)
{
    if (!m_handler)
        return;

    m_handler->acceptEvent(std::move(_event));
}

void EventManager::currentEventEnds()
{
    if (!m_handler)
        return;

    m_handler->eventEnds();
}
