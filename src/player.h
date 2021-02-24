#ifndef PLAYER_H
#define PLAYER_H

/**
  * Someone, who controlling the race. Can be human or AI
  */

#include <memory>
#include <vector>
#include <map>

#include "point_2d.h"
#include "resource_types.h"

#include "interfaces/i_player.h"

#include "forward/i_hero_forward.h"

class IHeroManager;
class ITown;

class Player
        : public IPlayer
{
public:
    Player() = default;
    Player(uint8_t _id,
           ControlSide _controlSide,
           Race _race,
           Color _color);

    Race race() const override;
    void setRace(const Race& _race);

    Color color() const override;
    void setColor(const Color& _color);

    ControlSide controlSide() const override;
    void setControl(const ControlSide& _controlSide);
    bool isHumanControl() const override;

    uint8_t id() const override;
    void setId(const uint8_t& _id);

    bool isAlive() const override { return m_isAlive; }
    void executeByBeheading() override { m_isAlive = false; }
    void surrenderAllTowns() override;

    void addHero(IHeroShared& _hero) override;
    void removeHero(IHeroShared& _hero) override;
    bool hasUndefeatedHero() const override;

    IHeroShared getHero(const std::string& _name) override;
    IHeroSharedConst getHero(const std::string& _name) const override;

    IHeroSharedVec getHeroes() override;
    IHeroSharedConstVec getHeroes() const override;

    int resourceAmount(ResourceType _type) const override;
    void acceptResource(ResourceType _type, int _value) override;

    ResourceMap& getResourceMap() override;
    const ResourceMap& getResourceMap() const override;

    void captureTown(ITownShared _town) override;
    void abandonTown(ITownShared _town) override;
    ITownSharedVec getTowns()  override;
    ITownShared getTownByGarrison(const IHeroShared& _hero) override;
    ITownSharedConst getTownByGarrison(const IHeroShared& _hero) const override;

private:
    uint8_t m_id;
    ControlSide m_control = ControlSide::AI;
    Race m_race = Race::Unknown;
    Color m_color = Color::White;
    bool m_isAlive = true;

    IHeroWeakVec m_heroesVec;
    ITownSharedVec m_townsVec;
    ResourceMap m_resourceMap;
};

#endif // PLAYER_H
