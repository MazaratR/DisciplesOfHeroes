#ifndef EVENT_TYPES_H
#define EVENT_TYPES_H

#include <cstdint>

enum class EventType : std::uint8_t
{
    Unknown = 0,
    EventEnd,
    ArrivalAtTown,
    Battle,
    BattleEnd,
    NewWeek,
    Notify,
    HeroExchange
};

#endif // EVENT_TYPES_H
