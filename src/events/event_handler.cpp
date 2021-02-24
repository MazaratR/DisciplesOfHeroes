#include "event_handler.h"

#include "events/event_base.h"
#include "events/event_battle.h"
#include "events/event_arrival_at_town.h"
#include "events/event_battle_end.h"
#include "events/event_notify.h"

#include "interfaces/i_game_logic.h"
#include "interfaces/i_user_interface.h"

void EventHandler::acceptEvent(std::shared_ptr<EventBase>&& _event)
{
    m_eventsQueue.emplace(std::move(_event));
}

void EventHandler::eventEnds()
{
    if (!m_currentEvent)
        return;

    m_currentEvent->onEventEnd();
    m_currentEvent.reset();
}

void EventHandler::processEventQueue()
{
    if (m_eventsQueue.empty() || m_currentEvent)
        return;

    while (!m_currentEvent)
    {
        if (m_eventsQueue.empty())
            return;

        m_currentEvent = m_eventsQueue.front();
        m_eventsQueue.pop();
    }

    switch (m_currentEvent->type())
    {
    case EventType::ArrivalAtTown:  arrivalAtTown(); break;
    case EventType::Battle:         battleBegin(); break;
    case EventType::BattleEnd:      battleEnd(); break;
    case EventType::NewWeek:        newWeek(); break;
    case EventType::HeroExchange:   break;
    case EventType::Notify:         notify(); break;
    default: break;
    }
}

void EventHandler::arrivalAtTown()
{
    if (!m_currentEvent)
        return;

    auto arrivalEvent = std::dynamic_pointer_cast<EventArrivalAtTown>(m_currentEvent);
    if (!arrivalEvent)
    {
        // Error
        m_currentEvent.reset();
        return;
    }

    m_userInterface->arrivalAtTown(arrivalEvent->m_town, arrivalEvent->m_visitor);
    m_currentEvent.reset();
}

void EventHandler::battleBegin()
{
    if (!m_currentEvent)
        return;

    auto battleEvent = std::dynamic_pointer_cast<EventBattle>(m_currentEvent);
    if (!battleEvent)
    {
        // Error
        m_currentEvent.reset();
        return;
    }

    m_gameLogic->battleBegin(battleEvent->m_attacker, battleEvent->m_defender);
}

void EventHandler::battleEnd()
{
    if (!m_currentEvent)
        return;

    auto battleEvent = std::dynamic_pointer_cast<EventBattleEnd>(m_currentEvent);
    if (!battleEvent)
    {
        // Error
        m_currentEvent.reset();
        return;
    }

    m_gameLogic->battleEnd(battleEvent->m_result);
    m_userInterface->battleEnd(battleEvent->m_result);
//    m_currentEvent.reset();
}

void EventHandler::newWeek()
{
    if (!m_currentEvent)
        return;

    m_gameLogic->onNewWeek();
    m_userInterface->onNewWeek();
}

void EventHandler::notify()
{
    if (!m_currentEvent)
        return;

    auto notifyEvent = std::dynamic_pointer_cast<EventNotify>(m_currentEvent);
    if (!notifyEvent)
    {
        // Error
        m_currentEvent.reset();
        return;
    }

    m_userInterface->onNotify(notifyEvent->m_text);
}


