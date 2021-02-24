#ifndef NOTIFICATION_VIEW_H
#define NOTIFICATION_VIEW_H

#include <QDialog>

namespace Ui {
class NotificationView;
}

class NotificationView : public QDialog
{
    Q_OBJECT

public:
    explicit NotificationView(QWidget* _parent,
                              const QString& _text);
    ~NotificationView();

private slots:
    void on_pb_ok_clicked();

signals:
    void close();

private:
    Ui::NotificationView *ui;
};

#endif // NOTIFICATION_VIEW_H
