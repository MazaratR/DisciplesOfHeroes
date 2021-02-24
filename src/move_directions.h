#ifndef MOVE_DIRECTIONS_H
#define MOVE_DIRECTIONS_H

#include <cstdint>

enum class MoveDirection : std::uint8_t
{
    Unknown = 0,
    Up,
    Down,
    Left,
    Right
};

#endif // MOVE_DIRECTIONS_H
