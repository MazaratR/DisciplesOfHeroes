#include "army.h"

#include "interfaces/i_unit.h"

IUnitShared &Army::getUnit(UnitPosition _position)
{
    return m_army[getIndexFromPosition(_position)];
}

IUnitSharedConst Army::getUnit(UnitPosition _position) const
{
    return m_army[getIndexFromPosition(_position)];
}

IUnitShared Army::getUnit(UnitType _type)
{
    for (auto& unit : m_army)
    {
        if (unit && (unit->type() == _type))
            return unit;
    }
    return IUnitShared();
}

IUnitSharedConst Army::getUnit(UnitType _type) const
{
    for (const auto& unit : m_army)
    {
        if (unit && (unit->type() == _type))
            return unit;
    }
    return IUnitSharedConst();
}

IUnitSharedVec Army::getArmyVec()
{
    IUnitSharedVec armyVec;

    for (const auto& unit : m_army)
    {
        if (unit)
            armyVec.push_back(unit);
    }

    return armyVec;
}

IUnitSharedConstVec Army::getArmyVec() const
{
    IUnitSharedConstVec armyVec;

    for (const auto& unit : m_army)
    {
        if (unit)
            armyVec.push_back(unit);
    }

    return armyVec;
}

void Army::setUnit(UnitPosition _position, const IUnitShared& _unit)
{
    auto& unitInArmy = m_army[getIndexFromPosition(_position)];
    if (!unitInArmy)
        unitInArmy = _unit;
    else if (unitInArmy->type() == _unit->type())
        unitInArmy->acceptUnitInParty(_unit->partySize());
}

void Army::getRidOfDeadBodies()
{
    size_t i = 0;
    for (const auto& unit : m_army)
    {
        if (unit && unit->partySize() < 1)
            m_army[i].reset();

        i++;
    }
}

bool Army::hasUnit(const IUnitSharedConst &_unit) const
{
    return getUnitPosition(_unit) != UnitPosition::Unknown;
}

UnitPosition Army::getFreeArmyPosition() const
{
    for (int i = 0; i < ARMY_SIZE; i++)
    {
        if (!m_army[i])
            return static_cast<UnitPosition>(i);
    }

    return UnitPosition::Unknown;
}

UnitPosition Army::getUnitPosition(const IUnitSharedConst &_unit) const
{
    size_t i = 0;
    for (const auto& unit : m_army)
    {
        if (unit == _unit)
            return getPositionFromIndex(i);

        i++;
    }

    return UnitPosition::Unknown;
}

void Army::swapUnits(IUnitSharedConst _first, IUnitSharedConst _second)
{
    auto firstPos = getUnitPosition(_first);
    auto secondPos = getUnitPosition(_second);

    auto firstUnitNonConst = getUnit(firstPos);
    auto secondUnitNonConst = getUnit(secondPos);

    if ((firstPos != UnitPosition::Unknown) &&
        (secondPos != UnitPosition::Unknown))
    {
        m_army[getIndexFromPosition(firstPos)] = secondUnitNonConst;
        m_army[getIndexFromPosition(secondPos)] = firstUnitNonConst;
    }
}

int Army::getIndexFromPosition(UnitPosition _position) const
{
    return static_cast<int>(_position);
}

UnitPosition Army::getPositionFromIndex(int _index) const
{
    return static_cast<UnitPosition>(_index);
}
