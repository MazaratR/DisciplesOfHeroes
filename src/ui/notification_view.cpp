#include "notification_view.h"
#include "ui_notification_view.h"
#include "images_paths.h"

NotificationView::NotificationView(QWidget *_parent, const QString &_text) :
    QDialog(_parent),
    ui(new Ui::NotificationView)
{
    ui->setupUi(this);

//    this->setCursor(QCursor(QPixmap(IMAGE_CURSOR_NORMAL)));

    this->setModal(true);
    this->setWindowFlags(this->windowFlags() | Qt::FramelessWindowHint);

    ui->lbl_notificationText->setText(_text);
}

NotificationView::~NotificationView()
{
    delete ui;
}

void NotificationView::on_pb_ok_clicked()
{
    emit close();
}
