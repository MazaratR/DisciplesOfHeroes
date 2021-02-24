#ifndef UNIT_POSITION_H
#define UNIT_POSITION_H

#include <cstdint>

/// \warning Must corresponds to Hero::ARMY_SIZE
enum class UnitPosition : std::uint8_t
{
    RearguardTop    = 0,
    RearguardBottom = 1,
    VanguardTop     = 2,
    VanguardBottom  = 3,
    Unknown
};

#endif // UNIT_POSITION_H
