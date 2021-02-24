#ifndef I_HERO_GENERATOR_H
#define I_HERO_MANAGER_H

#include <memory>
#include <queue>

#include "forward/i_hero_forward.h"
enum class Race : std::uint8_t;

class IHeroesManager
{
public:
    IHeroesManager() = default;
    virtual ~IHeroesManager() = default;

    virtual IHeroShared getNextAvailableHero(Race _race) = 0;
    virtual IHeroShared getHero(const std::string& _name) = 0;
    virtual IHeroSharedVec getHeroes(Race _race) const = 0;
    virtual int getAvailableHeroCount(Race _race) const = 0;
};

#endif // I_HERO_GENERATOR_H
