#ifndef I_TOWN_H
#define I_TOWN_H

#include <map>
#include "units/unit_levels.h"

#include "interfaces/i_interactable_object.h"

#include "forward/i_hero_forward.h"
#include "forward/i_player_forward.h"
#include "forward/i_unit_forward.h"

class MapElement;

class ITown
    : public IInteractableObject
{
public:
    ITown() : IInteractableObject(ObjectType::Town) {}
    virtual ~ITown() = default;

    virtual const std::string& name() const = 0;
    virtual IPlayerShared controllingPlayer() = 0;
    virtual IPlayerSharedConst controllingPlayer() const = 0;
    virtual void setControllingPLayer(IPlayerShared&_controllingPLayer) = 0;
    virtual void freeFromControllingPlayer() = 0;

    virtual void acceptHero(IHeroShared& _hero) = 0;
    virtual IHeroShared garrisonHero() = 0;
    virtual void heroLeaved() = 0;

    virtual void processWeeklyGrow() = 0;
    virtual const std::map<UnitLevel, IUnitShared>& getRecruitsMap() = 0;
    virtual IUnitShared getRecruitUnit(UnitLevel _level) = 0;
};

#endif // I_TOWN_H
