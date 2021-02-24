#include "army_layout.h"
#include "ui_army_layout.h"

#include <QMouseEvent>

#include "battle_queue_element.h"

#include "interfaces/i_hero.h"
#include "interfaces/i_unit.h"

ArmyLayout::ArmyLayout(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ArmyLayout)
{
    ui->setupUi(this);
    ui->lbl_title->setText("");

    configurateAsLeftSided();

    ui->wgt_unit_1_1->installEventFilter(this);
    ui->wgt_unit_2_1->installEventFilter(this);
    ui->wgt_unit_1_2->installEventFilter(this);
    ui->wgt_unit_2_2->installEventFilter(this);

    connect(ui->wgt_unit_1_1, &UnitLayout::hireUnit, this, &ArmyLayout::onHireUnit);
    connect(ui->wgt_unit_2_1, &UnitLayout::hireUnit, this, &ArmyLayout::onHireUnit);
    connect(ui->wgt_unit_1_2, &UnitLayout::hireUnit, this, &ArmyLayout::onHireUnit);
    connect(ui->wgt_unit_2_2, &UnitLayout::hireUnit, this, &ArmyLayout::onHireUnit);
}

ArmyLayout::~ArmyLayout()
{
    delete ui;
}

bool ArmyLayout::eventFilter(QObject *_obj, QEvent *_event)
{
    if (_obj->metaObject()->className() == ui->wgt_unit_1_1->metaObject()->className())
    {
        if (_event->type() == QEvent::MouseButtonRelease)
        {
            auto event = dynamic_cast<QMouseEvent*>(_event);
            UnitPosition unitPos = getUnitPosByLayout(dynamic_cast<UnitLayout*>(_obj));

            if (event->button() == Qt::LeftButton)
            {
                if (m_battleMode)
                {
                    emit attackUnit(unitPos);
                }
                else
                {
                    if (m_selectedPosition == UnitPosition::Unknown)
                    {
                        // First selection
                        m_selectedPosition = unitPos;
                    }
                    else
                    {
                        // Second selection
                        auto first  = getDisplayedUnit(m_selectedPosition);
                        auto second = getDisplayedUnit(unitPos);
                        emit swapUnits(first, second);
                        m_selectedPosition = UnitPosition::Unknown;
                    }
                }

                return true;
            }
            else if (event->button() == Qt::RightButton)
            {
                emit stopShowUnitInfo();
                return true;
            }
        }
        else if (_event->type() == QEvent::MouseButtonPress)
        {
            auto event = dynamic_cast<QMouseEvent*>(_event);
            UnitPosition unitPos = getUnitPosByLayout(dynamic_cast<UnitLayout*>(_obj));

            if (event->button() == Qt::RightButton)
            {
                auto unit = getDisplayedUnit(unitPos);
                emit showUnitInfo(unit, QCursor::pos().x(), QCursor::pos().y());
                return true;
            }
        }
    }

    return false;
}

void ArmyLayout::setUnitsPixmap(const std::map<UnitType, QPixmap>* _map)
{
    ui->wgt_unit_1_1->setUnitsPixmap(_map);
    ui->wgt_unit_2_1->setUnitsPixmap(_map);
    ui->wgt_unit_1_2->setUnitsPixmap(_map);
    ui->wgt_unit_2_2->setUnitsPixmap(_map);
}

void ArmyLayout::setTitle(QString _title)
{
    ui->lbl_title->setText(_title);
}

void ArmyLayout::showHireUi(bool _show)
{
    ui->wgt_unit_1_1->showHireUi(_show);
    ui->wgt_unit_2_1->showHireUi(_show);
    ui->wgt_unit_1_2->showHireUi(_show);
    ui->wgt_unit_2_2->showHireUi(_show);
}

void ArmyLayout::showAmount(bool _show)
{
    ui->wgt_unit_1_1->showAmount(_show);
    ui->wgt_unit_2_1->showAmount(_show);
    ui->wgt_unit_1_2->showAmount(_show);
    ui->wgt_unit_2_2->showAmount(_show);
}

void ArmyLayout::hideEmptyUnits(bool _hide)
{
    ui->wgt_unit_1_1->hideEmptyUnits(_hide);
    ui->wgt_unit_2_1->hideEmptyUnits(_hide);
    ui->wgt_unit_1_2->hideEmptyUnits(_hide);
    ui->wgt_unit_2_2->hideEmptyUnits(_hide);
}

void ArmyLayout::setBattleMode(bool _battle)
{
    m_battleMode = _battle;
}

void ArmyLayout::configurateAsLeftSided()
{
    m_unitLayoutsByPos = {
            {UnitPosition::RearguardTop,    ui->wgt_unit_1_1},
            {UnitPosition::RearguardBottom, ui->wgt_unit_2_1},
            {UnitPosition::VanguardTop,     ui->wgt_unit_1_2},
            {UnitPosition::VanguardBottom,  ui->wgt_unit_2_2}
        };

    m_unitLayoutsByLevel = {
            {UnitLevel::Level_1, ui->wgt_unit_1_1},
            {UnitLevel::Level_2, ui->wgt_unit_2_1},
            {UnitLevel::Level_3, ui->wgt_unit_1_2},
            {UnitLevel::Level_4, ui->wgt_unit_2_2}
    };
}

void ArmyLayout::configurateAsRightSided()
{
    m_unitLayoutsByPos = {
            {UnitPosition::RearguardTop,    ui->wgt_unit_1_2},
            {UnitPosition::RearguardBottom, ui->wgt_unit_2_2},
            {UnitPosition::VanguardTop,     ui->wgt_unit_1_1},
            {UnitPosition::VanguardBottom,  ui->wgt_unit_2_1}
    };

    m_unitLayoutsByLevel = {
            {UnitLevel::Level_1, ui->wgt_unit_1_2},
            {UnitLevel::Level_2, ui->wgt_unit_2_2},
            {UnitLevel::Level_3, ui->wgt_unit_1_1},
            {UnitLevel::Level_4, ui->wgt_unit_2_1}
    };
}

void ArmyLayout::resetHighlight()
{
    for (auto& mapIt : m_unitLayoutsByPos)
    {
        auto layout = mapIt.second;
        layout->setHighlightType(UnitHighlightType::None);
    }
}

void ArmyLayout::setRenderData(const IHeroSharedConst& _hero,
                        const BattleQueueElemShared& _unitToHighlight,
                        bool _mirroredUnits)
{
    // Clear all layouts
    for (auto& it : m_unitLayoutsByPos)
        it.second->clear();

    if (!_hero)
        return;

    for (const auto& it : m_unitLayoutsByPos)
    {
        auto pos = it.first;
        auto wgt_layout = it.second;

        auto unit = _hero->getUnit(pos);
        bool highlight = _unitToHighlight ? unit == _unitToHighlight->unit.lock() // Battle selection
                                          : m_selectedPosition == pos;            // Other selection
        wgt_layout->setRenderData(unit, highlight, _mirroredUnits);
    }
}

void ArmyLayout::setRenderData(const IUnitSharedVec &_amryVec, bool _mirroredUnits)
{
    size_t i = 0;
    for (const auto& unit : _amryVec)
    {
        if (i >= m_unitLayoutsByPos.size())
            return;

        m_unitLayoutsByPos[static_cast<UnitPosition>(i++)]->setRenderData(unit, false, _mirroredUnits);
    }
}

void ArmyLayout::setRenderData(const std::map<UnitLevel, IUnitShared> &_unitsMap)
{
    for (const auto& mapIt : _unitsMap)
    {
        auto level = mapIt.first;
        const auto& unit = mapIt.second;

        auto layout = m_unitLayoutsByLevel[level];
        if (layout)
            layout->setRenderData(unit);
    }
}

void ArmyLayout::displayDamage(uint16_t _damage, UnitPosition _position)
{
    auto layout = m_unitLayoutsByPos[_position];
    layout->setHighlightType(UnitHighlightType::Damage);
    layout->setDisplayAmount(_damage);
}

void ArmyLayout::displayHeal(uint16_t _healAmont, UnitPosition _position)
{
    auto layout = m_unitLayoutsByPos[_position];
    layout->setHighlightType(UnitHighlightType::Heal);
    layout->setDisplayAmount(_healAmont);
}

void ArmyLayout::resetHireUi()
{
    ui->wgt_unit_1_1->resetHireUi();
    ui->wgt_unit_2_1->resetHireUi();
    ui->wgt_unit_1_2->resetHireUi();
    ui->wgt_unit_2_2->resetHireUi();
}

UnitPosition ArmyLayout::getUnitPosByLayout(UnitLayout *_layout) const
{
    if (!_layout)
        return UnitPosition::Unknown;

    for (const auto& it : m_unitLayoutsByPos)
    {
        if (it.second == _layout)
            return it.first;
    }

    return UnitPosition::Unknown;
}

UnitLayout *ArmyLayout::getLayoutByPos(UnitPosition _pos) const
{
    auto it = m_unitLayoutsByPos.find(_pos);
    if (it != m_unitLayoutsByPos.end())
        return it->second;
    else
        return nullptr;
}

IUnitSharedConst ArmyLayout::getDisplayedUnit(UnitPosition _pos) const
{
    auto layout = getLayoutByPos(_pos);
    if (!layout)
        return IUnitSharedConst();
    else
        return layout->displayedUnit();
}

void ArmyLayout::onHireUnit(int _amount)
{
    auto sender = QObject::sender();
    if (!sender)
        return;

    auto level = UnitLevel::Unknown;
    for (const auto& it : m_unitLayoutsByLevel)
    {
        if (it.second == sender)
        {
            level = it.first;
            break;
        }
    }

    emit hireUnit(level, _amount);
}
