#include <QtGui>
#include "displaysegment.h"

DisplaySegment::DisplaySegment(QWidget *parent)
    : QWidget(parent)
{
    this->show();
    this->setMinimumWidth(130);
    this->setMinimumHeight(200);
    displayWidgets[0] = new DisplayWidget(this, 0, true, false);
    connect(displayWidgets[0], SIGNAL(pressed(int)), this, SLOT(emitPressed(int)));
    displayWidgets[0]->move(40, 15);
    displayWidgets[1] = new DisplayWidget(this, 1, false, false);
    connect(displayWidgets[1], SIGNAL(pressed(int)), this, SLOT(emitPressed(int)));
    displayWidgets[1]->move(10, 40);
    displayWidgets[2] = new DisplayWidget(this, 2, false, false);
    connect(displayWidgets[2], SIGNAL(pressed(int)), this, SLOT(emitPressed(int)));
    displayWidgets[2]->move(95, 40);
    displayWidgets[3] = new DisplayWidget(this, 3, true, false);
    connect(displayWidgets[3], SIGNAL(pressed(int)), this, SLOT(emitPressed(int)));
    displayWidgets[3]->move(40, 90);
    displayWidgets[4] = new DisplayWidget(this, 4, false, false);
    connect(displayWidgets[4], SIGNAL(pressed(int)), this, SLOT(emitPressed(int)));
    displayWidgets[4]->move(10, 115);
    displayWidgets[5] = new DisplayWidget(this, 5, false, false);
    connect(displayWidgets[5], SIGNAL(pressed(int)), this, SLOT(emitPressed(int)));
    displayWidgets[5]->move(95, 115);
    displayWidgets[6] = new DisplayWidget(this, 6, true, false);
    connect(displayWidgets[6], SIGNAL(pressed(int)), this, SLOT(emitPressed(int)));
    displayWidgets[6]->move(40, 165);
    displayWidgets[7] = new DisplayWidget(this, 7, true, true);
    connect(displayWidgets[7], SIGNAL(pressed(int)), this, SLOT(emitPressed(int)));
    displayWidgets[7]->move(115, 165);
}

DisplayWidget* DisplaySegment::getWidget(int index)
{
    return displayWidgets[index];
}

void DisplaySegment::emitPressed(int index)
{
    emit pressed(index);
}

void DisplaySegment::setChar(int number, bool comma)
{

}
