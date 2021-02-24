#include "heroes_manager.h"
#include "hero.h"

HeroesManager::HeroesManager()
{
    initHeroes();
}

IHeroShared HeroesManager::getNextAvailableHero(Race _race)
{
    const auto& heroesVec = m_heroesPool[_race];

    for (const auto& hero : heroesVec)
    {
        if (hero->controllingPlayer())
            continue;

        return hero;
    }

    return nullptr;
}

IHeroShared HeroesManager::getHero(const std::string &_name)
{
    IHeroShared retvalHero;

    for (const auto& it : m_heroesPool)
    {
        const auto& heroesVec = it.second;
        for (const auto& hero : heroesVec)
        {
            if (hero->name() == _name)
            {
                retvalHero = hero;
                break;
            }
        }
    }

    return retvalHero;
}

IHeroSharedVec HeroesManager::getHeroes(Race _race) const
{
    const auto& it = m_heroesPool.find(_race);
    if (it == m_heroesPool.end())
        return IHeroSharedVec();

    return it->second;
}

int HeroesManager::getAvailableHeroCount(Race _race) const
{
    const auto& it = m_heroesPool.find(_race);
    if (it == m_heroesPool.end())
        return 0;

    const auto& heroesVec = it->second;
    return heroesVec.size();
}

void HeroesManager::initHeroes()
{
    m_heroesPool = {
        {Race::Human,  {std::make_shared<Hero>("Arthur",      Race::Human,    HeroParameters(3,100)),
                        std::make_shared<Hero>("Cathrine",    Race::Human,    HeroParameters(3,100))}},
        {Race::Elf,    {std::make_shared<Hero>("Gildor",      Race::Elf,      HeroParameters(4,120)),
                        std::make_shared<Hero>("Galadriel",   Race::Elf,      HeroParameters(4,120))}},
        {Race::Dwarf,  {std::make_shared<Hero>("Sturmir",     Race::Dwarf,    HeroParameters(2,130)),
                        std::make_shared<Hero>("Wotan",       Race::Dwarf,    HeroParameters(2,130))}},
        {Race::Undead, {std::make_shared<Hero>("Sandro",      Race::Undead,   HeroParameters(3,115)),
                        std::make_shared<Hero>("Mortis",      Race::Undead,   HeroParameters(3,115))}}
    };
}
