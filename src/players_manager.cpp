#include "players_manager.h"

#include "player.h"

#include "interfaces/i_hero.h"
#include "interfaces/i_heroes_manager.h"

PlayersManager::PlayersManager()
{

}

IPlayerShared PlayersManager::createNewPLayer(ControlSide _controlSide,
                                              Race _race,
                                              Color _color)
{
    auto newPlayer = std::make_shared<Player>(generateNewId(),
                                              _controlSide,
                                              _race,
                                              _color);

    m_playersMap[newPlayer->id()] = newPlayer;

    // If there heroes of the same race on the map,
    // we take control over them. Map must be loaded at this point.
    auto heroes = m_heroManager->getHeroes(_race);
    for (auto& hero : heroes)
    {
        if (hero->placedOnMap())
        {
            hero->setControllingPLayer(m_playersMap[newPlayer->id()]);
            newPlayer->addHero(hero);
        }
    }

    return m_playersMap[newPlayer->id()];
}

std::queue<IPlayerShared> PlayersManager::getPlayersQueue() const
{
    std::queue<IPlayerShared> playersQueue;

    for (const auto& it : m_playersMap)
    {
        auto player = it.second;
        if (!player->isAlive())
            continue;

        playersQueue.push(it.second);
    }

    return playersQueue;
}

IPlayerSharedVec PlayersManager::getPlayersVec() const
{
    IPlayerSharedVec playersQueue;

    for (const auto& it : m_playersMap)
    {
        auto player = it.second;
        if (!player->isAlive())
            continue;

        playersQueue.push_back(it.second);
    }

    return playersQueue;

}

void PlayersManager::setHeroManager(std::shared_ptr<IHeroesManager> _heroManager)
{
    m_heroManager = _heroManager;
}

bool PlayersManager::hireHero(uint8_t _playerId, const std::string &_heroName)
{
    auto it = m_playersMap.find(_playerId);
    if (it == m_playersMap.end())
        return false;

    auto player = it->second;

    IHeroShared newHero;

    if (_heroName.empty())
        newHero = m_heroManager->getNextAvailableHero(player->race());
    else
        newHero = m_heroManager->getHero(_heroName);

    if (!newHero)
        return false; // No available hero or no hero with given name

    if (newHero->controllingPlayer())
        return false; // Error, hero already hired

    newHero->setControllingPLayer(player);
    player->addHero(newHero);

    return true;
}

