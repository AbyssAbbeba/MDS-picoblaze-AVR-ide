#include <QtGui>
#include "displaywidget.h"

DisplayWidget::DisplayWidget(QWidget *parent, int index, bool align)
    : QWidget(parent)
{
    if (align)
    {
        this->setMinimumWidth(40);
        this->setMinimumHeight(15);
        this->setMaximumWidth(40);
        this->setMaximumHeight(15);
    }
    else
    {
        this->setMinimumWidth(15);
        this->setMinimumHeight(40);
        this->setMaximumWidth(15);
        this->setMaximumHeight(40);
    }
    this->index = index;
    this->show();
}


void DisplayWidget::mousePressEvent(QMouseEvent *event)
{
    emit pressed(this->index);
    qDebug() << "pressed: " << this->index;
}

void DisplayWidget::paintEvent(QPaintEvent *event)
{
    QPainter paint;
    paint.begin(this);
    QBrush brush(Qt::red);
    paint.setBackground(brush);
    paint.setBrush(Qt::red);
    paint.drawRect(this->rect());
    paint.end();
}
