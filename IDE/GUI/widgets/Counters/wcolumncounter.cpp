/**
 * @brief
 * C++ Implementation: ...
 *
 * ...
 *
 * (C) copyright 2013, 2014 Moravia Microsystems, s.r.o.
 *
 * @author Erik Chalupa <erik.chalupa@moravia-microsystems.com>
 * @ingroup WColumnCounter
 * @file wcolumncounter.cpp
 */


#include <QtGui>
#include "wcolumncounter.h"



/**
 * @brief Constructor. Sets properties of the base widget.
 * @param parent Parent QTextEdit
 * @param font Used font.
 * @param columns Number of shown columns.
 */
WColumnCounter::WColumnCounter(QPlainTextEdit *parent, QFont font, int columns)
    : QScrollArea(parent)
{
    this->setFrameShape(QFrame::NoFrame);
    this->parent = parent;
    QFontMetrics fontMetrics(font);
    int height = fontMetrics.height();
    this->setFixedHeight(height);
    this->setFixedWidth(columns*((int)fontMetrics.width("0 0")));
    //this->setMinimumWidth(parent->width());
    //this->setMaximumWidth(parent->width());
    widget = new WColumnCounterWidget(this, font, columns);
    this->setWidget(widget);
    this->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    this->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    connect(parent->horizontalScrollBar(), SIGNAL(valueChanged(int)), this, SLOT(change(int)));
}


/**
 * @brief Returns pointer to the parent QTextEdit
 * @return Returns pointer to the parent QTextEdit
 */
QPlainTextEdit* WColumnCounter::getTextEdit()
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
 * @param font Used font.
 * @param columns Number of shown columns.
 */
WColumnCounterWidget::WColumnCounterWidget(WColumnCounter *parent, QFont font, int columns)
    : QWidget(parent)
{
    this->parent = parent;
    this->columns = columns;
    //qDebug() << font.pixelSize();
    //font.setPixelSize(font.pixelSize());
    //font.setStyleHint(QFont::Monospace);
    QFontMetrics fontMetrics(font);
    this->setFont(font);
    //qDebug() << this->fontInfo().pixelSize();
    //qDebug() << font.pixelSize();
    this->fontHeight = fontMetrics.height();
    this->fontWidth = fontMetrics.width("0 0");
    this->setFixedHeight(this->fontHeight);
    this->setMinimumWidth(parent->width());
}


/**
 * @brief Reimplemented paintEvent
 */
void WColumnCounterWidget::paintEvent(QPaintEvent *)
{
    //int size = parent->getTextEdit()->currentFont().pointSize();
    QPainter paint;
    paint.begin(this);
    QRectF rect(0,0,this->fontWidth,this->fontHeight+9);
    QPointF point;
    point.setY(0);
    QBrush brush(Qt::darkCyan);
    paint.setBackground(brush);
    QPen pen(Qt::black);
    paint.setBrush(brush);
    paint.setPen(pen);
    for (int i = 0; i<columns; i++)
    {
        point.setX(i*this->fontWidth+5);
        rect.moveTopLeft(point);
        //paint.drawRect(rect);
        //paint.setPen(pen);
        if (i<16)
        {
            paint.drawText(rect, Qt::AlignLeft, "0" + QString::number(i, 16).toUpper());
        }
        else
        {
            paint.drawText(rect, Qt::AlignLeft, QString::number(i, 16).toUpper());
        }
        //pen.setColor(Qt::darkCyan);
        //paint.setPen(pen);
    }
    paint.end();
}