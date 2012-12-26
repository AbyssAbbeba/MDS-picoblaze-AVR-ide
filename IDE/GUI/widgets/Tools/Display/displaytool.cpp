#include <QtGui>
#include "displaytool.h"

DisplayTool::DisplayTool(QWidget *parent)
    : QWidget(parent)
{
    //vBoxLayout = new QVBoxLayout();
    gridLayout = new QGridLayout(this);
    display = new DisplaySegment(this);
    
    gridLayout->addWidget(display, 0, 0);
    //gridLayout->addLayout(vBoxLayout);
    this->show();
}

DisplayTool::~DisplayTool()
{
    delete vBoxLayout;
    delete gridLayout;
    delete this;
}

void DisplayTool::segmentPressed(int index)
{
}
