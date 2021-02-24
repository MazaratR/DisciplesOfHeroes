#ifndef RACES_H
#define RACES_H

#include <cstdint>
#include <string>

enum class Race : std::uint8_t
{
    Human = 0,
    Elf,
    Dwarf,
    Undead,
    Unknown
};

// Full name
constexpr const char* HUMAN_TEXT    = "Human";
constexpr const char* ELF_TEXT      = "Elf";
constexpr const char* DWARF_TEXT    = "Dwarf";
constexpr const char* UNDEAD_TEXT   = "Undead";
// Short name
constexpr const char* HUMAN_SHORT_TEXT    = "H";
constexpr const char* ELF_SHORT_TEXT      = "E";
constexpr const char* DWARF_SHORT_TEXT    = "D";
constexpr const char* UNDEAD_SHORT_TEXT   = "U";

namespace Convert
{
inline std::string raceToText(Race _race)
{
    switch (_race)
    {
    case Race::Human :  return HUMAN_TEXT;
    case Race::Elf :    return ELF_TEXT;
    case Race::Dwarf :  return DWARF_TEXT;
    case Race::Undead : return UNDEAD_TEXT;
    default:            return "Unknown";
    }
}

inline std::string raceToSymbol(Race _race)
{
    switch (_race)
    {
    case Race::Human :  return HUMAN_SHORT_TEXT;
    case Race::Elf :    return ELF_SHORT_TEXT;
    case Race::Dwarf :  return DWARF_SHORT_TEXT;
    case Race::Undead : return UNDEAD_SHORT_TEXT;
    default:            return "?";
    }
}

inline Race raceFromText(const std::string& _race)
{
    if (_race == HUMAN_TEXT)
        return Race::Human;
    if (_race == ELF_TEXT)
        return Race::Elf;
    if (_race == DWARF_TEXT)
        return Race::Dwarf;
    if (_race == UNDEAD_TEXT)
        return Race::Undead;
    else
        return Race::Unknown;
}

} // namespace Convert
#endif // RACES_H
