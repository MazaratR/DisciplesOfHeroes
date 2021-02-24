#include "hero.h"
#include <iostream>

#include "hero_functions.h"

#include "events/event_manager.h"
#include "events/event_battle.h"

#include "interfaces/i_player.h"
#include "interfaces/i_unit.h"

Hero::Hero(const std::string &_name, Race _race, const HeroParameters &_params)
    : m_name(_name)
    , m_race(_race)
    , m_params(_params)
{
    m_moveCostMap = {
        {TerrainType::Dirt,  20},
        {TerrainType::Snow,  15},
        {TerrainType::Grass, 10}
    };
}

const std::string &Hero::name() const
{
    return m_name;
}

IPlayerShared Hero::controllingPlayer()
{
    return m_controllingPLayer.lock();
}

IPlayerSharedConst Hero::controllingPlayer() const
{
    return m_controllingPLayer.lock();
}

void Hero::setControllingPLayer(IPlayerShared &_controllingPLayer)
{
    m_controllingPLayer = _controllingPLayer;
}

void Hero::collectResource(ResourceType _type, int _value)
{
    if (auto player = m_controllingPLayer.lock())
    {
        player->acceptResource(_type, _value);
    }
}

const HeroParameters& Hero::params() const
{
    return m_params;
}

void Hero::interact(IHeroShared &_hero, IInteractObjShared &_selfShared)
{
    if (!_hero || (_hero.get() == this))
        return;

    if (this->controllingPlayer() != _hero->controllingPlayer())
    {
        // Battle
        if (auto player = m_controllingPLayer.lock())
        {
            auto defender = std::dynamic_pointer_cast<IHero>(_selfShared);
            if (!defender)
                return;

            // Creating battle event
            auto battleEvent = std::make_shared<EventBattle>(_hero, // Attacker
                                                             defender, // Defender
                                                             [_hero, defender]()
            {
                // Function will be called after battle is over
                if (_hero && _hero->isDefeated())
                    HeroFunctions::onHeroDefeat(_hero);
                if (defender && defender->isDefeated())
                    HeroFunctions::onHeroDefeat(defender);
            });

            EventManager::getInstance().addEventToQueue(std::move(battleEvent));
        }
    }
    else
    {
        // Exchange
    }
}

std::string Hero::title() const
{
    return m_name;
}

bool Hero::assignUnit(const IUnitShared& _unit)
{
    auto unitPos = getPositionForUnit(_unit);
    if (unitPos == UnitPosition::Unknown)
        return false;

    m_army.setUnit(unitPos, _unit);
    return true;
}

bool Hero::canAssignUnit(const IUnitShared &_unit)
{
    return getPositionForUnit(_unit) != UnitPosition::Unknown;
}

UnitPosition Hero::getPositionForUnit(const IUnitShared &_unit) const
{
    const auto& unitInArmy = m_army.getUnit(_unit->type());
    if (!unitInArmy)
        return m_army.getFreeArmyPosition();
    else
        return m_army.getUnitPosition(unitInArmy);
}

IUnitShared& Hero::getUnit(UnitPosition _position)
{
    return m_army.getUnit(_position);
}

IUnitSharedConst Hero::getUnit(UnitPosition _position) const
{
    return m_army.getUnit(_position);
}

IUnitSharedVec Hero::getArmy()
{
    return m_army.getArmyVec();
}

IUnitSharedConstVec Hero::getArmy() const
{
    return m_army.getArmyVec();
}

bool Hero::hasUnitInArmy(const IUnitSharedConst &_unit) const
{
    return m_army.hasUnit(_unit);
}

UnitPosition Hero::getUnitPosition(const IUnitSharedConst &_unit) const
{
    return m_army.getUnitPosition(_unit);
}

void Hero::swapUnits(IUnitSharedConst _first, IUnitSharedConst _second)
{
    m_army.swapUnits(_first, _second);
}

void Hero::getRidOfDeadBodies()
{
    m_army.getRidOfDeadBodies();
}

uint16_t Hero::movePointsLeft() const
{
    return m_params.m_movePoints.current;
}

uint16_t Hero::movePointsMax() const
{
    return m_params.m_movePoints.max;
}

bool Hero::canMoveTo(TerrainType _terrainType) const
{
    switch (_terrainType)
    {
    case TerrainType::Dirt:     return true;
    case TerrainType::Grass:    return true;
    case TerrainType::Snow:     return true;
    /// \todo Consider ships, special spells and items
    default:                    return false;
    }
}

bool Hero::canMoveFrom(TerrainType _terrainType) const
{
    auto cost = getMoveCost(_terrainType);
    if (!cost)
        return false;

    return m_params.m_movePoints.current >= cost;
}

void Hero::reduceMovePoints(TerrainType _terrainType)
{
    auto cost = getMoveCost(_terrainType);
    if (!cost)
        return;

    m_params.m_movePoints.current -= cost;
}

void Hero::recoverMovePoints()
{
    m_params.m_movePoints.current = m_params.m_movePoints.max;
}

uint8_t Hero::getMoveCost(TerrainType _terrainType) const
{
    auto it = m_moveCostMap.find(_terrainType);
    if (it == m_moveCostMap.end())
        return 0;

    return it->second;
}

