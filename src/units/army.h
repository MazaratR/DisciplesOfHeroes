#ifndef ARMY_H
#define ARMY_H

#include "unit_position.h"
#include "unit_types.h"
#include "forward/i_unit_forward.h"

class Army
{
public:
    Army() = default;
    ~Army() = default;

    IUnitShared&     getUnit(UnitPosition _position);
    IUnitSharedConst getUnit(UnitPosition _position) const;
    IUnitShared getUnit(UnitType _type);
    IUnitSharedConst getUnit(UnitType _type) const;
    IUnitSharedVec getArmyVec();
    IUnitSharedConstVec getArmyVec() const;

    void setUnit(UnitPosition _position, const IUnitShared &_unit);
    void getRidOfDeadBodies();

    bool hasUnit(const IUnitSharedConst &_unit) const;
    UnitPosition getFreeArmyPosition() const;
    UnitPosition getUnitPosition(const IUnitSharedConst &_unit) const;
    void swapUnits(IUnitSharedConst _first, IUnitSharedConst _second);

private:
    int getIndexFromPosition(UnitPosition _position) const;
    UnitPosition getPositionFromIndex(int _index) const;

private:
    static constexpr int ARMY_SIZE = 4;
    IUnitShared m_army[ARMY_SIZE];
};

#endif // ARMY_H
