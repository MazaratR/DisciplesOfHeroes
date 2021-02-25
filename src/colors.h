#ifndef COLORS_H
#define COLORS_H

#include <cstdint>
#include <string>
#include <sstream>

enum class Color : std::uint8_t
{
    White = 0,
    Blue,
    Green,
    Yellow,
    Black
};

constexpr const int32_t COLOR_WHITE     = 0xe6e6e6;
constexpr const int32_t COLOR_BLUE      = 0x2f2cdb;
constexpr const int32_t COLOR_GREEN     = 0x2cdb38;
constexpr const int32_t COLOR_YELLOW    = 0xc7db2c;
constexpr const int32_t COLOR_BLACK     = 0x000000;

constexpr const char* COLOR_WHITE_TEXT  = "Pure White";
constexpr const char* COLOR_BLUE_TEXT   = "Noble Blue";
constexpr const char* COLOR_GREEN_TEXT  = "Lifefull Green";
constexpr const char* COLOR_YELLOW_TEXT = "Fearless Yellow";
constexpr const char* COLOR_BLACK_TEXT  = "Grim Black";

namespace Convert
{

inline std::string colorToText(Color _color)
{
    switch (_color)
    {
    case Color::White :     return COLOR_WHITE_TEXT;
    case Color::Blue :      return COLOR_BLUE_TEXT;
    case Color::Green :     return COLOR_GREEN_TEXT;
    case Color::Yellow :    return COLOR_YELLOW_TEXT;
    case Color::Black :     return COLOR_BLACK_TEXT;
    default:                return "Unknown";
    }
}

inline Color colorFromText(const std::string& _color)
{
    if (_color == COLOR_WHITE_TEXT)
        return Color::White;
    if (_color == COLOR_BLUE_TEXT)
        return Color::Blue;
    if (_color == COLOR_GREEN_TEXT)
        return Color::Green;
    if (_color == COLOR_YELLOW_TEXT)
        return Color::Yellow;
    else
        return Color::Black;

}

inline int32_t colorToHexValue(Color _color)
{
    switch (_color)
    {
    case Color::White:  return COLOR_WHITE;
    case Color::Blue:   return COLOR_BLUE;
    case Color::Green:  return COLOR_GREEN;
    case Color::Yellow: return COLOR_YELLOW;
    case Color::Black:  return COLOR_BLACK;
    default: return 0;
    }
}

inline std::string colorToHexStr(Color _color)
{
    std::stringstream ss;
    ss << std::hex << colorToHexValue(_color);
    return ss.str();
}

inline std::string colorTextToHexStr(const std::string& _color)
{
    return colorToHexStr(colorFromText(_color));
}

} // namespace Convert

#endif // COLORS_H
