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
#include "../MovGridLayout/movgridlayoutitem.h"


//constructor, create layout with HexEdit and MovGridLayout
WSimulationInfo::WSimulationInfo(MCUSimControl *controlUnit, QWidget *parent)
    : QWidget(parent)
{
    this->focusTracking = false;
    this->dragItem = NULL;
    this->parent = parent;
    this->controlUnit = controlUnit;
    layout = new QHBoxLayout(this);
    layout->setSpacing(0);
    m_regListScroll = new QScrollArea(this);
    m_regListScroll->setFrameShape(QFrame::NoFrame);
    gridLayout = new MovGridLayout(m_regListScroll);
    m_regListScroll->setMinimumWidth(gridLayout->width());
    m_hexEdit = new McuMemoryView(controlUnit, gridLayout);
    m_hexEdit->show();
    gridLayout->addWidget(m_hexEdit);
    //m_hexEdit->move(0,0);
    //m_regListScroll->move(m_hexEdit->width()+5,0);
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
    //layout->addWidget(m_hexEdit);
    //qDebug() << "done2";
    layout->addWidget(m_regListScroll);
    //qDebug() << "done3";
    this->setLayout(layout);
    //qDebug() << "done done";
    popupMenu = new QMenu(this);
    QAction *rearangeAct = new QAction("Rearange layout", popupMenu);
    popupMenu->addAction(rearangeAct);
    connect(rearangeAct, SIGNAL(triggered()), this, SLOT(rearangeLayoutSlot()));
}


//copyconstructor for rearanging
WSimulationInfo::WSimulationInfo(MCUSimControl *controlUnit, WSimulationInfo *copySimInfo)
    : QWidget(0)
{
    this->focusTracking = true;
    this->dragItem = NULL;
    this->controlUnit = controlUnit;
    layout = new QHBoxLayout(this);
    layout->setSpacing(0);
    m_regListScroll = new QScrollArea(this);
    m_regListScroll->setFrameShape(QFrame::NoFrame);
    gridLayout = new MovGridLayout(m_regListScroll);
    m_regListScroll->setMinimumWidth(gridLayout->width());
    m_hexEdit = new McuMemoryView(controlUnit, gridLayout);
    m_hexEdit->show();
    gridLayout->addWidget(m_hexEdit, copySimInfo->getGridLayout()->getX(this->gridLayout->actualIndex()), copySimInfo->getGridLayout()->getY(this->gridLayout->actualIndex()));
    //m_hexEdit->move(0,0);
    //m_regListScroll->move(m_hexEdit->width()+5,0);
    m_regList = new RegDisplayList(controlUnit, gridLayout);
    //load MovGridLayout items
    for (int i = 0; i < m_regList->m_numberOfDisplays; i++)
    {
        //some displays can be nullified, NULL doesn't mean end of array
        if (m_regList->m_regDisplays[i] != NULL)
        {
            gridLayout->addWidget((QWidget*)(m_regList->m_regDisplays[i]), copySimInfo->getGridLayout()->getX(this->gridLayout->actualIndex()), copySimInfo->getGridLayout()->getY(this->gridLayout->actualIndex()));
        }
    }
    //qDebug() << "done";
    m_regListScroll->setWidget(gridLayout);
    //qDebug() << "done2";
    //layout->addWidget(m_hexEdit);
    //qDebug() << "done2";
    layout->addWidget(m_regListScroll);
    //qDebug() << "done3";
    this->setLayout(layout);
    //qDebug() << "done done";
    popupMenu = new QMenu(this);
    QAction *saveAct = new QAction("Save layout", popupMenu);
    popupMenu->addAction(saveAct);
    connect(saveAct, SIGNAL(triggered()), this, SLOT(saveLayoutSlot()));
}

//constructor for blank info for rearanging
WSimulationInfo::WSimulationInfo(MCUSimControl *controlUnit)
    : QWidget(0)
{
    this->focusTracking = true;
    this->dragItem = NULL;
    this->controlUnit = controlUnit;
    layout = new QHBoxLayout(this);
    layout->setSpacing(0);
    m_regListScroll = new QScrollArea(this);
    m_regListScroll->setFrameShape(QFrame::NoFrame);
    gridLayout = new MovGridLayout(m_regListScroll);
    m_regListScroll->setMinimumWidth(gridLayout->width());
    m_hexEdit = new McuMemoryView(controlUnit, gridLayout);
    m_hexEdit->show();
    gridLayout->addTempWidget(m_hexEdit);
    //m_hexEdit->move(0,0);
    //m_regListScroll->move(m_hexEdit->width()+5,0);
    m_regList = new RegDisplayList(controlUnit, gridLayout);
    //load MovGridLayout items
    for (int i = 0; i < m_regList->m_numberOfDisplays; i++)
    {
        //some displays can be nullified, NULL doesn't mean end of array
        if (m_regList->m_regDisplays[i] != NULL)
        {
            gridLayout->addTempWidget((QWidget*)(m_regList->m_regDisplays[i]));
        }
    }
    //qDebug() << "done";
    m_regListScroll->setWidget(gridLayout);
    //qDebug() << "done2";
    //layout->addWidget(m_hexEdit);
    //qDebug() << "done2";
    layout->addWidget(m_regListScroll);
    //qDebug() << "done3";
    this->setLayout(layout);
    //qDebug() << "done done";
    popupMenu = new QMenu(this);
    QAction *saveAct = new QAction("Save layout", popupMenu);
    popupMenu->addAction(saveAct);
    connect(saveAct, SIGNAL(triggered()), this, SLOT(saveLayoutSlot()));
}



WSimulationInfo::~WSimulationInfo()
{
    delete m_hexEdit;
    delete m_regList;
    delete gridLayout;
}


void WSimulationInfo::contextMenuEvent(QContextMenuEvent *event)
{
    qDebug() << "simulation info: popup";
    popupMenu->popup(event->globalPos());
    this->gridLayout->grab = 0;
}


void WSimulationInfo::rearangeLayoutSlot()
{
    qDebug() << "simulation info: rearange layout slot";
    WSimulationInfo *info1 = new WSimulationInfo(controlUnit, this);
    info1->show();
    WSimulationInfo *info2 = new WSimulationInfo(controlUnit);
    info2->show();
    info2->setFocusPolicy(Qt::StrongFocus);
    connect(info1, SIGNAL(closeSignal()), info2, SLOT(close()));
    connect(info2, SIGNAL(closeSignal()), info1, SLOT(close()));
    connect(info1->getGridLayout(), SIGNAL(dragSignal(MovGridLayoutItem*)), info2, SLOT(catchDragSlot(MovGridLayoutItem*)));
    connect(info1, SIGNAL(focusOutSignal()), info2, SLOT(setFocusSlot()));
}


void WSimulationInfo::saveLayoutSlot()
{
    qDebug() << "simulation info: save layout slot";
}


void WSimulationInfo::setFocusSlot()
{
    qDebug() << "simulation info: focus in";
    this->setFocus(Qt::OtherFocusReason);
    this->activateWindow();
}

MovGridLayout* WSimulationInfo::getGridLayout()
{
    return gridLayout;
}


void WSimulationInfo::closeEvent(QCloseEvent *event)
{
    emit closeSignal();
    this->close();
}


void WSimulationInfo::catchDragSlot(MovGridLayoutItem *item)
{
    this->dragItem = item;
    if (item != NULL)
    {
        this->gridLayout->grab = true;
    }
    else
    {
        this->gridLayout->grab = false;
    }
}


void WSimulationInfo::enterEvent(QEvent *event)
{
    if (focusTracking == true)
    {
        qDebug() << "simulation info: focus out";
        this->activateWindow();
    }
    //this->clearFocus();
    /*emit focusOutSignal();
    this->gridLayout->grab = false;
    QMouseEvent mouseClick(QEvent::MouseButtonRelease, QPoint(-1, -1), Qt::LeftButton, Qt::LeftButton, Qt::NoModifier);
    QApplication::sendEvent(this->gridLayout, &mouseClick);*/
    /*if (this->dragItem != NULL)
    {
        MovGridLayoutItem *temp = this->gridLayout->getTempGridWidget(dragItem->index);
        this->gridLayout->addWidget(temp->widget, event->pos().x(), event->pos().y());
        temp->widget->show();
    }*/
}