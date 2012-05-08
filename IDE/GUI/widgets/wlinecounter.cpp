#include <QtGui>
#include "wlinecounter.h"

WLineCounter::WLineCounter(QTextEdit *parent, bool icons, int width)
{
    this->parent = parent;
    widget = new WLineCounterWidget(this, icons, width);
    this->setWidget(widget);
    this->setMaximumWidth(width);
    //this->setMaximumHeight(parent->height());
    this->setMinimumWidth(width);
    this->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    this->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    //this->setMinimumHeight(parent->height());
    connect(parent->verticalScrollBar(), SIGNAL(valueChanged(int)), this, SLOT(change(int)));
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
    this->verticalScrollBar()->setValue(value);

}




WLineCounterWidget::WLineCounterWidget(WLineCounter *parent, bool icons, int width)
{
    this->parent = parent;
    this->icons = icons;
    this->setMaximumWidth(width);
    //this->setMaximumHeight(parent->height());
    this->setMinimumWidth(width);
    this->setMinimumHeight(parent->height());

    if (icons == true)
    {
        //inicializace poli a pripojeni na signal texteditu content changed
        //nebo zrejme reimplementace handlovani eventu pro qkey::return
        //v texteditu a emitnuti signalu pro novy radek a pote zmena pole
        //+ pripojeni na mouseclick (na cislo je breakpoint, na mezeru
        //vedle je bookmark ci naopak)...
    }
}


void WLineCounterWidget::paintEvent(QPaintEvent *)
{
    QTextEdit* textEdit = parent->getTextEdit();
    int size = textEdit->currentFont().pointSize();
    QPainter paint;
    paint.begin(this);
    QRectF rect(0,0,15,size+9);
    QRectF iconRect(0,0,5,size+9);
    QPointF point;
    point.setX(0);
    QBrush brush(Qt::darkCyan);
    paint.setBackground(brush);
    QPen pen(Qt::darkCyan);
    paint.setBrush(brush);
    paint.setPen(pen);
    for (int i = 0; i<textEdit->document()->lineCount(); i++)
    {
        point.setY(i*(size+7));
        rect.moveTopLeft(point);
        if (icons == true && breakpointList->at(i) == true)
        {
            pen.setColor(Qt::green);
            paint.setPen(pen);
        }
        paint.drawRect(rect);
        pen.setColor(Qt::black);
        paint.setPen(pen);
        if (i<16)
            paint.drawText(rect, Qt::AlignCenter, "0" + QString::number(i, 16).toUpper());
        else
            paint.drawText(rect, Qt::AlignCenter, QString::number(i, 16).toUpper());
        if (icons == true && bookmarkList->at(i) == true)
        {
            pen.setColor(Qt::yellow);
            paint.setPen(pen);
            paint.drawRect(iconRect);
        }
        pen.setColor(Qt::darkCyan);
        paint.setPen(pen);
    }
    paint.end();
}


