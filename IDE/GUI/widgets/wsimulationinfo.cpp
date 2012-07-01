#include <QtGui>
#include "wsimulationinfo.h"
#include "sim/McuMemoryView.h"
#include "sim/RegDisplayList.h"
#include "MCUSimControl.h"


WSimulationInfo::WSimulationInfo(MCUSimControl *controlUnit, QWidget *parent)
{
    this->parent = parent;
    layout = new QHBoxLayout(this);
    m_hexEdit = new McuMemoryView(controlUnit, this);
    m_regList = new RegDisplayList(controlUnit);
    m_regListScroll = new QScrollArea(this);
    m_regListScroll->setWidget(m_regList);
    //gridLayout = new MovGridLayout(this);
    //gridLayout->loadGridWidgets();
    //gridLayout->setMinimumWidth(500);
    layout->addWidget(m_hexEdit);
    layout->addWidget(m_regListScroll);
    m_regListScroll->setMinimumWidth(700);
    //layout->addWidget(gridLayout);
    this->setLayout(layout);
}

WSimulationInfo::~WSimulationInfo()
{
    delete m_hexEdit;
    delete m_regList;
}
