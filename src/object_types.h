#ifndef OBJECT_TYPES_H
#define OBJECT_TYPES_H

#include <cstdint>

enum class ObjectType : std::uint8_t
{
    Unknown = 0,
    Hero,
    Resource,
    Town,
    Mine
};

#endif // OBJECT_TYPES_H
