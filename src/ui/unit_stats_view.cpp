#include "unit_stats_view.h"
#include "ui_unit_stats_view.h"

#include <QString>

#include "interfaces/i_unit.h"

UnitStatsView::UnitStatsView(QWidget *parent,
                             const IUnitSharedConst &_unit)
    : QWidget(parent)
    , ui(new Ui::UnitStatsView)
{
    ui->setupUi(this);

    updateView(_unit);
}

UnitStatsView::~UnitStatsView()
{
    delete ui;
}

void UnitStatsView::updateView(const IUnitSharedConst &_unit)
{
    ui->lbl_unitName->setText(_unit->name().c_str());

    const auto& stats = _unit->stats();
    ui->lbl_armor->setText(QString::number(stats.armor));
    ui->lbl_damage_melee->setText(QString::number(stats.damageMelee.min) + " - " +
                                  QString::number(stats.damageMelee.max));
    ui->lbl_damage_range->setText(QString::number(stats.damageRange.min) + " - " +
                                  QString::number(stats.damageRange.max));
    ui->lbl_ammo->setText(QString::number(stats.ammo.current) + " / " +
                          QString::number(stats.ammo.max));
    ui->lbl_health->setText(QString::number(stats.health.current) + " / " +
                            QString::number(stats.health.max));
    ui->lbl_speed->setText(QString::number(stats.speed));
}
