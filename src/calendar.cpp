#include "calendar.h"

#include "events/event_manager.h"
#include "events/event_base.h"

uint8_t Calendar::getDay() const
{
    return m_day;
}

uint8_t Calendar::getWeek() const
{
    return m_week;
}

uint8_t Calendar::getMonth() const
{
    return m_month;
}

void Calendar::onEndOfDay()
{
    m_day++;
    if (m_day > DAYS_IN_WEEK)
    {
        m_day = 1;
        onEndOfWeek();
    }
}

void Calendar::onEndOfWeek()
{
    m_week++;
    if (m_week > WEEKS_IN_MONTH)
    {
        m_week = 1;
        onEndOfMonth();
    }

    EventManager::getInstance().addEventToQueue(std::make_shared<EventBase>(EventType::NewWeek));
}

void Calendar::onEndOfMonth()
{
    m_month++;
    if (m_month > MONTHS_IN_YEAR)
        m_month = 1;
}
