#ifndef CALENDAR_H
#define CALENDAR_H

#include <cstdint>

class Calendar
{
public:
    static Calendar& getInstance(){
        static Calendar instance;
        return instance;
    }

    uint8_t getDay() const;
    uint8_t getWeek() const;
    uint8_t getMonth() const;

    void onEndOfDay();

private:
    Calendar() = default;

    void onEndOfWeek();
    void onEndOfMonth();

public:
    static constexpr uint8_t DAYS_IN_WEEK = 7;
    static constexpr uint8_t WEEKS_IN_MONTH = 4;
    static constexpr uint8_t MONTHS_IN_YEAR = 12;

private:
    uint8_t m_day = 0;
    uint8_t m_week = 1;
    uint8_t m_month = 1;
};

#endif // CALENDAR_H
