#ifndef RESOURCE_TYPES_H
#define RESOURCE_TYPES_H

#include <cstdint>
#include <map>

enum class ResourceType : std::uint8_t
{
    Unknown = 0,
    Gold,
    Wood,
    Stone,
    Gem
};

using ResourceMap = std::map<ResourceType,int>;


namespace Convert
{

inline std::string resourceToText(ResourceType _color)
{
    switch (_color)
    {
    case ResourceType::Gold  : return "Gold";
    case ResourceType::Gem   : return "Gem";
    case ResourceType::Stone : return "Stone";
    case ResourceType::Wood  : return "Wood";
    default:                   return "Unknown";
    }
}

inline ResourceType resourceFromText(const std::string& _name)
{
    if (_name == "Gold")
        return ResourceType::Gold;
    else if (_name == "Gem")
        return ResourceType::Gem;
    else if (_name == "Wood")
        return ResourceType::Wood;
    else if (_name == "Stone")
        return ResourceType::Stone;
    else
        return ResourceType::Unknown;
}

}

#endif // RESOURCE_TYPES_H
