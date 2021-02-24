#ifndef TOWN_H
#define TOWN_H

#include <memory>
#include <map>
#include "races.h"
#include "units/unit_levels.h"

#include "interfaces/i_town.h"

#include "forward/i_hero_forward.h"
#include "forward/i_player_forward.h"
#include "forward/i_unit_forward.h"

class IUnitFactory;

class Town
        : public ITown
{
public:
    Town(Race _race, const std::string& _name, const std::shared_ptr<IUnitFactory>& _unitFactory);

    const std::string& name() const override;

    IPlayerShared controllingPlayer() override;
    IPlayerSharedConst controllingPlayer() const override;
    void setControllingPLayer(IPlayerShared& _controllingPLayer) override;
    void freeFromControllingPlayer() override;

    void interact(IHeroShared& _hero, IInteractObjShared& _selfShared) override;
    std::string title() const override;

    void acceptHero(IHeroShared& _hero) override;
    IHeroShared garrisonHero() override;
    void heroLeaved() override;

    void processWeeklyGrow() override;
    const std::map<UnitLevel, IUnitShared>& getRecruitsMap() override; 
    IUnitShared getRecruitUnit(UnitLevel _level) override;

private:
    std::shared_ptr<IUnitFactory> m_unitFactory;

    Race m_race = Race::Unknown;
    std::string m_name;
    IPlayerWeak m_controllingPlayer;
    IHeroWeak m_heroInside;

    std::map<UnitLevel, uint8_t> m_weeklyGrowAmount;
    std::map<UnitLevel, IUnitShared> m_recruitsMap;
};

#endif // TOWN_H
