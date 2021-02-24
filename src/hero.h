#ifndef HERO_H
#define HERO_H

/**
  * Hero on the game map. Can be controlled by Player or just chill in Tavern
  */

#include <memory>
#include <vector>

#include "races.h"
#include "colors.h"
#include "control_side.h"
#include "point_2d.h"
#include "resource_types.h"
#include "hero_parameters.h"
#include "units/army.h"

#include "interfaces/i_hero.h"

#include "forward/i_unit_forward.h"
#include "forward/i_player_forward.h"

class Hero
        : public IHero
{
public:
    Hero(const std::string& _name, Race _race, const HeroParameters& _params);
    Hero(const Hero& _other) = delete;

    const std::string& name() const override;

    void setControllingPLayer(IPlayerShared& _controllingPLayer) override;
    IPlayerShared controllingPlayer() override;
    IPlayerSharedConst controllingPlayer() const override;

    const HeroParameters& params() const override;
    std::string title() const override;

    void interact(IHeroShared& _hero, IInteractObjShared& _selfShared) override;
    void collectResource(ResourceType _type, int _value) override;

    bool assignUnit(const IUnitShared &_unit) override;

    bool canAssignUnit(const IUnitShared& _unit) override;
    UnitPosition getPositionForUnit(const IUnitShared& _unit) const;

    IUnitShared &getUnit(UnitPosition _position) override;
    IUnitSharedConst getUnit(UnitPosition _position) const override;
    IUnitSharedVec getArmy() override;
    IUnitSharedConstVec getArmy() const override;
    bool hasUnitInArmy(const IUnitSharedConst &_unit) const override;
    UnitPosition getUnitPosition(const IUnitSharedConst &_unit) const override;
    void swapUnits(IUnitSharedConst _first, IUnitSharedConst _second) override;
    void getRidOfDeadBodies() override;

    bool isDefeated() const override { return m_isDefeated; }
    void setDefeated(bool _defeated) override { m_isDefeated = _defeated; }

    uint16_t movePointsLeft() const override;
    uint16_t movePointsMax()  const override;

    bool canMoveTo(TerrainType _terrainType) const override;
    bool canMoveFrom(TerrainType _terrainType) const override;
    void reduceMovePoints(TerrainType _terrainType) override;
    void recoverMovePoints() override;

private:
    uint8_t getMoveCost(TerrainType _terrainType) const;

private:
    bool m_isDefeated = false;
    std::string m_name;
    Race m_race;
    HeroParameters m_params;

    IPlayerWeak m_controllingPLayer;
    Army m_army;

    std::map<TerrainType,uint8_t> m_moveCostMap;
};

#endif // HERO_H
