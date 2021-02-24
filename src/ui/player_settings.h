#ifndef PLAYER_SETTINGS_H
#define PLAYER_SETTINGS_H

#include <QWidget>

#include "races.h"
#include "colors.h"

namespace Ui {
class PlayerSettings;
}

class PlayerSettings : public QWidget
{
    Q_OBJECT

public:
    explicit PlayerSettings(QWidget *parent = nullptr);
    ~PlayerSettings();

    void configAsPlayer(uint8_t _playerId, Race _race);

    Race getSelectedRace() const { return m_selectedRace; }
    Color getSelectedColor() const { return m_selectedColor; }

private:
    Ui::PlayerSettings *ui;

    Color m_selectedColor;
    Race m_selectedRace = Race::Unknown;
    std::map<Race,Color> m_raceAndColorMap;
};

#endif // PLAYER_SETTINGS_H
