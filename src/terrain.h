#ifndef TERRAIN_H
#define TERRAIN_H

#include <cstdint>

enum class TerrainType : std::uint8_t
{
    Unknown = 0,
    Grass,
    Dirt,
    Lava,
    Water,
    Snow,
    Mountains,
    MountainsSnow,
    Rock,
    RockSnow,
    Road
};

#endif // TERRAIN_H
