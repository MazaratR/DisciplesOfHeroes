#include "player_settings.h"
#include "ui_player_settings.h"

#include "races.h"
#include "colors.h"

PlayerSettings::PlayerSettings(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PlayerSettings)
{
    ui->setupUi(this);

    m_raceAndColorMap = {
        {Race::Human,   Color::Blue},
        {Race::Elf,     Color::Green},
        {Race::Dwarf,   Color::Yellow},
        {Race::Undead,  Color::White}
    };

    ui->lbl_colorSample->setMaximumHeight(ui->lbl_selectedColor->height());
}

PlayerSettings::~PlayerSettings()
{
    delete ui;
}

void PlayerSettings::configAsPlayer(uint8_t _playerId, Race _race)
{
    ui->lbl_player->setText("Player " + QString::number(_playerId));

    m_selectedRace = _race;
    m_selectedColor = m_raceAndColorMap[m_selectedRace];

    ui->lbl_selectedColor->setText(Convert::colorToText(m_selectedColor).c_str());
    ui->lbl_selectedRace->setText(Convert::raceToText(m_selectedRace).c_str());

    std::string colorHexText = Convert::colorToHexStr(m_selectedColor);
    std::string stylesheet = "background-color: #" + colorHexText;
    ui->lbl_colorSample->setStyleSheet(stylesheet.c_str());
}
