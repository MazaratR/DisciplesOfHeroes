#include "canvas.h"

#include <QPainter>

Canvas::Canvas(QWidget *_parent)
    : QWidget(_parent)
{

}

void Canvas::paintEvent(QPaintEvent *_event)
{
    Q_UNUSED(_event)

    if (!m_pixmap)
        return;

    QPainter painter(this);

    painter.drawPixmap(m_pixmap->rect(), *m_pixmap);
}
