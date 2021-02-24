#ifndef I_HERO_H
#define I_HERO_H

#include <string>

#include "hero_parameters.h"
#include "point_2d.h"
#include "resource_types.h"
#include "object_types.h"
#include "terrain.h"

#include "units/unit_position.h"

#include "forward/i_player_forward.h"
#include "forward/i_unit_forward.h"

#include "interfaces/i_interactable_object.h"

class IHero
        : public IInteractableObject
{
public:
    IHero() : IInteractableObject(ObjectType::Hero) {}
    virtual ~IHero() = default;

    virtual const std::string& name() const = 0;

    virtual IPlayerShared controllingPlayer() = 0;
    virtual IPlayerSharedConst controllingPlayer() const = 0;
    virtual void setControllingPLayer(IPlayerShared& _controllingPLayer) = 0;

    virtual void collectResource(ResourceType _type, int _value) = 0;

    virtual const HeroParameters& params() const = 0;

    virtual bool assignUnit(const IUnitShared& _unit) = 0;
    virtual bool canAssignUnit(const IUnitShared& _unit) = 0;
    virtual IUnitShared& getUnit(UnitPosition _position) = 0;
    virtual IUnitSharedConst getUnit(UnitPosition _position) const = 0;
    virtual IUnitSharedVec getArmy() = 0;
    virtual IUnitSharedConstVec getArmy() const = 0;
    virtual bool hasUnitInArmy(const IUnitSharedConst& _unit) const = 0;
    virtual UnitPosition getUnitPosition(const IUnitSharedConst &_unit) const = 0;
    virtual void swapUnits(IUnitSharedConst _first, IUnitSharedConst _second) = 0;

    virtual void getRidOfDeadBodies() = 0;

    virtual bool isDefeated() const = 0;
    virtual void setDefeated(bool _defeated) = 0;

    virtual uint16_t movePointsLeft() const = 0;
    virtual uint16_t movePointsMax()  const = 0;

    virtual bool canMoveTo(TerrainType _terrainType) const = 0;
    virtual bool canMoveFrom(TerrainType _terrainType) const = 0;
    virtual void reduceMovePoints(TerrainType _terrainType) = 0;
    virtual void recoverMovePoints() = 0;
};

#endif // I_HERO_H
