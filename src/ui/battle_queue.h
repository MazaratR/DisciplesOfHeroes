#ifndef BATTLE_QUEUE_H
#define BATTLE_QUEUE_H

#include <QWidget>

#include "units/unit_types.h"

#include "forward/battle_queue_element_forward.h"

class UnitLayout;

namespace Ui {
class BattleQueue;
}

class BattleQueue : public QWidget
{
    Q_OBJECT

public:
    explicit BattleQueue(QWidget *parent = nullptr);
    ~BattleQueue();

    void setUnitsPixmap(const std::map<UnitType, QPixmap>*_map);

    void render(const BattleQueueSharedVec& _queue);

private:
    Ui::BattleQueue *ui;

    std::vector<UnitLayout*> m_layouts;
};

#endif // BATTLE_QUEUE_H
