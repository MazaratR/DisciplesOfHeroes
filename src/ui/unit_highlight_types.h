#ifndef UNIT_HIGHLIGHT_TYPES_H
#define UNIT_HIGHLIGHT_TYPES_H

#include <cstdint>

enum class UnitHighlightType : std::uint8_t
{
    None = 0,
    Current,
    Damage,
    Heal
};

#endif // UNIT_HIGHLIGHT_TYPES_H
