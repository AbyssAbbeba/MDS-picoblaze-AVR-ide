/**
 * @brief C++ file for WLineCounter and WLineCounterWidget classes.
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
#include "wlinecounter.h"



/**
 * @brief Constructor. Sets widget (base) rect.
 * @param parent Parent QTextEdit.
 * @param icons If icons are available.
 * @param hex If shown numbers will be hexadecimal (or decimal).
 * @param font Used font.
 */
WLineCounter::WLineCounter(QPlainTextEdit *parent, bool icons, bool hex, int offset, QFont font)
    : QScrollArea(parent)
{
    //qDebug() << "WLineCounter: WLineCounter()";
    this->setFrameShape(QFrame::NoFrame);
    //parent->show();
    this->parent = parent;
    //this->setMaximumHeight(parent->height());
    //this->setMinimumHeight(this->parent->height());
    QFontMetrics fontMetrics(font);
    int fontWidth = fontMetrics.width("0000");
    this->setMaximumWidth(fontWidth);
    this->setMinimumWidth(fontWidth);
    widget = new WLineCounterWidget(this, icons, hex, offset, font);
    this->setWidget(widget);
    this->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    this->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    connect(this->parent->verticalScrollBar(), SIGNAL(valueChanged(int)), this, SLOT(change(int)));
    //qDebug() << "WLineCounter: return WLineCounter()";
}


/**
 * @brief Returns pointer to the parent QTextEdit.
 * @return Returns pointer to the parent QTextEdit.
 */
QPlainTextEdit* WLineCounter::getTextEdit()
{
    return parent;
}


/**
 * @brief Returns pointer to the shown widget.
 * @return Returns pointer to the shown widget.
 */
WLineCounterWidget* WLineCounter::getWidget()
{
    return widget;
}


/**
 * @brief Sets scrollbar value to given value. (synchronisation with qtextedit)
 * @param value New ScrollBar value.
 */
void WLineCounter::change(int value)
{
    //qDebug() << "WLineCounter: change()" << value;
    //this->verticalScrollBar()->setValue(value);
    this->widget->update();
    this->update();
    //qDebug() << "WLineCounter: return change()";
}


/**
 * @brief
 */
void WLineCounter::changeFont(QFont font)
{
    QFontMetrics fontMetrics(font);
    int fontWidth = fontMetrics.width("0000");
    this->setMaximumWidth(fontWidth);
    this->setMinimumWidth(fontWidth);
    this->widget->changeFont(font);
    this->widget->update();
}



/**
 * @brief Constructor. Inits shown widget.
 * @param parent Parent WLineCounter (base).
 * @param icons If icons are available
 * @param hex If line numbers will be in hexadecimal (or decimal)
 * @param font Used font.
 */
WLineCounterWidget::WLineCounterWidget(WLineCounter *parent, bool icons, bool hex, int offset, QFont font)
    : QWidget(parent)
{
    //qDebug() << "WLineCounterWidget: WLineCounterWidget()";
    this->parent = parent;
    this->icons = icons;
    font.setPixelSize(font.pixelSize()-2);
    this->setFont(font);
    QFontMetrics fontMetrics(font);
    this->fontWidth = fontMetrics.width("0000");
    this->fontHeight = fontMetrics.height();
    this->setMaximumWidth(fontWidth*2);
    //this->setMaximumHeight(parent->height());
    this->setMinimumWidth(fontWidth*2);
    //this->setMinimumHeight(parent->parent->height());
    this->hex = hex;
    this->offset = offset;
    //this->show();
    /*if (parent->getTextEdit()->verticalScrollBar()->maximum() > 0)
    {
        this->changeHeight();
        this->update();
    }*/
    //qDebug() << "WLineCounterWidget: height" << this->height();
    //this->changeHeight();
    //this->parent->verticalScrollBar()->setMaximum(parent->getTextEdit()->verticalScrollBar()->maximum());

    //if (icons == true)
    //{
    //}
    //qDebug() << "WLineCounterWidget: return WLineCounterWidget()";
}


/**
 * @brief Reimplemented paintEvent
 */
void WLineCounterWidget::paintEvent(QPaintEvent *)
{
    QPlainTextEdit* textEdit = parent->getTextEdit();
    //int size = textEdit->currentFont().pointSize();
    //int normalize = 0;
    //QTextCursor lastBlockCursor(textEdit->document()->lastBlock());
    //QRect lastBlockRect = textEdit->cursorRect(lastBlockCursor);
    //this->setMinimumHeight(lastBlockRect.bottom());
    //this->setMaximumHeight(lastBlockRect.bottom());
    QPainter paint;
    paint.begin(this);
    QRectF rect(0,0,this->fontWidth,this->fontHeight);
    QRectF iconRect(0,0,8,this->fontHeight);
    QPointF point;
    point.setX(0);
    QBrush brush(Qt::darkCyan);
    paint.setBackground(brush);
    //QPen pen(Qt::darkCyan);
    QPen pen(Qt::black);
    paint.setBrush(brush);
    paint.setPen(pen);
    //pen.setColor(Qt::black);
    //paint.setPen(pen);
    QTextBlock lineBlock;
    QRect cursorRect;
    for (int i = 0; i < textEdit->document()->blockCount(); i++)
    {
        lineBlock = textEdit->document()->findBlockByNumber(i);
        QTextCursor cursor(lineBlock);
        cursorRect = textEdit->cursorRect(cursor);
        //qDebug() << i << "WLineCounterWidget: block top" << cursorRect.top();
        /*if (i == 0 && cursorRect.top() < 0)
        {
            normalize = cursorRect.top() - 2;
        }*/
        point.setY(cursorRect.top());
        //qDebug() << "cursor normalized: " << cursorRect.top()-normalize;
        //point.setY(i*(size+7)+size/3);
        point.setX(0);
        rect.moveTopLeft(point);
        /*if (icons == true && breakpointList->at(i) == true)
        {
            pen.setColor(Qt::green);
            paint.setPen(pen);
            paint.drawRect(iconRect);
        }
        if (icons == true && bookmarkList->at(i) == true)
        {
            point.setX(8);
            pen.setColor(Qt::yellow);
            paint.setPen(pen);
            paint.drawRect(iconRect);
        }*/
        //paint.drawRect(rect);
        //pen.setColor(Qt::black);
        //paint.setPen(pen);
        if (true == hex)
        {
            if (i * this->offset < 16)
            {
                paint.drawText(rect, Qt::AlignCenter, "0" + QString::number(i * this->offset, 16).toUpper());
            }
            else
            {
                paint.drawText(rect, Qt::AlignCenter, QString::number(i * this->offset, 16).toUpper());
            }
        }
        else
        {
            QString decDraw = "";
            int doneZeros = i+1;
            int len = textEdit->document()->blockCount();
            int zeros = 0;
            while (len >= 10)
            {
                len = len / 10;
                zeros++;
            }
            while (doneZeros >= 10)
            {
                doneZeros = doneZeros/10;
                zeros--;
            }
            for (int j = 0; j < zeros; j++)
            {
                decDraw = decDraw + "0";
            }
            paint.drawText(rect, Qt::AlignCenter, decDraw + QString::number(i+1, 10));
        }
        //pen.setColor(Qt::darkCyan);
        //paint.setPen(pen);
    }
    paint.end();
}


/**
 * @brief Sets new maximum height of WLineCounterWidget.
 */
void WLineCounterWidget::changeHeight()
{
    //qDebug() << "WLineCounterWidget: changeHeight()";
    QPlainTextEdit* textEdit = parent->getTextEdit();
    //this->setMinimumHeight(textEdit->document()->size().height());
    //this->setMaximumHeight(textEdit->document()->size().height());
    this->setMinimumHeight(textEdit->height());
    this->setMaximumHeight(textEdit->height());
    //this->parent->setMaximumHeight(this->height());
    //this->parent->setMinimumHeight(this->height());
    //qDebug() << "WLineCounterWidget: height" << this->height();
    //qDebug() << "WLineCounterWidget: parent doc height:" << textEdit->document()->size().height();
    //qDebug() << "WLineCounterWidget: parent height:" << textEdit->height();
    //qDebug() << "WLineCounterWidget: parent block count:" << textEdit->document()->blockCount();
    this->update();
    //qDebug() << "WLineCounterWidget: return changeHeight()";
}


/**
 * @brief
 */
void WLineCounterWidget::changeFont(QFont font)
{
    font.setPixelSize(font.pixelSize()-2);
    this->setFont(font);
    QFontMetrics fontMetrics(font);
    this->fontWidth = fontMetrics.width("0000");
    this->fontHeight = fontMetrics.height();
    this->setMaximumWidth(fontWidth*2);
    this->setMinimumWidth(fontWidth*2);
}