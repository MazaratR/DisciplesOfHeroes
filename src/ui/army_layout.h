#ifndef ARMY_LAYOUT_H
#define ARMY_LAYOUT_H

#include <QWidget>
#include <map>

#include "units/unit_types.h"
#include "units/unit_position.h"
#include "units/unit_levels.h"

#include "forward/i_hero_forward.h"
#include "forward/i_unit_forward.h"
#include "forward/battle_queue_element_forward.h"

class UnitLayout;

namespace Ui {
class ArmyLayout;
}

class ArmyLayout : public QWidget
{
    Q_OBJECT

public:
    explicit ArmyLayout(QWidget *parent = nullptr);
    ~ArmyLayout();

    bool eventFilter(QObject* _obj, QEvent* _event) override;

    void setUnitsPixmap(const std::map<UnitType, QPixmap>*_map);
    void setTitle(QString _title);

    void showHireUi(bool _show = true);
    void showAmount(bool _show = true);
    void hideEmptyUnits(bool _hide = true);
    void setBattleMode(bool _battle = true);

    void configurateAsLeftSided();
    void configurateAsRightSided();

    void resetHighlight();

    void setRenderData(const IHeroSharedConst &_hero,
                const BattleQueueElemShared& _unitToHighlight = BattleQueueElemShared(),
                bool _mirroredUnits = false);

    void setRenderData(const IUnitSharedVec& _amryVec, bool _mirroredUnits = false);
    void setRenderData(const std::map<UnitLevel, IUnitShared> & _unitsMap);
    void displayDamage(uint16_t _damage, UnitPosition _position);
    void displayHeal(uint16_t _healAmont, UnitPosition _position);

    void resetHireUi();

private:
    UnitPosition getUnitPosByLayout(UnitLayout* _layout) const;
    UnitLayout* getLayoutByPos(UnitPosition _pos) const;
    IUnitSharedConst getDisplayedUnit(UnitPosition _pos) const;

private slots:
    void onHireUnit(int _amount);

signals:
    void attackUnit(UnitPosition);
    void showUnitInfo(IUnitSharedConst _unit, int _x, int _y);
    void stopShowUnitInfo();

    void hireUnit(UnitLevel _level,int _amount);
    void swapUnits(IUnitSharedConst _first, IUnitSharedConst _second);

private:
    Ui::ArmyLayout *ui;

    bool m_battleMode = false; // for different process of mouse click on units
    UnitPosition m_selectedPosition = UnitPosition::Unknown;

    std::map<UnitPosition, UnitLayout*> m_unitLayoutsByPos;
    std::map<UnitLevel, UnitLayout*> m_unitLayoutsByLevel;
};

#endif // ARMY_LAYOUT_H
