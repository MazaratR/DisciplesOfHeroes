#ifndef TOWN_VIEW_H
#define TOWN_VIEW_H

#include <QWidget>

#include "units/unit_types.h"
#include "units/unit_levels.h"

#include "forward/i_hero_forward.h"
#include "forward/i_town_forward.h"
#include "forward/i_unit_forward.h"

class ITown;

namespace Ui {
class TownView;
}

class TownView : public QWidget
{
    Q_OBJECT

public:
    explicit TownView(QWidget *parent = nullptr);
    ~TownView();

    void setUnitsPixmap(const std::map<UnitType, QPixmap>*_map);
    void setDisplayedTown(ITownShared& _town);
    void setArrivedHero(IHeroShared& _hero);

    void renderView();
    void resetUi();

private slots:
    void on_pb_exitTown_clicked();
    void on_pb_stayAsGarrison_clicked();

    void onHireUnit(UnitLevel _level, int _amount);

signals:
    void exitFromTown();
    void heroStayAsGarrison(ITownShared _town, IHeroShared _hero);
    void hireUnitInTown(ITownShared _town, IHeroShared _hero,
                        UnitLevel _level,  int _amount);
    void swapUnits(IUnitSharedConst _first, IUnitSharedConst _second);
    void showUnitInfo(IUnitSharedConst _unit, int _x, int _y);
    void stopShowUnitInfo();

private:
    Ui::TownView *ui;

    ITownShared m_displayedTown;
    IHeroShared m_arrivedHero;
};

#endif // TOWN_VIEW_H
