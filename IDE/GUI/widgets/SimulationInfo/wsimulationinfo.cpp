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
#include "wsimulationinfo.h"
#include "../sim/McuMemoryView.h"
#include "../sim/RegDisplayList.h"
#include "MCUSimControl.h"


//constructor, create layout with HexEdit and MovGridLayout
WSimulationInfo::WSimulationInfo(MCUSimControl *controlUnit, QWidget *parent)
{
    this->parent = parent;
    layout = new QHBoxLayout(this);
    m_regListScroll = new QScrollArea(this);
    gridLayout = new MovGridLayout(m_regListScroll);
    m_regListScroll->setMinimumWidth(gridLayout->width());
    m_hexEdit = new McuMemoryView(controlUnit, this);
    m_hexEdit->show();
    m_hexEdit->move(0,0);
    m_regListScroll->move(m_hexEdit->width()+5,0);
    m_regList = new RegDisplayList(controlUnit, gridLayout);
    //load MovGridLayout items
    for (int i = 0; i < m_regList->m_numberOfDisplays; i++)
    {
        //some displays can be nullified, NULL doesn't mean end of array
        if (m_regList->m_regDisplays[i] != NULL)
        {
            gridLayout->addWidget((QWidget*)(m_regList->m_regDisplays[i]));
        }
    }
    //qDebug() << "done";
    m_regListScroll->setWidget(gridLayout);
    //qDebug() << "done2";
    layout->addWidget(m_hexEdit);
    //qDebug() << "done2";
    layout->addWidget(m_regListScroll);
    //qDebug() << "done3";
    this->setLayout(layout);
    //qDebug() << "done done";
}

WSimulationInfo::~WSimulationInfo()
{
    delete m_hexEdit;
    delete m_regList;
    delete gridLayout;
}
