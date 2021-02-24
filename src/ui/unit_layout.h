#ifndef UNIT_LAYOUT_H
#define UNIT_LAYOUT_H

#include <QWidget>
#include <QElapsedTimer>

#include "units/unit_types.h"
#include "ui/unit_highlight_types.h"

#include "forward/i_unit_forward.h"

namespace Ui {
class UnitLayout;
}

class UnitLayout : public QWidget
{
    Q_OBJECT

public:
    explicit UnitLayout(QWidget *parent = nullptr);
    ~UnitLayout();

    void paintEvent(QPaintEvent* _event) override;

    void setUnitsPixmap(const std::map<UnitType, QPixmap>*_map);

    void clear();
    void setRenderData(const IUnitSharedConst &_unit,
                       bool _highlight = false,
                       bool _mirrored = false);
    void setHighlightType(UnitHighlightType _type);
    void setDisplayAmount(int _displayAmount);

    void showHireUi(bool _show = true);
    void showAmount(bool _show = true);
    void hideEmptyUnits(bool _hide = true);

    IUnitSharedConst displayedUnit() const;

    void resetHireUi();

private slots:
    void on_sb_hireAmount_valueChanged(int _amount);
    void on_pb_hire_clicked();

signals:
    void hireUnit(int _amount);

private:
    Ui::UnitLayout *ui;

    IUnitSharedConst m_displayedUnit;
    const std::map<UnitType, QPixmap>* m_unitPixmaps;
    bool m_mirrored = false;
    bool m_hideEmptyUnits = true;
    uint16_t m_displayAmount = 0;  // For damage/heal display
    uint16_t m_hireAmount = 0;
    UnitHighlightType m_highlightType = UnitHighlightType::None;
    std::map<UnitHighlightType, QString> m_highlightStyleMap;
    QElapsedTimer m_animationTimer;
};

#endif // UNIT_LAYOUT_H
