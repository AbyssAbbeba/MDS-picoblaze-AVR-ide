/**
 * @brief
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

WLineCounter::WLineCounter(QTextEdit *parent, bool icons, bool hex, int width)
{
    this->setFrameShape(QFrame::NoFrame);
    this->parent = parent;
    //this->setMaximumHeight(parent->height());
    this->setMinimumHeight(this->parent->height());
    widget = new WLineCounterWidget(this, icons, hex, width);
    this->setWidget(widget);
    this->setMaximumWidth(width);
    this->setMinimumWidth(width);
    this->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    this->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    connect(this->parent->verticalScrollBar(), SIGNAL(valueChanged(int)), this, SLOT(change(int)));
}

QTextEdit* WLineCounter::getTextEdit()
{
    return parent;
}

WLineCounterWidget* WLineCounter::getWidget()
{
    return widget;
}




void WLineCounter::change(int value)
{
    qDebug() << "Line Counter: value" << value;
    qDebug() << "Line Counter: max value" << this->verticalScrollBar()->maximum();
    qDebug() << "Line Counter: min value" << this->verticalScrollBar()->minimum();
    qDebug() << "Line Counter: texedit max value" << this->parent->verticalScrollBar()->maximum();
    qDebug() << "Line Counter: textedit min value" << this->parent->verticalScrollBar()->minimum();
    qDebug() << "Line Counter: set value" << value*this->verticalScrollBar()->maximum()/this->parent->verticalScrollBar()->maximum();
    this->verticalScrollBar()->setValue(value*this->verticalScrollBar()->maximum()/this->parent->verticalScrollBar()->maximum());
    this->widget->update();
}




WLineCounterWidget::WLineCounterWidget(WLineCounter *parent, bool icons, bool hex, int width)
{
    this->parent = parent;
    this->icons = icons;
    this->setMaximumWidth(width);
    //this->setMaximumHeight(parent->height());
    this->setMinimumWidth(width);
    this->setMinimumHeight(parent->height());
    this->hex = hex;

    //if (icons == true)
    //{
        //inicializace poli a pripojeni na signal texteditu content changed
        //nebo zrejme reimplementace handlovani eventu pro qkey::return
        //v texteditu a emitnuti signalu pro novy radek a pote zmena pole
        //+ pripojeni na mouseclick (na cislo je breakpoint, na mezeru
        //vedle je bookmark ci naopak)...
    //}
}


void WLineCounterWidget::paintEvent(QPaintEvent *)
{
    QTextEdit* textEdit = parent->getTextEdit();
    int size = textEdit->currentFont().pointSize();
    QTextCursor lastBlockCursor(textEdit->document()->lastBlock());
    QRect lastBlockRect = textEdit->cursorRect(lastBlockCursor);
    this->setMinimumHeight(lastBlockRect.top()+2*size);
    this->setMaximumHeight(lastBlockRect.top()+2*size);
    QPainter paint;
    paint.begin(this);
    QRectF rect(0,0,this->width(),2*size);
    QRectF iconRect(0,0,8,2*size);
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
    for (int i = 0; i<textEdit->document()->blockCount(); i++)
    {
        lineBlock = textEdit->document()->findBlockByNumber(i);
        QTextCursor cursor(lineBlock);
        cursorRect = textEdit->cursorRect(cursor);
        point.setY(cursorRect.top());
        //qDebug() << "cursor: " << cursorRect.top();
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
            if (i<16)
                paint.drawText(rect, Qt::AlignCenter, "0" + QString::number(i, 16).toUpper());
            else
                paint.drawText(rect, Qt::AlignCenter, QString::number(i, 16).toUpper());
        }
        else
        {
            QString decDraw = "";
            int doneZeros = i+1;
            int len = textEdit->document()->lineCount();
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
                decDraw = decDraw + "0";
            paint.drawText(rect, Qt::AlignCenter, decDraw + QString::number(i+1, 10));
        }
        //pen.setColor(Qt::darkCyan);
        //paint.setPen(pen);
    }
    paint.end();
}


