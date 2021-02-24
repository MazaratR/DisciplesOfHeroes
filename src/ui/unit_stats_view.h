#ifndef UNIT_STATS_VIEW_H
#define UNIT_STATS_VIEW_H

#include <QWidget>

#include "forward/i_unit_forward.h"

namespace Ui {
class UnitStatsView;
}

class UnitStatsView : public QWidget
{
    Q_OBJECT

public:
    explicit UnitStatsView(QWidget *parent,
                           const IUnitSharedConst& _unit);
    ~UnitStatsView();

    void updateView(const IUnitSharedConst& _unit);

private:
    Ui::UnitStatsView *ui;
};

#endif // UNIT_STATS_VIEW_H
