/**
 * @brief C++ file for WColumnCounter and WColumnCounterWidget classes
 * C++ Implementation: ...
 *
 * ...
 *
 * Copyright: See COPYING file that comes with this distribution.
 *
 * @author: Erik Chalupa <xchalu10@stud.fit.vutbr.cz>, (C) 2012
 *
 */



#include <QtGui>
#include "wcolumncounter.h"



/**
 * @brief Constructor. Sets properties of the base widget.
 * @param parent Parent QTextEdit
 * @param height Height of widget.
 * @param columns Number of shown columns.
 */
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


/**
 * @brief Returns pointer to the parent QTextEdit
 * @return Returns pointer to the parent QTextEdit
 */
QTextEdit* WColumnCounter::getTextEdit()
{
    return parent;
}


/**
 * @brief Returns shown WColumnCounterWidget.
 * @return Returns shown WColumnCounterWidget.
 */
WColumnCounterWidget* WColumnCounter::getWidget()
{
    return widget;
}


/**
 * @brief Set scrollbar to the given value. (synchronisation)
 * @param value ScrollBar value.
 */
void WColumnCounter::change(int value)
{
    this->horizontalScrollBar()->setValue(value);

}


/**
 * @brief Constructor. Sets properties of shown widget.
 * @param parent Parent WColumnCounter (base).
 * @param height Height of the widget.
 * @param columns Number of shown columns.
 */
WColumnCounterWidget::WColumnCounterWidget(WColumnCounter *parent, int height, int columns)
{
    this->parent = parent;
    this->columns = columns;
    this->setMaximumHeight(height);
    this->setMinimumWidth(parent->width());
    this->setMinimumHeight(height);
}


/**
 * @brief Reimplemented paintEvent
 */
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