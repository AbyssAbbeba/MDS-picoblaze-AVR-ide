#include <QtGui>
#include "wsimulationinfo.h"
#include "../sim/McuMemoryView.h"
#include "../sim/RegDisplayList.h"
#include "MCUSimControl.h"


WSimulationInfo::WSimulationInfo(MCUSimControl *controlUnit, QWidget *parent)
{
    this->parent = parent;
    layout = new QHBoxLayout(this);
    m_hexEdit = new McuMemoryView(controlUnit, this);
    m_regList = new RegDisplayList(controlUnit);
    m_regListScroll = new QScrollArea(this);
    m_regListScroll->setWidget(m_regList);
    gridLayout = new MovGridLayout(this);
    m_regListScroll->setMinimumWidth(700);
    m_regListScroll->setMinimumHeight(500);
    //gridLayout->addWidget(m_hexEdit);
    //gridLayout->addWidget(m_regListScroll);
    QLabel a("this");
    QLabel b("that");
    gridLayout->addWidget(&a);
    gridLayout->addWidget(&b);
    layout->addWidget(m_hexEdit);
    layout->addWidget(m_regListScroll);
    //layout->addWidget(gridLayout);
    this->setLayout(layout);
}

WSimulationInfo::~WSimulationInfo()
{
    delete m_hexEdit;
    delete m_regList;
}
