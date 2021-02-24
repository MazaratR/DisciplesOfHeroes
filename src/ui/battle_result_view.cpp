#include "battle_result_view.h"
#include "ui_battle_result_view.h"
#include "images_paths.h"

BattleResultView::BattleResultView(QWidget *_parent,
                                   const BattleResult &_results,
                                   const std::map<UnitType, QPixmap> *_map)
    : QDialog(_parent)
    , ui(new Ui::BattleResultView)
    , m_results(_results)
{
    ui->setupUi(this);

    this->setModal(true);
    this->setWindowFlags(this->windowFlags() | Qt::FramelessWindowHint);

//    this->setCursor(QCursor(QPixmap(IMAGE_CURSOR_NORMAL)));

    auto resultText = getResultText(m_results);
    ui->lbl_resultText->setText(resultText);

    ui->wgt_armyOnLeftLosses->setUnitsPixmap(_map);
    ui->wgt_armyOnRightLosses->setUnitsPixmap(_map);

    ui->wgt_armyOnLeftLosses->setTitle(m_results.armyOnLeft.hero ? m_results.armyOnLeft.hero->name().c_str()
                                                                 : "");
    ui->wgt_armyOnRightLosses->setTitle(m_results.armyOnRight.hero ? m_results.armyOnRight.hero->name().c_str()
                                                                   : "");

    auto render = [this](){
        ui->wgt_armyOnLeftLosses->setRenderData(m_results.armyOnLeft.losses);
        ui->wgt_armyOnRightLosses->setRenderData(m_results.armyOnRight.losses, true);
    };

    connect(&m_timer, &QTimer::timeout, render);
    connect(ui->pb_close, SIGNAL(clicked()), this, SIGNAL(close()));

    m_timer.start(100);
}

QString BattleResultView::getResultText(const BattleResult &_results) const
{
    const auto& leftHero = _results.armyOnLeft.hero;
    const auto& rightHero = _results.armyOnRight.hero;

    bool leftIsDefeated = !leftHero || leftHero->isDefeated();
    bool rightIsDefeated = !rightHero || rightHero->isDefeated();

    static QString victoryText = " is celebtaring today's victory!";
    static QString drawText = "Everyone is raven's food today...";

    if (leftIsDefeated && rightIsDefeated)
        return drawText;
    else if (leftIsDefeated)
        return QString(rightHero->name().c_str()) + victoryText;
    else
        return QString(leftHero->name().c_str()) + victoryText;
}
