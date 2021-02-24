#include "town_view.h"
#include "ui_town_view.h"

#include "interfaces/i_player.h"
#include "interfaces/i_hero.h"
#include "interfaces/i_town.h"

TownView::TownView(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TownView)
{
    ui->setupUi(this);

    ui->wgt_townArmy->showAmount();
    ui->wgt_townArmy->showHireUi();
    ui->wgt_townArmy->configurateAsRightSided();
    ui->wgt_townArmy->hideEmptyUnits(false);

    connect(ui->wgt_townArmy, &ArmyLayout::hireUnit,    this, &TownView::onHireUnit);
    connect(ui->wgt_heroArmy, &ArmyLayout::swapUnits,   this, &TownView::swapUnits);

    connect(ui->wgt_heroArmy, &ArmyLayout::showUnitInfo,  this, &TownView::showUnitInfo);
    connect(ui->wgt_heroArmy, &ArmyLayout::stopShowUnitInfo,  this, &TownView::stopShowUnitInfo);
    connect(ui->wgt_townArmy, &ArmyLayout::showUnitInfo,  this, &TownView::showUnitInfo);
    connect(ui->wgt_townArmy, &ArmyLayout::stopShowUnitInfo,  this, &TownView::stopShowUnitInfo);
}

TownView::~TownView()
{
    delete ui;
}

void TownView::setUnitsPixmap(const std::map<UnitType, QPixmap> *_map)
{
    ui->wgt_heroArmy->setUnitsPixmap(_map);
    ui->wgt_townArmy->setUnitsPixmap(_map);
}

void TownView::setDisplayedTown(ITownShared &_town)
{
    m_displayedTown = _town;

    if (!m_displayedTown)
        return;

    ui->wgt_townArmy->setTitle(m_displayedTown->name().c_str());
}

void TownView::setArrivedHero(IHeroShared &_hero)
{
    m_arrivedHero = _hero;

    if (!m_arrivedHero)
        return;

    ui->wgt_heroArmy->setTitle(m_arrivedHero->name().c_str());
}

void TownView::renderView()
{
    if (!m_arrivedHero)
        return;
    if (!m_displayedTown)
        return;

    ui->wgt_heroArmy->setRenderData(m_arrivedHero);

    ui->wgt_townArmy->setRenderData(m_displayedTown->getRecruitsMap());

    if (auto player = m_arrivedHero->controllingPlayer())
        ui->wgt_heroResources->updateResourceAmount(player->getResourceMap());

    ui->pb_stayAsGarrison->setEnabled(!m_displayedTown->garrisonHero());
}

void TownView::resetUi()
{
    ui->wgt_townArmy->resetHireUi();
}

void TownView::on_pb_exitTown_clicked()
{
    m_arrivedHero.reset();
    m_displayedTown.reset();
    emit exitFromTown();
}

void TownView::on_pb_stayAsGarrison_clicked()
{
    emit heroStayAsGarrison(m_displayedTown, m_arrivedHero);
}

void TownView::onHireUnit(UnitLevel _level, int _amount)
{
    emit hireUnitInTown(m_displayedTown, m_arrivedHero, _level, _amount);
}
