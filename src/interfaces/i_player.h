#ifndef I_PLAYER_H
#define I_PLAYER_H

#include <memory>
#include <vector>

#include "control_side.h"
#include "races.h"
#include "colors.h"
#include "point_2d.h"
#include "resource_types.h"

#include "interfaces/i_town.h"

#include "forward/i_hero_forward.h"
#include "forward/i_town_forward.h"

class IPlayer
{
public:
    IPlayer() = default;
    virtual ~IPlayer() = default;

    virtual Race race() const = 0;
    virtual Color color() const = 0;
    virtual ControlSide controlSide() const = 0;
    virtual bool isHumanControl() const = 0;
    virtual uint8_t id() const = 0;

    virtual bool isAlive() const = 0;
    virtual void executeByBeheading() = 0;
    virtual void surrenderAllTowns() = 0;

    virtual void addHero(IHeroShared& _hero) = 0;
    virtual void removeHero(IHeroShared& _hero) = 0;
    virtual bool hasUndefeatedHero() const = 0;

    virtual IHeroShared getHero(const std::string& _name) = 0;
    virtual IHeroSharedConst getHero(const std::string& _name) const = 0;

    virtual IHeroSharedVec getHeroes() = 0;
    virtual IHeroSharedConstVec getHeroes() const = 0;

    virtual int resourceAmount(ResourceType _type) const = 0;
    virtual void acceptResource(ResourceType _type, int _value) = 0;

    virtual ResourceMap& getResourceMap() = 0;
    virtual const ResourceMap& getResourceMap() const = 0;

    virtual void captureTown(ITownShared _town) = 0;
    virtual void abandonTown(ITownShared _town) = 0;
    virtual ITownSharedVec getTowns()  = 0;
    virtual ITownShared getTownByGarrison(const IHeroShared& _hero) = 0;
    virtual ITownSharedConst getTownByGarrison(const IHeroShared& _hero) const = 0;
};

#endif // I_PLAYER_H
