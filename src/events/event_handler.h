#ifndef EVENT_HANDLER_H
#define EVENT_HANDLER_H

#include <memory>
#include <queue>

#include "interfaces/i_event_handler.h"

class IGameLogic;
class IUserInterface;
class EventBase;

class EventHandler
        : public IEventHandler
{
public:
    EventHandler() = default;

    void acceptEvent(std::shared_ptr<EventBase>&& _event) override;
    void eventEnds() override;
    void processEventQueue();
    void setGameLogic(std::shared_ptr<IGameLogic> _logic)        { m_gameLogic = _logic; }
    void setUseInterface(std::shared_ptr<IUserInterface> _logic) { m_userInterface = _logic; }

private:
    void arrivalAtTown();
    void battleBegin();
    void battleEnd();
    void newWeek();
    void notify();

public:
    std::shared_ptr<IGameLogic> m_gameLogic;
    std::shared_ptr<IUserInterface> m_userInterface;
    std::queue<std::shared_ptr<EventBase>> m_eventsQueue;
    std::shared_ptr<EventBase> m_currentEvent;
};

#endif // EVENT_HANDLER_H
