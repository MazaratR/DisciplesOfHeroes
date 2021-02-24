#ifndef CANVAS_H
#define CANVAS_H

#include <QWidget>

class Canvas
        : public QWidget
{
public:
    Canvas(QWidget* _parent = nullptr);
    void paintEvent(QPaintEvent* _event) override;

    void setPixmapToDraw(const QPixmap* _pixmap){
        m_pixmap = _pixmap;
    }

private:
    const QPixmap *m_pixmap = nullptr;
};

#endif // CANVAS_H
