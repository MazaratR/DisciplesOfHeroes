#include "unit_layout.h"
#include "ui_unit_layout.h"

#include <QPainter>
#include <QString>

#include "interfaces/i_unit.h"

UnitLayout::UnitLayout(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::UnitLayout)
{
    ui->setupUi(this);
    showHireUi(false);

    m_highlightStyleMap = {
        {UnitHighlightType::None,    this->styleSheet()},
        {UnitHighlightType::Current, "background-color: rgb(200,255,50,50);"},
        {UnitHighlightType::Damage,  "background-color: rgb(255,10,10,70);"},
        {UnitHighlightType::Heal,    "background-color: rgb(10,255,10,70);"}
    };
}

UnitLayout::~UnitLayout()
{
    delete ui;
}

void UnitLayout::paintEvent(QPaintEvent *_event)
{
    Q_UNUSED(_event)
    static constexpr int animationDuration_ms = 1000;

    if (!m_unitPixmaps || !m_displayedUnit)
    {
        setStyleSheet(m_highlightStyleMap[UnitHighlightType::None]);
        return;
    }

    auto it = m_unitPixmaps->find(m_displayedUnit->type());
    if (it == m_unitPixmaps->end())
        return;

    auto portrait = ui->lbl_unit;
    auto amount   = ui->lbl_unitAmount;

    int borderWidth = 2;
    auto pixmap = it->second.scaled(portrait->width() - borderWidth, portrait->height() - borderWidth);

    if (m_mirrored)
        pixmap = pixmap.transformed(QTransform().scale(-1,1));

    portrait->setPixmap(pixmap);
    amount->setText(QString::number(m_displayedUnit->partySize()));

    ui->sb_hireAmount->setMaximum(m_displayedUnit->partySize());

    int goldCost = m_displayedUnit->cost(ResourceType::Gold);
    ui->sb_goldHireCost->setValue(ui->sb_hireAmount->value() * goldCost);

    if (m_displayAmount)
    {
        if (!m_animationTimer.isValid())
            m_animationTimer.start();
        else if (m_animationTimer.elapsed() >= animationDuration_ms)
            m_displayAmount = 0;
    }
    else
    {
        m_animationTimer.invalidate();
        if (m_highlightType != UnitHighlightType::Current)
            m_highlightType = UnitHighlightType::None;
    }

    setStyleSheet(m_highlightStyleMap[m_highlightType]);

    if (m_displayAmount)
    {
        QString displayText = QString::number(m_displayAmount) + " health points";
        displayText = m_highlightType == UnitHighlightType::Damage ? "-" + displayText
                                                                   : "+" + displayText;
        QPainter painter(this);
        painter.drawText(this->rect().topLeft() + QPoint(25, 30), displayText);
    }
}

void UnitLayout::setUnitsPixmap(const std::map<UnitType, QPixmap> *_map)
{
    m_unitPixmaps = _map;
}

void UnitLayout::clear()
{
    ui->lbl_unit->setPixmap(QPixmap());
    ui->lbl_unitAmount->setText("");
    m_displayedUnit.reset();
}

void UnitLayout::setRenderData(const IUnitSharedConst& _unit, bool _highlight, bool _mirrored)
{
    if (!_unit ||
        ((_unit->partySize() < 1) && m_hideEmptyUnits))
    {
        clear();
        m_highlightType = UnitHighlightType::None;
        update();
        return;
    }

    m_displayedUnit = _unit;

    if ((m_highlightType != UnitHighlightType::Damage) &&
        (m_highlightType != UnitHighlightType::Heal))
    {
        m_highlightType = _highlight ? UnitHighlightType::Current
                                     : UnitHighlightType::None;
    }

    m_mirrored = _mirrored;
    update();
}

void UnitLayout::setHighlightType(UnitHighlightType _type)
{
    m_highlightType = _type;
}

void UnitLayout::setDisplayAmount(int _displayAmount)
{
    m_displayAmount = _displayAmount;
}

void UnitLayout::showHireUi(bool _show)
{
    ui->wgt_hire->setVisible(_show);
}

void UnitLayout::showAmount(bool _show)
{
    ui->lbl_unitAmount->setVisible(_show);
}

void UnitLayout::hideEmptyUnits(bool _hide)
{
    m_hideEmptyUnits = _hide;
}

IUnitSharedConst UnitLayout::displayedUnit() const
{
    return m_displayedUnit;
}

void UnitLayout::resetHireUi()
{
    ui->sb_goldHireCost->setValue(0);
    ui->sb_hireAmount->setValue(0);
}

void UnitLayout::on_sb_hireAmount_valueChanged(int _amount)
{
    m_hireAmount = _amount;
}

void UnitLayout::on_pb_hire_clicked()
{
    emit hireUnit(m_hireAmount);
    resetHireUi();
}
