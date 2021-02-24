#include "town.h"

#include "hero_functions.h"

#include "events/event_manager.h"
#include "events/event_battle.h"
#include "events/event_arrival_at_town.h"

#include "interfaces/i_hero.h"
#include "interfaces/i_unit.h"
#include "interfaces/i_unit_factory.h"

Town::Town(Race _race, const std::string &_name, const std::shared_ptr<IUnitFactory>& _unitFactory)
    : m_unitFactory(_unitFactory)
    , m_race(_race)
    , m_name(_name)
{
    m_weeklyGrowAmount = {
        {UnitLevel::Level_1, 8},
        {UnitLevel::Level_2, 4},
        {UnitLevel::Level_3, 2},
        {UnitLevel::Level_4, 1}
    };

    m_recruitsMap = {
        {UnitLevel::Level_1, m_unitFactory->createUnit(m_race, UnitLevel::Level_1, m_weeklyGrowAmount[UnitLevel::Level_1])},
        {UnitLevel::Level_2, m_unitFactory->createUnit(m_race, UnitLevel::Level_2, m_weeklyGrowAmount[UnitLevel::Level_2])},
        {UnitLevel::Level_3, m_unitFactory->createUnit(m_race, UnitLevel::Level_3, m_weeklyGrowAmount[UnitLevel::Level_3])},
        {UnitLevel::Level_4, m_unitFactory->createUnit(m_race, UnitLevel::Level_4, m_weeklyGrowAmount[UnitLevel::Level_4])}
    };
}

const std::string &Town::name() const
{
    return m_name;
}

IPlayerShared Town::controllingPlayer()
{
    return m_controllingPlayer.lock();
}

IPlayerSharedConst Town::controllingPlayer() const
{
    return m_controllingPlayer.lock();
}

void Town::setControllingPLayer(IPlayerShared &_controllingPLayer)
{
    m_controllingPlayer = _controllingPLayer;
}

void Town::freeFromControllingPlayer()
{
    m_controllingPlayer.reset();
}

void Town::interact(IHeroShared &_hero, IInteractObjShared &_selfShared)
{
    if (!_hero)
        return;

    auto changeOwner = [this, _selfShared](IPlayerShared _newOwner)
    {
        auto townPtr = std::dynamic_pointer_cast<ITown>(_selfShared);

        if (auto prevOwner = m_controllingPlayer.lock())
            prevOwner->abandonTown(townPtr);

        m_controllingPlayer = _newOwner;
        if (_newOwner)
            _newOwner->captureTown(townPtr);
    };

    if (_hero->controllingPlayer() != m_controllingPlayer.lock())
    {
        // Battle or free taking
        if (auto defender = m_heroInside.lock())
        {
            // Creating battle event
            auto battleEvent = std::make_shared<EventBattle>(_hero, // Attacker
                                                             defender, // Defender
                                                             [this, _hero, defender, _selfShared, changeOwner]()
            {
                // Function will be called after battle is over

                if (_hero && _hero->isDefeated())
                    HeroFunctions::onHeroDefeat(_hero);

                if (defender && defender->isDefeated())
                {
                    HeroFunctions::onHeroDefeat(defender);

                    // Town is lost, changing owner
                    m_heroInside.reset();
                    if (_hero && !_hero->isDefeated())
                        changeOwner(_hero->controllingPlayer());
                    else
                        m_controllingPlayer.reset();
                }
            });

            EventManager::getInstance().addEventToQueue(std::move(battleEvent));
        }
        else
        {
            // Town is taken
            changeOwner(_hero->controllingPlayer());
        }
    }
    else
    {
        // Town management
        auto town = std::dynamic_pointer_cast<ITown>(_selfShared);
        if (!town)
            return;

        auto battleEvent = std::make_shared<EventArrivalAtTown>(town, _hero);
        EventManager::getInstance().addEventToQueue(std::move(battleEvent));
    }
}

std::string Town::title() const
{
    std::string title;
    title = Convert::raceToText(m_race) + " town";
    return title;
}

void Town::acceptHero(IHeroShared &_hero)
{
    m_heroInside = _hero;
}

IHeroShared Town::garrisonHero()
{
    return m_heroInside.lock();
}

void Town::heroLeaved()
{
    m_heroInside.reset();
}

void Town::processWeeklyGrow()
{
    for (auto& mapIt : m_recruitsMap)
    {
        auto level = mapIt.first;
        auto& recruitUnit = mapIt.second;

        recruitUnit->acceptUnitInParty(m_weeklyGrowAmount[level]);
    }
}

const std::map<UnitLevel, IUnitShared> &Town::getRecruitsMap()
{
    return m_recruitsMap;
}

IUnitShared Town::getRecruitUnit(UnitLevel _level)
{
    return m_recruitsMap[_level];
}
