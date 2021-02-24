#ifndef EVENT_MANAGER_H
#define EVENT_MANAGER_H

#include <memory>

class IEventHandler;
class EventBase;

class EventManager
{
public:
    static EventManager& getInstance(){
        static EventManager instance;
        return instance;
    }

    void setHandler(std::shared_ptr<IEventHandler> handler){
        m_handler = handler;
    }

    void addEventToQueue(std::shared_ptr<EventBase> &&_event);
    void currentEventEnds();

private:
    EventManager() = default;

private:
    std::shared_ptr<IEventHandler> m_handler;
};

#endif // EVENT_MANAGER_H
