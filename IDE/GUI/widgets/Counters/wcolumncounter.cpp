#include <QtGui>
#include "wcolumncounter.h"

WColumnCounter::WColumnCounter(QTextEdit *parent, int height, int columns)
{
    this->setFrameShape(QFrame::NoFrame);
    this->parent = parent;
    widget = new WColumnCounterWidget(this, height, columns);
    this->setWidget(widget);
    this->setMaximumHeight(height);
    this->setMinimumWidth(parent->width());
    this->setMaximumWidth(parent->width());
    this->setMinimumHeight(height);
    this->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    this->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    connect(parent->horizontalScrollBar(), SIGNAL(valueChanged(int)), this, SLOT(change(int)));
}

QTextEdit* WColumnCounter::getTextEdit()
{
    return parent;
}

WColumnCounterWidget* WColumnCounter::getWidget()
{
    return widget;
}




void WColumnCounter::change(int value)
{
    this->horizontalScrollBar()->setValue(value);

}



WColumnCounterWidget::WColumnCounterWidget(WColumnCounter *parent, int height, int columns)
{
    this->parent = parent;
    this->columns = columns;
    this->setMaximumHeight(height);
    this->setMinimumWidth(parent->width());
    this->setMinimumHeight(height);
}



void WColumnCounterWidget::paintEvent(QPaintEvent *)
{
    int size = parent->getTextEdit()->currentFont().pointSize();
    QPainter paint;
    paint.begin(this);
    QRectF rect(0,0,25,size+9);
    QPointF point;
    point.setY(0);
    QBrush brush(Qt::darkCyan);
    paint.setBackground(brush);
    QPen pen(Qt::black);
    paint.setBrush(brush);
    paint.setPen(pen);
    for (int i = 0; i<columns; i++)
    {
        //oddelat fixni sirku, zavislost je na velikosti fontu
        point.setX(i*24);
        rect.moveTopLeft(point);
        //paint.drawRect(rect);
        //paint.setPen(pen);
        if (i<16)
            paint.drawText(rect, Qt::AlignCenter, "0" + QString::number(i, 16).toUpper());
        else
            paint.drawText(rect, Qt::AlignCenter, QString::number(i, 16).toUpper());
        //pen.setColor(Qt::darkCyan);
        //paint.setPen(pen);
    }
    paint.end();
}



