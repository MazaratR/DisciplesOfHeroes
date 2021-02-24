#include "player.h"

#include "interfaces/i_hero.h"
#include "interfaces/i_heroes_manager.h"
#include "interfaces/i_town.h"

Player::Player(uint8_t _id,
               ControlSide _controlSide,
               Race _race,
               Color _color)
    : m_id(_id)
    , m_control(_controlSide)
    , m_race(_race)
    , m_color(_color)
{
    m_resourceMap = {
        {ResourceType::Gold,  1000},
        {ResourceType::Wood,  5},
        {ResourceType::Stone, 5},
        {ResourceType::Gem,   1}
    };

}

Race Player::race() const
{
    return m_race;
}

void Player::setRace(const Race &_race)
{
    m_race = _race;
}

Color Player::color() const
{
    return m_color;
}

void Player::setColor(const Color &_color)
{
    m_color = _color;
}

ControlSide Player::controlSide() const
{
    return m_control;
}

void Player::setControl(const ControlSide &_controlSide)
{
    m_control = _controlSide;
}

bool Player::isHumanControl() const
{
    return m_control == ControlSide::Human;
}

uint8_t Player::id() const
{
    return m_id;
}

void Player::setId(const uint8_t &_id)
{
    m_id = _id;
}

void Player::surrenderAllTowns()
{
    for (auto& town : m_townsVec)
        town->freeFromControllingPlayer();
}

void Player::addHero(IHeroShared &_hero)
{
    m_heroesVec.push_back(_hero);
}

IHeroShared Player::getHero(const std::string &_name)
{
    for (const auto& heroWeakPtr : m_heroesVec)
    {
        if (auto hero = heroWeakPtr.lock())
        {
            if (hero->name() == _name)
                return hero;
        }
    }

    return IHeroShared();
}

IHeroSharedConst Player::getHero(const std::string &_name) const
{
    for (const auto& heroWeakPtr : m_heroesVec)
    {
        if (auto hero = heroWeakPtr.lock())
        {
            if (hero->name() == _name)
                return hero;
        }
    }

    return IHeroSharedConst();
}

IHeroSharedVec Player::getHeroes()
{
    IHeroSharedVec retvalVec;
    retvalVec.reserve(m_heroesVec.size());

    for (const auto& heroWeak : m_heroesVec)
    {
        if (auto hero = heroWeak.lock())
            retvalVec.emplace_back(hero);
    }

    return retvalVec;
}

IHeroSharedConstVec Player::getHeroes() const
{
    IHeroSharedConstVec retvalVec;
    retvalVec.reserve(m_heroesVec.size());

    for (const auto& heroWeak : m_heroesVec)
    {
        if (auto hero = heroWeak.lock())
            retvalVec.emplace_back(hero);
    }

    return retvalVec;
}

void Player::removeHero(IHeroShared &_hero)
{
    for (unsigned i = 0 ; i < m_heroesVec.size(); i++)
    {
        auto heroWeakPtr = m_heroesVec.at(i);
        if (auto hero = heroWeakPtr.lock())
        {
            if (hero == _hero)
            {
                m_heroesVec.erase(m_heroesVec.begin() + i);
                return;
            }
        }
    }
}

bool Player::hasUndefeatedHero() const
{
    auto heroesVec = getHeroes();
    for (const auto& hero : heroesVec)
    {
        if (!hero->getArmy().empty())
            return true;
    }

    return false;
}

int Player::resourceAmount(ResourceType _type) const
{
    auto it = m_resourceMap.find(_type);
    if (it == m_resourceMap.end())
        return 0;
    else
        return it->second;
}

void Player::acceptResource(ResourceType _type, int _value)
{
    m_resourceMap[_type] += _value;
}

ResourceMap &Player::getResourceMap()
{
    return m_resourceMap;
}

const ResourceMap &Player::getResourceMap() const
{
    return m_resourceMap;
}

void Player::captureTown(ITownShared _town)
{
    m_townsVec.emplace_back(_town);
}

void Player::abandonTown(ITownShared _town)
{
    size_t i = 0;
    for (auto& town : m_townsVec)
    {
        if (town == _town)
        {
            m_townsVec.erase(m_townsVec.begin() + i);
            break;
        }
        i++;
    }
}

ITownSharedVec Player::getTowns()
{
    return m_townsVec;
}

ITownShared Player::getTownByGarrison(const IHeroShared &_hero)
{
    for (auto& town : m_townsVec)
    {
        if (town->garrisonHero() == _hero)
            return town;
    }

    return ITownShared();
}

ITownSharedConst Player::getTownByGarrison(const IHeroShared &_hero) const
{
    for (auto& town : m_townsVec)
    {
        if (town->garrisonHero() == _hero)
            return town;
    }

    return ITownSharedConst();
}
