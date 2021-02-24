#include "battlefield.h"
#include "ui_battlefield.h"

#include "attack_result.h"

#include "interfaces/i_hero.h"

Battlefield::Battlefield(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Battlefield)
{
    ui->setupUi(this);

    ui->wgt_armyOnLeft->configurateAsLeftSided();
    ui->wgt_armyOnRight->configurateAsRightSided();

    ui->wgt_armyOnLeft->setBattleMode();
    ui->wgt_armyOnRight->setBattleMode();

    connect(ui->pb_skip,      &QPushButton::clicked, this, &Battlefield::skipMove);
    connect(ui->pb_surrender, &QPushButton::clicked, this, &Battlefield::surrender);

    connect(ui->wgt_armyOnLeft, &ArmyLayout::attackUnit,        this, &Battlefield::attackUnitOnLeft);
    connect(ui->wgt_armyOnLeft, &ArmyLayout::showUnitInfo,      this, &Battlefield::showUnitInfo);
    connect(ui->wgt_armyOnLeft, &ArmyLayout::stopShowUnitInfo,  this, &Battlefield::stopShowUnitInfo);

    connect(ui->wgt_armyOnRight, &ArmyLayout::attackUnit,       this, &Battlefield::attackUnitOnRight);
    connect(ui->wgt_armyOnRight, &ArmyLayout::showUnitInfo,     this, &Battlefield::showUnitInfo);
    connect(ui->wgt_armyOnRight, &ArmyLayout::stopShowUnitInfo, this, &Battlefield::stopShowUnitInfo);
}

Battlefield::~Battlefield()
{
    delete ui;
}

void Battlefield::setParticipants(IHeroSharedConst _heroOnLeft, IHeroSharedConst _heroOnRight)
{
    m_heroOnLeft = _heroOnLeft;
    m_heroOnRight = _heroOnRight;

    ui->wgt_armyOnLeft->setTitle(m_heroOnLeft->name().c_str());
    ui->wgt_armyOnRight->setTitle(m_heroOnRight->name().c_str());

    ui->wgt_armyOnLeft->resetHighlight();
    ui->wgt_armyOnRight->resetHighlight();
}

void Battlefield::setUnitsPixmap(const std::map<UnitType, QPixmap>* _map)
{
    ui->wgt_armyOnLeft->setUnitsPixmap(_map);
    ui->wgt_armyOnRight->setUnitsPixmap(_map);
    ui->wgt_battleQueue->setUnitsPixmap(_map);
}

void Battlefield::setTitles(const QString& _armyOnLeft, const QString& _armyOnRight)
{
    ui->wgt_armyOnLeft->setTitle(_armyOnLeft);
    ui->wgt_armyOnRight->setTitle(_armyOnRight);
}

void Battlefield::setRenderData(const BattleQueueSharedVec &_queue)
{
    BattleQueueElemShared currentUnit;
    if (_queue.size())
        currentUnit = _queue.front();

    ui->wgt_armyOnLeft->setRenderData(m_heroOnLeft, currentUnit);
    ui->wgt_armyOnRight->setRenderData(m_heroOnRight, currentUnit, true);
    ui->wgt_battleQueue->render(_queue);
}

void Battlefield::renderAttackResult(const AttackResult &_result)
{
    auto renderResult = [](ArmyLayout* _armyLayout, const IHeroSharedConst& _hero, const AttackData& _data)
    {
        UnitPosition position = _hero->getUnitPosition(_data.unit);
        if (position != UnitPosition::Unknown)
        {
            if (_data.damageTaken)
                _armyLayout->displayDamage(_data.damageTaken, position);
            if (_data.healAmount)
                _armyLayout->displayHeal(_data.healAmount, position);
        }
    };

    renderResult(ui->wgt_armyOnLeft,  m_heroOnLeft,  _result.defender);
    renderResult(ui->wgt_armyOnLeft,  m_heroOnLeft,  _result.attacker);
    renderResult(ui->wgt_armyOnRight, m_heroOnRight, _result.defender);
    renderResult(ui->wgt_armyOnRight, m_heroOnRight, _result.attacker);
}
