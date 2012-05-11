#include <QtGui>
#include "wsimulationinfo.h"


WSimulationInfo::WSimulationInfo(QWidget *parent)
{
    layout = new QHBoxLayout(this);
    hexEdit = new HexEdit(this, false, 20, 16);
    gridLayout = new MovGridLayout(this);
    gridLayout->loadGridWidgets();
    gridLayout->setMinimumWidth(500);
    layout->addWidget(hexEdit);
    layout->addWidget(gridLayout);
    this->setLayout(layout);
}
