/**
 * @brief
 * C++ Implementation: ...
 *
 * ...
 *
 * (C) copyright 2013, 2014 Moravia Microsystems, s.r.o.
 *
 * @author Erik Chalupa <erik.chalupa@moravia-microsystems.com>
 * @ingroup WLineCounter
 * @file wlinecounter.cpp
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
    this->setFixedWidth(fontWidth);
    widget = new WLineCounterWidget(this, icons, hex, offset, font);
    this->setWidget(widget);
    this->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    this->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    connect(this->parent->verticalScrollBar(),
            SIGNAL(valueChanged(int)),
            this,
            SLOT(change(int))
           );
    connect(widget,
            SIGNAL(breakpointEmit(int)),
            this,
            SLOT(manageBreakpointEmit(int))
           );
    connect(widget,
            SIGNAL(bookmarkEmit(int)),
            this,
            SLOT(manageBookmarkEmit(int))
           );
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
void WLineCounter::change(int /*value*/)
{
    //qDebug() << "WLineCounter: change()" << value;
    //qDebug() << "WLineCounter: value" << this->verticalScrollBar()->value();
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
    this->setFixedWidth(fontWidth);
    this->widget->changeFont(font);
    this->widget->update();
}


void WLineCounter::manageBreakpointEmit(int line)
{
    emit breakpoint(line);
}


void WLineCounter::manageBookmarkEmit(int line)
{
    emit bookmark(line);
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
    //qDebug() << "WLineCounterWidget: ";
    this->parent = parent;
    this->icons = icons;
    //font.setPixelSize(font.pixelSize()-2);
    this->setFont(font);
    QFontMetrics fontMetrics(font);
    this->fontWidth = fontMetrics.width("0000");
    this->fontHeight = fontMetrics.height();
    this->setFixedWidth(fontWidth*2);
    //this->setMaximumHeight(parent->height());
    //this->setMinimumHeight(parent->parent->height());
    this->hex = hex;
    this->offset = offset;

    
    pen.setColor(Qt::black);
    gradient.setColorAt(0, Qt::red);
    gradient.setColorAt(1, Qt::yellow);
    //this->show();
    /*if (parent->getTextEdit()->verticalScrollBar()->maximum() > 0)
    {
        this->changeHeight();
        this->update();
    }*/
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
    //qDebug() << "WLineCounterWidget: paintEvent";
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
    point.setX(0);
    //QBrush brush(Qt::red);
    //paint.setBackground(brush);
    //QPen pen(Qt::darkCyan);
    //QPen pen(Qt::black);
    //paint.setBrush(brush);
    paint.setPen(pen);
    //pen.setColor(Qt::black);
    //paint.setPen(pen);
    for (int i = 0; i < textEdit->document()->blockCount(); i++)
    {
        lineBlock = textEdit->document()->findBlockByNumber(i);
        QTextCursor cursor(lineBlock);
        cursorRect = textEdit->cursorRect(cursor);
        //qDebug() << "WLineCounterWidget: block" << lineBlock.text();
        /*if (i == 0 && cursorRect.top() < 0)
        {
            normalize = cursorRect.top() - 2;
        }*/
        point.setY(cursorRect.top()+1);
        //qDebug() << "cursor normalized: " << cursorRect.top()-normalize;
        //point.setY(i*(size+7)+size/3);
        point.setX(0);
        rect.moveTopLeft(point);
        if (icons == true)
        {
            if (breakpointList->contains(i+1) == true && bookmarkList->contains(i+1) == true)
            {
                gradient.setStart(rect.topLeft());
                gradient.setFinalStop(rect.topRight());
                paint.fillRect(rect, gradient);
            }
            else
            {
                if (bookmarkList->contains(i+1))
                {
                    paint.fillRect(rect, Qt::yellow);
                }
                if (breakpointList->contains(i+1))
                {
                    paint.fillRect(rect, Qt::red);
                }
            }
        }
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
            decDraw = "";
            doneZeros = i+1;
            len = textEdit->document()->blockCount();
            zeros = 0;
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
    //QPlainTextEdit* textEdit = parent->getTextEdit();
    //this->setMinimumHeight(textEdit->document()->size().height());
    //this->setMaximumHeight(textEdit->document()->size().height());
    //if (parent->getTextEdit()->height() < 0)
    //{
    //    this->setFixedHeight(1000);
    //}
    //else
    //{
        this->setFixedHeight(parent->getTextEdit()->height());
    //}
    //this->parent->setMaximumHeight(this->height());
    //this->parent->setMinimumHeight(this->height());
    //qDebug() << "WLineCounterWidget: height" << this->height();
    //qDebug() << "WLineCounterWidget: parent doc height:" << parent->getTextEdit()->document()->size().height();
    //qDebug() << "WLineCounterWidget: parent textedit height:" << parent->getTextEdit()->height();
    //qDebug() << "WLineCounterWidget: parent height:" << parent->height();
    //qDebug() << "WLineCounterWidget: parent block count:" << parent->getTextEdit()->document()->blockCount();
    this->update();
    //qDebug() << "WLineCounterWidget: return changeHeight()";
}


/**
 * @brief
 */
void WLineCounterWidget::changeFont(QFont font)
{
    //font.setPixelSize(font.pixelSize()-2);
    this->setFont(font);
    QFontMetrics fontMetrics(font);
    this->fontWidth = fontMetrics.width("0000");
    this->fontHeight = fontMetrics.height();
    this->setFixedWidth(fontWidth*2);
}


void WLineCounterWidget::setBreakpointList(QList<int> *list)
{
    this->breakpointList = list;
}


void WLineCounterWidget::setBookmarkList(QList<int> *list)
{
    this->bookmarkList = list;
}


void WLineCounterWidget::mousePressEvent(QMouseEvent *event)
{
    if (true == this->icons)
    {
        if (event->button() == Qt::LeftButton)
        {
            //QTextCursor cur = this->parent->getTextEdit()->cursorForPosition(QPoint(0,event->y()));
            //int line = (event->y()+7)/this->fontHeight+this->parent->getTextEdit()->verticalScrollBar()->value();
            //int line = cur.blockNumber();
            emit breakpointEmit(this->parent->getTextEdit()->cursorForPosition(QPoint(0,event->y())).blockNumber());
        }
        else if (event->button() == Qt::RightButton)
        {
            //int line = (event->y()+7)/this->fontHeight+this->parent->getTextEdit()->verticalScrollBar()->value();
            //if (line > 0)
            //{
            //    emit bookmarkEmit(line-1);
            //}
            emit bookmarkEmit(this->parent->getTextEdit()->cursorForPosition(QPoint(0,event->y())).blockNumber());
        }
    }
}