#ifndef UNIT_TYPES_H
#define UNIT_TYPES_H

#include <cstdint>
#include <string>

// Sorted by race and by unit level ascension
enum class UnitType : std::uint8_t
{
    Unknown = 0,
    // Human race
    Swordsman,
    Archer,
    Monk,
    Angel,
    // Elf race
    Wisp,
    Sniper,
    Pegas,
    GreenDragon,
    // Dwarf race
    Shieldman,
    Axethrower,
    Axemaster,
    Runemaster,
    // Undead race
    Skeleton,
    Vampire,
    Lich,
    BoneDragon
};

namespace Convert
{

inline std::string unitTypeToText(UnitType _unit)
{
    switch (_unit)
    {
    case UnitType::Swordsman    : return "Swordsman";
    case UnitType::Archer       : return "Archer";
    case UnitType::Monk         : return "Monk";
    case UnitType::Angel        : return "Angel";

    case UnitType::Wisp         : return "Wisp";
    case UnitType::Sniper       : return "Sniper";
    case UnitType::Pegas        : return "Pegas";
    case UnitType::GreenDragon  : return "GreenDragon";

    case UnitType::Shieldman    : return "Shieldman";
    case UnitType::Axethrower   : return "Axthrower";
    case UnitType::Axemaster    : return "Axemaster";
    case UnitType::Runemaster   : return "Runemaster";

    case UnitType::Skeleton     : return "Skeleton";
    case UnitType::Vampire      : return "Vampire";
    case UnitType::Lich         : return "Lich";
    case UnitType::BoneDragon   : return "BoneDragon";

    default:                   return "Unknown";
    }
}


}

#endif // UNIT_TYPES_H
