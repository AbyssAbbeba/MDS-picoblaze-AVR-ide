/**
 * @brief
 * C++ Implementation: ...
 *
 * ...
 *
 * (C) copyright 2013, 2014 Moravia Microsystems, s.r.o.
 *
 * @author Erik Chalupa <erik.chalupa@moravia-microsystems.com>
 * @ingroup DisplayWidget
 * @file displaywidget.cpp
 */


#include <QtGui>
#include "displaywidget.h"


//polygon of displaysegment
DisplayWidget::DisplayWidget(QWidget *parent, int index, bool align, bool comma)
    : QWidget(parent)
{
    if (comma == true)
    {
        this->setMinimumWidth(20);
        this->setMinimumHeight(20);
        this->setMaximumWidth(20);
        this->setMaximumHeight(20);
    }
    else
    {
        if (align == true)
        {
            this->setMinimumWidth(72);
            this->setMinimumHeight(22);
            this->setMaximumWidth(72);
            this->setMaximumHeight(22);
        }
        else
        {
            this->setMinimumWidth(30);
            this->setMinimumHeight(70);
            this->setMaximumWidth(30);
            this->setMaximumHeight(70);
        }
    }
    switch (index)
    {
        case 0:
        {
            QVector<QPointF> points; //0 10   10 0    61 0    71 10  61 20   10 20
            points.append(QPointF(0.0, 10.0));
            points.append(QPointF(10.0, 0.0));
            points.append(QPointF(61.0, 0.0));
            points.append(QPointF(71.0, 10.0));
            points.append(QPointF(61.0, 20.0));
            points.append(QPointF(10.0, 20.0));
            widgetPolygon = new QPolygonF(points);
            break;
        }
        case 1:
        {
            QVector<QPointF> points; //19 0  29 10  21 54  9 66  0 57   8 11
            points.append(QPointF(19.0, 0.0));
            points.append(QPointF(29.0, 10.0));
            points.append(QPointF(21.0, 54.0));
            points.append(QPointF(9.0, 66.0));
            points.append(QPointF(0.0, 57.0));
            points.append(QPointF(8.0, 11.0));
            widgetPolygon = new QPolygonF(points);
            break;
        }
        case 2:
        {
            QVector<QPointF> points; //19 0  29 10  21 54  9 66  0 57   8 11
            points.append(QPointF(19.0, 0.0));
            points.append(QPointF(29.0, 10.0));
            points.append(QPointF(21.0, 54.0));
            points.append(QPointF(9.0, 66.0));
            points.append(QPointF(0.0, 57.0));
            points.append(QPointF(8.0, 11.0));
            widgetPolygon = new QPolygonF(points);
            break;
        }
        case 3:
        {
            QVector<QPointF> points; //0 10   10 0    61 0    71 10  61 20   10 20
            points.append(QPointF(0.0, 10.0));
            points.append(QPointF(10.0, 0.0));
            points.append(QPointF(61.0, 0.0));
            points.append(QPointF(71.0, 10.0));
            points.append(QPointF(61.0, 20.0));
            points.append(QPointF(10.0, 20.0));
            widgetPolygon = new QPolygonF(points);
            break;
        }
        case 4:
        {
            QVector<QPointF> points; //19 0  29 10  21 54  9 66  0 57   8 11
            points.append(QPointF(19.0, 0.0));
            points.append(QPointF(29.0, 10.0));
            points.append(QPointF(21.0, 54.0));
            points.append(QPointF(9.0, 66.0));
            points.append(QPointF(0.0, 57.0));
            points.append(QPointF(8.0, 11.0));
            widgetPolygon = new QPolygonF(points);
            break;
        }
        case 5:
        {
            QVector<QPointF> points; //19 0  29 10  21 54  9 66  0 57   8 11
            points.append(QPointF(19.0, 0.0));
            points.append(QPointF(29.0, 10.0));
            points.append(QPointF(21.0, 54.0));
            points.append(QPointF(9.0, 66.0));
            points.append(QPointF(0.0, 57.0));
            points.append(QPointF(8.0, 11.0));
            widgetPolygon = new QPolygonF(points);
            break;
        }
        case 6:
        {
            QVector<QPointF> points; //0 10   10 0    61 0    71 10  61 20   10 20
            points.append(QPointF(0.0, 10.0));
            points.append(QPointF(10.0, 0.0));
            points.append(QPointF(61.0, 0.0));
            points.append(QPointF(71.0, 10.0));
            points.append(QPointF(61.0, 20.0));
            points.append(QPointF(10.0, 20.0));
            widgetPolygon = new QPolygonF(points);
            break;
        }
    }
    this->index = index;
    this->activated = false;
    this->show();
}


void DisplayWidget::mousePressEvent(QMouseEvent */*event*/)
{
    emit pressed(this->index);
    if (activated)
    {
        activated = false;
    }
    else
    {
        activated = true;
    }
    //qDebug() << "DisplayWidget: pressed: " << this->index;
    this->update();
}


void DisplayWidget::activate(bool active)
{
    activated = active;
    //qDebug() << "DisplayWidget: auto pressed: " << this->index;
    this->update();
}


void DisplayWidget::paintEvent(QPaintEvent */*event*/)
{
    QPainter paint;
    paint.begin(this);

    if (activated == true)
    {
        paint.setBrush(Qt::red);
    }
    else
    {
        paint.setBrush(this->palette().base().color());
        //paint.setBrush(((QWidget*)(this->parent()))->palette().base().color());
    }

    if (index < 7)
    {
        paint.drawConvexPolygon(*widgetPolygon);
        paint.drawText(this->rect(), QString(QChar(index+'A')), QTextOption(Qt::AlignCenter));
    }
    else
    {
        paint.drawEllipse(this->rect().adjusted(0, 0, -1, -1));
        paint.drawText(this->rect().adjusted(0, 0, -1, -1), QString(QChar('P')), QTextOption(Qt::AlignCenter));
    }
    paint.end();
}
