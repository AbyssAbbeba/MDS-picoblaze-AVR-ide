#include <QtGui>
#include "displaysegment.h"

DisplaySegment::DisplaySegment(QWidget *parent)
    : QWidget(parent)
{
    this->show();
    this->setMinimumWidth(125);
    this->setMinimumHeight(200);
    displayWidgets[0] = new DisplayWidget(this, 0, true);
    displayWidgets[0]->move(40, 15);
    displayWidgets[1] = new DisplayWidget(this, 1, false);
    displayWidgets[1]->move(10, 40);
    displayWidgets[2] = new DisplayWidget(this, 2, false);
    displayWidgets[2]->move(95, 40);
    displayWidgets[3] = new DisplayWidget(this, 3, true);
    displayWidgets[3]->move(40, 90);
    displayWidgets[4] = new DisplayWidget(this, 4, false);
    displayWidgets[4]->move(10, 115);
    displayWidgets[5] = new DisplayWidget(this, 5, false);
    displayWidgets[5]->move(95, 115);
    displayWidgets[6] = new DisplayWidget(this, 6, true);
    displayWidgets[6]->move(40, 165);
}

DisplayWidget* DisplaySegment::getWidget(int index)
{
    return displayWidgets[index];
}
