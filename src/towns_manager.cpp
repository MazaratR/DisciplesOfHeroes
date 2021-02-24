#include "towns_manager.h"

#include "town.h"

#include "interfaces/i_unit_factory.h"

void TownsManager::setUnitFantory(std::shared_ptr<IUnitFactory> _unitFatory)
{
    m_unitFactory = _unitFatory;
    initTowns();
}

ITownShared TownsManager::getTown(const std::string &_name) const
{
    for (const auto& mapIt : m_townsPool)
    {
        for (const auto& town : mapIt.second)
        {
            if (town->name() == _name)
                return town;
        }
    }

    return ITownShared();
}

ITownSharedVec TownsManager::getTowns(Race _race) const
{
    const auto& it = m_townsPool.find(_race);
    if (it == m_townsPool.end())
        return ITownSharedVec();

    return it->second;
}

void TownsManager::processWeeklyGrow()
{
    for (const auto& mapIt : m_townsPool)
    {
        for (const auto& town : mapIt.second)
        {
            if (town->placedOnMap())
                town->processWeeklyGrow();
        }
    }
}

void TownsManager::initTowns()
{
    m_townsPool = {
      {Race::Human,     {std::make_shared<Town>(Race::Human,  "Albion",          m_unitFactory),
                         std::make_shared<Town>(Race::Human,  "Lothric",         m_unitFactory)}},
      {Race::Elf,       {std::make_shared<Town>(Race::Elf,    "Great Tree",      m_unitFactory),
                         std::make_shared<Town>(Race::Elf,    "Spirits Forge",   m_unitFactory)}},
      {Race::Dwarf,     {std::make_shared<Town>(Race::Dwarf,  "Great Fortress",  m_unitFactory),
                         std::make_shared<Town>(Race::Dwarf,  "Moria",           m_unitFactory)}},
      {Race::Undead,    {std::make_shared<Town>(Race::Undead, "Stonekeep",       m_unitFactory),
                         std::make_shared<Town>(Race::Undead, "Morbid Gates",    m_unitFactory)}}
    };
}
