#include "battle_manager.h"

#include <algorithm>

#include "battle_queue_element.h"
#include "attack_result.h"

#include "events/event_manager.h"
#include "events/event_battle_end.h"

#include "interfaces/i_unit_factory.h"
#include "interfaces/i_game_logic.h"
#include "interfaces/i_hero.h"
#include "interfaces/i_unit.h"

void BattleManager::setGameLogic(std::shared_ptr<IGameLogic> _gameLogic)
{
    m_gameLogic = _gameLogic;
}

void BattleManager::setUnitFactory(std::shared_ptr<IUnitFactory> _unitFactory)
{
    m_unitFactory = _unitFactory;
}

void BattleManager::setPartisipants(IHeroShared &_heroOnLeft, IHeroShared &_heroOnRight)
{
    m_heroOnLeft = _heroOnLeft;
    m_heroOnRight = _heroOnRight;

    initAllUnitsVec();
    rememberUnitsPartySize();
    onNextUnit();
}

void BattleManager::onNextUnit()
{
    if (m_currentUnit)
        m_currentUnit->actedOnThisTurn = true;

    if (m_allUnitsVec.empty())
        return;

    sortUnitsQueue(m_allUnitsVec);

    m_currentUnit = getNextUnit();
    if (!m_currentUnit)
        onNextTurn();
}

void BattleManager::onNextTurn()
{
    for (auto& battleQueueElem : m_allUnitsVec)
        battleQueueElem->actedOnThisTurn = false;

    onNextUnit();
}

void BattleManager::surrender()
{
    if (!m_currentUnit)
        return;

    IHeroShared& hero = getHero(m_currentUnit);

    hero->setDefeated(true);
    endBattle();
}

void BattleManager::skipMove()
{
    onNextUnit();
}

AttackResult BattleManager::processAttack(UnitPosition _defenderPos, bool _attackedUnitOnLeft)
{
    AttackResult result;

    if (!m_currentUnit)
        return result;

    auto attacker = m_currentUnit->unit.lock();
    if (!attacker)
        return result;

    if (_attackedUnitOnLeft == m_currentUnit->fromLeftArmy)
    {
        // Same army unit, applying buff, heal, etc
        return result;
    }

    auto defender = m_currentUnit->fromLeftArmy ? m_heroOnRight->getUnit(_defenderPos)
                                                : m_heroOnLeft->getUnit(_defenderPos);

    // Empty position
    if (!defender || (defender->partySize() < 1))
        return result;

    result = attacker->attack(defender);

    result.defender.unit = defender;
    result.attacker.unit = attacker;

    updateIsDefeatedState(m_heroOnLeft);
    updateIsDefeatedState(m_heroOnRight);

    if (needToEndBattle())
        endBattle();
    else
        onNextUnit();

    return result;
}

const BattleQueueSharedVec &BattleManager::getBattleQueueVec() const
{
    return m_allUnitsVec;
}

void BattleManager::initAllUnitsVec()
{
    auto leftArmy = m_heroOnLeft->getArmy();
    auto rightArmy = m_heroOnRight->getArmy();

    m_allUnitsVec.reserve(leftArmy.size() + rightArmy.size());
    auto addToVec = [this](const IUnitSharedVec& _anotherVec, bool _fromLeftArmy)
    {
        for (const auto& unit : _anotherVec)
        {
            auto newElem = std::make_shared<BattleQueueElement>(unit);
            newElem->fromLeftArmy = _fromLeftArmy;
            m_allUnitsVec.emplace_back(newElem);
        }
    };

    addToVec(leftArmy, true);
    addToVec(rightArmy, false);
}

void BattleManager::rememberUnitsPartySize()
{
    for (const auto& elem : m_allUnitsVec)
    {
        if (auto unit = elem->unit.lock())
            m_partySizeBeforeBattle[elem] = unit->partySize();
    }
}

void BattleManager::sortUnitsQueue(BattleQueueSharedVec &_vec)
{
    std::sort(_vec.begin(), _vec.end(),
              [](const BattleQueueElemShared& _first, const BattleQueueElemShared& _second) -> bool
    {
        const auto firstUnit = _first->unit.lock();
        const auto secondUnit = _second->unit.lock();

        // If both units are dead, doesn't matter
        if (!firstUnit->partySize() && !secondUnit->partySize())
            return &_first > &_second;

        // Still alive
        if (!firstUnit->partySize() || !secondUnit->partySize())
            return firstUnit->partySize() > secondUnit->partySize();

        // Not acted on this turn
        if (_first->actedOnThisTurn != _second->actedOnThisTurn)
            return !_first->actedOnThisTurn && _second->actedOnThisTurn;

        // Or faster
        if (firstUnit->stats().speed != secondUnit->stats().speed)
            return firstUnit->stats().speed > secondUnit->stats().speed;

        // Or the one on the left side
        if (_first->fromLeftArmy != _second->fromLeftArmy)
            return _first->fromLeftArmy && !_second->fromLeftArmy;

        // Or highest level
        if (firstUnit->level() != secondUnit->level())
            return firstUnit->level() > secondUnit->level();

        // Or race priority
        if (firstUnit->race() != secondUnit->race())
            return firstUnit->race() > secondUnit->race();

        /// \todo by position

        // They are the same. Sorting by pointer addres for
        // no specific reason
        return &_first > &_second;
    });
}

IHeroShared &BattleManager::getHero(BattleQueueElemShared _elem)
{
    return _elem->fromLeftArmy ? m_heroOnLeft
                               : m_heroOnRight;
}

BattleQueueElemShared BattleManager::getNextUnit()
{
    for (auto& battleQueueElem : m_allUnitsVec)
    {
        if (battleQueueElem->actedOnThisTurn)
            continue;
        if (battleQueueElem->isDead())
            continue;

        return battleQueueElem;
    }

    return BattleQueueElemShared();
}

bool BattleManager::needToEndBattle()
{
    return m_heroOnLeft->isDefeated() || m_heroOnRight->isDefeated();
}

void BattleManager::endBattle()
{
    calcResults();

    auto battleEndEvent = std::make_shared<EventBattleEnd>(m_result);
    auto& eventManager = EventManager::getInstance();
    eventManager.addEventToQueue(std::move(battleEndEvent));
    eventManager.currentEventEnds();

    m_result.clear();
    m_allUnitsVec.clear();
    m_currentUnit = nullptr;
    m_heroOnLeft.reset();
    m_heroOnRight.reset();
    m_partySizeBeforeBattle.clear();
}

void BattleManager::calcResults()
{
    m_result.armyOnLeft.hero = m_heroOnLeft;
    m_result.armyOnRight.hero = m_heroOnRight;

    for (const auto& it : m_partySizeBeforeBattle)
    {
        const auto& elem = it.first;
        int intialSize = it.second;
        if (auto unit = elem->unit.lock())
        {
            int sizeDiff = intialSize - unit->partySize();
            if (sizeDiff > 0)
            {
                Participant& participant = elem->fromLeftArmy ? m_result.armyOnLeft
                                                              : m_result.armyOnRight;
                participant.losses.push_back(m_unitFactory->createUnit(unit->type(), sizeDiff));
            }
        }
    }
}

void BattleManager::updateIsDefeatedState(IHeroShared &_hero)
{
    bool defeated = true;
    const auto& army = _hero->getArmy();
    for (const auto& unit : army)
    {
        if (unit->partySize() > 0)
        {
            defeated = false;
            break;
        }
    }

    _hero->setDefeated(defeated);
}
