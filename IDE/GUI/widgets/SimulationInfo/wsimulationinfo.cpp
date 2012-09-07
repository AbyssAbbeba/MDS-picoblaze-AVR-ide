#include <QtGui>
#include "wsimulationinfo.h"
#include "../sim/McuMemoryView.h"
#include "../sim/RegDisplayList.h"
#include "MCUSimControl.h"


WSimulationInfo::WSimulationInfo(MCUSimControl *controlUnit, QWidget *parent)
{
    this->parent = parent;
    layout = new QHBoxLayout(this);
    m_regListScroll = new QScrollArea(this);
    gridLayout = new MovGridLayout(m_regListScroll);
    m_hexEdit = new McuMemoryView(controlUnit, this);
    m_regList = new RegDisplayList(controlUnit, gridLayout);
    for (int i = 0; i < m_regList->m_numberOfDisplays; i++)
    {
        if (m_regList->m_regDisplays[i] != NULL)
        {
            gridLayout->addWidget((QWidget*)(m_regList->m_regDisplays[i]));
            qDebug() << "adding " << i+1 << "from " << m_regList->m_numberOfDisplays;
        }
    }
    qDebug() << "done";
    m_regListScroll->setWidget(gridLayout);
    layout->addWidget(m_hexEdit);
    layout->addWidget(m_regListScroll);
    this->setLayout(layout);
}

WSimulationInfo::~WSimulationInfo()
{
    delete m_hexEdit;
    delete m_regList;
}
