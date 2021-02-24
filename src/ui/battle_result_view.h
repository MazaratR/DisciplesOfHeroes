#ifndef BATTLERESULTVIEW_H
#define BATTLERESULTVIEW_H

#include <QDialog>
#include <QTimer>

#include "battle_result.h"
#include "units/unit_types.h"

QT_BEGIN_NAMESPACE
namespace Ui { class BattleResultView; }
QT_END_NAMESPACE

class BattleResultView
        : public QDialog
{
    Q_OBJECT

public:
    BattleResultView(QWidget* _parent,
                     const BattleResult& _results,
                     const std::map<UnitType, QPixmap>*_map);

private:
    QString getResultText(const BattleResult& _results) const;

signals:
    void close();

private:
    Ui::BattleResultView *ui;
    QTimer m_timer;
    BattleResult m_results;
};

#endif // BATTLERESULTVIEW_H
