#include "battle_queue.h"
#include "ui_battle_queue.h"
#include "battle_queue_element.h"

BattleQueue::BattleQueue(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::BattleQueue)
{
    ui->setupUi(this);

    ui->wgt_unitInQueue_1->showAmount(false);
    ui->wgt_unitInQueue_2->showAmount(false);
    ui->wgt_unitInQueue_3->showAmount(false);
    ui->wgt_unitInQueue_4->showAmount(false);
    ui->wgt_unitInQueue_5->showAmount(false);
    ui->wgt_unitInQueue_6->showAmount(false);
    ui->wgt_unitInQueue_7->showAmount(false);
    ui->wgt_unitInQueue_8->showAmount(false);

    m_layouts = {
        ui->wgt_unitInQueue_1,
        ui->wgt_unitInQueue_2,
        ui->wgt_unitInQueue_3,
        ui->wgt_unitInQueue_4,
        ui->wgt_unitInQueue_5,
        ui->wgt_unitInQueue_6,
        ui->wgt_unitInQueue_7,
        ui->wgt_unitInQueue_8
    };
}

BattleQueue::~BattleQueue()
{
    delete ui;
}

void BattleQueue::setUnitsPixmap(const std::map<UnitType, QPixmap> *_map)
{
    for (auto layout : m_layouts)
        layout->setUnitsPixmap(_map);
}

void BattleQueue::render(const BattleQueueSharedVec &_queue)
{
    for (auto layout : m_layouts)
        layout->clear();

    unsigned i = 0;
    for (const auto& queueELem : _queue)
    {
        if (i >= m_layouts.size())
            break;

        if (queueELem->isDead())
            continue;
        if (queueELem->actedOnThisTurn)
            continue;
        if (auto unit = queueELem->unit.lock())
            m_layouts.at(i)->setRenderData(unit);

        i++;
    }
}
