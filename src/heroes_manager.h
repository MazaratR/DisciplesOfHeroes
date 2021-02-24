#ifndef HEROES_MANAGER_H
#define HEROES_MANAGER_H

#include <map>
#include <queue>

#include "races.h"

#include "interfaces/i_heroes_manager.h"

#include "forward/i_hero_forward.h"

class HeroesManager
        : public IHeroesManager
{
public:
    HeroesManager();

    IHeroShared getNextAvailableHero(Race _race) override;
    IHeroShared getHero(const std::string& _name) override;
    IHeroSharedVec getHeroes(Race _race) const override;
    int getAvailableHeroCount(Race _race) const override;

private:
    void initHeroes();

private:
    std::map<Race, IHeroSharedVec> m_heroesPool;
};

#endif // HEROES_MANAGER_H
