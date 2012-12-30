#include <QtGui>
#include "displaywidget.h"

DisplayWidget::DisplayWidget(QWidget *parent, int index, bool align, bool comma)
    : QWidget(parent)
{
    if (comma == true)
    {
        this->setMinimumWidth(15);
        this->setMinimumHeight(15);
        this->setMaximumWidth(15);
        this->setMaximumHeight(15);
    }
    else
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
    }
    this->setToolTip(QString::number(index));
    this->index = index;
    this->activated = false;
    this->show();
}


void DisplayWidget::mousePressEvent(QMouseEvent *event)
{
    emit pressed(this->index);
    if (activated)
        activated = false;
    else
        activated = true;
    qDebug() << "pressed: " << this->index;
    this->repaint();
}

void DisplayWidget::paintEvent(QPaintEvent *event)
{
    QPainter paint;
    paint.begin(this);
    if (activated == true)
        paint.setBrush(Qt::red);
    else
        paint.setBrush(Qt::gray);
    paint.drawRect(this->rect());
    paint.end();
}
