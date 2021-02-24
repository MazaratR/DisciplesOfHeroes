#ifndef UNIT_LEVELS_H
#define UNIT_LEVELS_H

#include <cstdint>

enum class UnitLevel : std::uint8_t
{
    Unknown = 0,
    Level_1,
    Level_2,
    Level_3,
    Level_4
};

#endif // UNIT_LEVELS_H
