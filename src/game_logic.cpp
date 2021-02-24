#include "game_logic.h"

#include "grid_maker.h"
#include "map_element.h"
#include "calendar.h"
#include "battle_manager.h"
#include "attack_result.h"

#include "events/event_manager.h"
#include "events/event_notify.h"

#include "interfaces/i_hero.h"
#include "interfaces/i_unit.h"
#include "interfaces/i_town.h"
#include "interfaces/i_map_manager.h"
#include "interfaces/i_players_manager.h"
#include "interfaces/i_player.h"
#include "interfaces/i_towns_manager.h"
#include "interfaces/i_unit_factory.h"
#include "interfaces/i_user_interface.h"

void GameLogic::createPlayer(ControlSide _controlSide,
                             Race _race,
                             Color _color)
{
    auto newPlayer = m_playersManager->createNewPLayer(_controlSide, _race, _color);

    auto heroes = newPlayer->getHeroes();
    for (auto& hero : heroes)
    {
        hero->assignUnit(m_unitFactory->createUnit(_race, UnitLevel::Level_1, 15));
        hero->assignUnit(m_unitFactory->createUnit(_race, UnitLevel::Level_2, 8));
        hero->assignUnit(m_unitFactory->createUnit(_race, UnitLevel::Level_3, 5));
    }
}

void GameLogic::onMapSelected(const std::string &_mapName)
{
    m_currentMapName = _mapName;
    m_mapManager->loadMap(m_currentMapName);
}

void GameLogic::onStartGame()
{
    onNextTurn();
}

void GameLogic::moveCurrentHero(MoveDirection _dir)
{
    if (!m_currentPlayer || !m_currentPlayer->isHumanControl() || !m_selectedHero)
        return;

    auto hero = m_currentPlayer->getHero(m_selectedHero->name());
    if (!hero)
        return;

    auto prevCoords = hero->tileCoord();
    auto newCoords = prevCoords;
    switch (_dir)
    {
    case MoveDirection::Down:   newCoords.y++; break;
    case MoveDirection::Left:   newCoords.x--; break;
    case MoveDirection::Right:  newCoords.x++; break;
    case MoveDirection::Up:     newCoords.y--; break;
    default: return;
    }

    MapElement* newTile = m_mapManager->getMapElement(newCoords);
    if (!newTile)
        return;

    if (!hero->canMoveTo(newTile->type()))
        return;

    auto interactableObject = newTile->interactableObject();
    if (interactableObject)
    {
        interactableObject->interact(hero, interactableObject);

        // Some interactions (like picking resources) will be finished
        // at this point and - for optimisation - are ment to be followed by management
        // of object existance on a tile.
        // But some (like battles) will generate an event and, therefore,
        // will be finished later. In that case releasing map tiles from object
        // will be called in GameLogic::removeDeletedObjectsFromMap()
        if (interactableObject->waitingForDestroying())
            newTile->releaseInteractableObject();
    }
    else
    {
        // Process previous tile
        MapElement* prevTile = m_mapManager->getMapElement(prevCoords);

        if (!hero->canMoveFrom(prevTile->type()))
            return;

        if (prevTile->interactableObject() == hero)
            prevTile->releaseInteractableObject();

        // Check on leaving town
        auto town = m_currentPlayer->getTownByGarrison(hero);
        if (town)
            town->heroLeaved();

        // Process new tile
        hero->reduceMovePoints(prevTile->type());
        hero->moveToTile(newCoords);
        newTile->acceptInteractableObject(hero);
    }

    m_mapManager->processPlayerVisibility(m_currentPlayer);
}

void GameLogic::onNextTurn()
{
    m_selectedHero.reset();

    if (m_playersTurnQueue.empty())
    {
        m_playersTurnQueue = m_playersManager->getPlayersQueue();
        if (m_playersTurnQueue.empty())
            return;

        auto playersVec = m_playersManager->getPlayersVec();

        for (auto& player : playersVec)
        {
            if (!player)
                continue;

            auto heroes = player->getHeroes();
            for (auto& hero : heroes)
            {
                if (hero)
                    hero->recoverMovePoints();
            }
        }

        Calendar::getInstance().onEndOfDay();
    }

    m_currentPlayer.reset();
    while(!m_currentPlayer)
    {
        m_currentPlayer = m_playersTurnQueue.front();
        m_playersTurnQueue.pop();

        if (!m_currentPlayer->isAlive())
            m_currentPlayer.reset();

        if (m_playersTurnQueue.empty())
            break;
    }

    if (!m_currentPlayer)
        onNextTurn();

    if (m_currentPlayer->isHumanControl())
        m_mapManager->processPlayerVisibility(m_currentPlayer);
}

void GameLogic::onNewWeek()
{
    m_townsManager->processWeeklyGrow();
}

IPlayerShared GameLogic::getCurrentPlayer()
{
    return m_currentPlayer;
}

void GameLogic::onHeroSelected(const std::string &_name)
{
    if (!m_currentPlayer)
        return;

    auto selectedHero = m_currentPlayer->getHero(_name);
    if (selectedHero)
        m_selectedHero = selectedHero;
}

void GameLogic::setPlayersManager(std::shared_ptr<IPlayersManager> _manager)
{
    m_playersManager = _manager;
}

void GameLogic::setTownsManager(std::shared_ptr<ITownsManager> _townsManager)
{
    m_townsManager = _townsManager;
}

void GameLogic::setMapManager(std::shared_ptr<IMapManager> _manager)
{
    m_mapManager = _manager;
}

void GameLogic::setUnitFactory(std::shared_ptr<IUnitFactory> _unitFactory)
{
    m_unitFactory = _unitFactory;
}

void GameLogic::setUi(std::shared_ptr<IUserInterface> _ui)
{
    m_ui = _ui;
}

void GameLogic::setBattleManager(std::shared_ptr<IBattleManager> _battleManager)
{
    m_battleManager = _battleManager;
}

IHeroSharedConst GameLogic::getSelectedHero() const
{
    return m_selectedHero;
}

IHeroShared GameLogic::getSelectedHero()
{
    if (!m_currentPlayer || !m_selectedHero)
        return IHeroShared();

    return m_currentPlayer->getHero(m_selectedHero->name());
}

void GameLogic::swapUnits(IUnitSharedConst _first, IUnitSharedConst _second)
{
    if (!m_currentPlayer || !m_selectedHero)
        return;

    auto hero = m_currentPlayer->getHero(m_selectedHero->name());

    if (hero)
        hero->swapUnits(_first, _second);
}

void GameLogic::battleBegin(IHeroShared &_attacker, IHeroShared &_defender)
{
    m_ui->battleBegin(_attacker, _defender);
    m_battleManager->setPartisipants(_attacker, _defender);
}

void GameLogic::battleEnd(const BattleResult& _result)
{
    removeDeletedObjectsFromMap();

    auto checkPlayerDefeat = [](IHeroShared& _hero)
    {
        if (!_hero)
            return;

        auto player = _hero->controllingPlayer();
        if (!player)
            return;

        if (!player->hasUndefeatedHero())
        {
            player->surrenderAllTowns();
            player->executeByBeheading();
        }
    };

    _result.armyOnLeft.hero->getRidOfDeadBodies();
    _result.armyOnRight.hero->getRidOfDeadBodies();

    checkPlayerDefeat(_result.armyOnLeft.hero);
    checkPlayerDefeat(_result.armyOnRight.hero);

    checkGameOver();

    if (!m_currentPlayer->isAlive())
        onNextTurn();
}

void GameLogic::surrender()
{
    m_battleManager->surrender();
}

void GameLogic::skipMove()
{
    m_battleManager->skipMove();
}

AttackResult GameLogic::processAttack(UnitPosition _defenderPos, bool _attackedUnitOnLeft)
{
    return m_battleManager->processAttack(_defenderPos, _attackedUnitOnLeft);
}

const BattleQueueSharedVec &GameLogic::getBattleQueueVec() const
{
    return m_battleManager->getBattleQueueVec();
}

void GameLogic::setHeroAsGarrison(ITownShared &_town, IHeroShared &_hero)
{
    _town->acceptHero(_hero);

    auto tileCoord = _hero->tileCoord();
    auto tile = m_mapManager->getMapElement(tileCoord);
    tile->releaseInteractableObject();

    _hero->moveToTile(_town->tileCoord());
}

void GameLogic::hireUnitInTown(ITownShared _town, IHeroShared _hero, UnitLevel _level, int _amount)
{
    auto unitToRecruit = _town->getRecruitUnit(_level);
    if (!unitToRecruit)
        return;
    if (unitToRecruit->partySize() < _amount)
        return;

    if (!_hero->canAssignUnit(unitToRecruit))
        return;

    auto player = _hero->controllingPlayer();
    if (!player)
        return;

    auto& playerResources = player->getResourceMap();

    bool canAfford = true;
    for (auto& it : playerResources)
    {
        auto resourceType = it.first;
        auto amount = it.second;

        if (amount < (unitToRecruit->cost(resourceType) * _amount))
        {
            canAfford = false;
            break;
        }
    }

    if (!canAfford)
        return;

    if (_hero->assignUnit(m_unitFactory->createUnit(unitToRecruit->type(), _amount)))
    {
        // Reduce player resources
        for (auto& it : playerResources)
        {
            auto resourceType = it.first;
            auto& amount = it.second;

            amount -= (unitToRecruit->cost(resourceType) * _amount);
        }

        // Reduce recruit unit party size
        unitToRecruit->reducePartySizeBy(_amount);
    }
}

void GameLogic::removeDeletedObjectsFromMap()
{
    Point2D_int size = m_mapManager->getMapSize();

    for (int y = 0; y < size.y; y++)
    {
        for (int x = 0; x < size.x; x++)
        {
            // Invisible object could not be a part of interaction,
            // so we can skip hidden tiles
            if (!m_mapManager->isVisibleByPlayer({x, y}, m_currentPlayer))
                continue;

            MapElement* tile = m_mapManager->getMapElement({x, y});
            if (tile->interactableObject() &&
                tile->interactableObject()->waitingForDestroying())
                tile->releaseInteractableObject();
        }
    }
}

void GameLogic::checkGameOver()
{
    auto players = m_playersManager->getPlayersVec();

    int alivePlayersCount = 0;
    int aliveHumanPlayersCount = 0;
    IPlayerShared winner;

    for (const auto& player : players)
    {
        if (player->isAlive())
        {
            alivePlayersCount++;

            if (player->isHumanControl())
                aliveHumanPlayersCount++;

            winner = player;
        }
    }

    bool gameOver = false;
    std::string endGameText;
    if (aliveHumanPlayersCount == 0)
    {
        // All human players are defeated
        if (alivePlayersCount == 1)
        {
            // AI winner
            gameOver = true;
            endGameText = "You are defeated! Heroes with " + Convert::colorToText(winner->color()) +
                          " banners are the new rulers!";
        }
        else
        {
            // Several AI still alive, no winner, just game over
            gameOver = true;
            endGameText = "You are defeated! The story will go on without you!";
        }
    }
    else if (aliveHumanPlayersCount == 1 && alivePlayersCount == 1)
    {
        // Human winner
        gameOver = true;
        endGameText = "Congratulations! Heroes with " + Convert::colorToText(winner->color()) +
                      " banners are the new rulers!";
    }

    if (gameOver)
    {
        auto eventNotify = std::make_shared<EventNotify>(endGameText);
        EventManager::getInstance().addEventToQueue(std::move(eventNotify));
    }
}

