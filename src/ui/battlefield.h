#ifndef BATTLEFIELD_H
#define BATTLEFIELD_H

#include <QWidget>

#include "units/unit_types.h"
#include "units/unit_position.h"

#include "forward/i_hero_forward.h"
#include "forward/i_unit_forward.h"
#include "forward/battle_queue_element_forward.h"

struct AttackResult;

namespace Ui {
class Battlefield;
}

class Battlefield : public QWidget
{
    Q_OBJECT

public:
    explicit Battlefield(QWidget *parent = nullptr);
    ~Battlefield();

    void setParticipants(IHeroSharedConst _heroOnLeft, IHeroSharedConst _heroOnRight);
    void setUnitsPixmap(const std::map<UnitType, QPixmap>*_map);
    void setTitles(const QString &_armyOnLeft, const QString &_armyOnRight);

    void setRenderData(const BattleQueueSharedVec& _queue);
    void renderAttackResult(const AttackResult& _result);

signals:
    void skipMove();
    void surrender();
    void attackUnitOnRight(UnitPosition);
    void attackUnitOnLeft(UnitPosition);
    void showUnitInfo(IUnitSharedConst _unit, int _x, int _y);
    void stopShowUnitInfo();

private:
    Ui::Battlefield *ui;

    IHeroSharedConst m_heroOnLeft;
    IHeroSharedConst m_heroOnRight;
};

#endif // BATTLEFIELD_H
