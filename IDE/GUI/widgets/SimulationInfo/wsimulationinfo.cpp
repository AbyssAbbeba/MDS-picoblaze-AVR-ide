/**
 * @brief
 * C++ Implementation: ...
 *
 * ...
 *
 * (C) copyright 2013, 2014 Moravia Microsystems, s.r.o.
 *
 * @author Erik Chalupa <erik.chalupa@moravia-microsystems.com>
 * @ingroup WSimulationInfo
 * @file wsimulationinfo.cpp
 */


#include <QtGui>
#include "wsimulationinfo.h"
#include "../sim/McuMemoryView.h"
#include "../sim/RegDisplayList.h"


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
WSimulationInfo::WSimulationInfo(MCUSimControl *controlUnit, WSimulationInfo *copySimInfo, QWidget *parent)
    : QWidget(parent)
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
WSimulationInfo::WSimulationInfo(QWidget *parent, MCUSimControl *controlUnit)
    : QWidget(parent)
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
    //qDebug() << "simulation info: popup";
    popupMenu->popup(event->globalPos());
    this->gridLayout->grab = 0;
}


void WSimulationInfo::rearangeLayoutSlot()
{
    //qDebug() << "simulation info: rearange layout slot";
    QWidget *infoWidget = new QWidget(0);
    QVBoxLayout *layout = new QVBoxLayout(infoWidget);
    WSimulationInfo *info1 = new WSimulationInfo(controlUnit, this, infoWidget);
    info1->setProjectPath(projectPath);
    info1->show();
    WSimulationInfo *info2 = new WSimulationInfo(infoWidget, controlUnit);
    info2->setProjectPath(projectPath);
    info2->getGridLayout()->loadGridWidgets(projectPath);
    info2->show();
    layout->addWidget(info1);
    layout->addWidget(info2);
    infoWidget->setLayout(layout);
    //info2->setFocusPolicy(Qt::StrongFocus);
    infoWidget->show();
    //connect(info1, SIGNAL(closeSignal()), info2, SLOT(close()));
    //connect(info2, SIGNAL(closeSignal()), info1, SLOT(close()));
    connect(info1->getGridLayout(), SIGNAL(dragSignal(MovGridLayoutItem*)), info2, SLOT(catchDragSlot(MovGridLayoutItem*)));
    connect(info2, SIGNAL(widgetAdded()), info1->getGridLayout(), SLOT(cancelDrag()));
}


void WSimulationInfo::saveLayoutSlot()
{
    //qDebug() << "simulation info: save layout slot";
    this->getGridLayout()->saveGridWidgets(projectPath);
}


void WSimulationInfo::setFocusSlot()
{
    //qDebug() << "simulation info: focus in";
    this->setFocus(Qt::OtherFocusReason);
    //this->activateWindow();
}

MovGridLayout* WSimulationInfo::getGridLayout()
{
    return gridLayout;
}


/*void WSimulationInfo::closeEvent(QCloseEvent *event)
{
    emit closeSignal();
    this->close();
}*/


void WSimulationInfo::catchDragSlot(MovGridLayoutItem *item)
{
    //qDebug() << "WSimulationInfo: catchDragSlot";
    this->dragItem = item;
    if (item != NULL)
    {
       // this->gridLayout->grab = true;
        MovGridLayoutItem *temp = this->gridLayout->getTempGridWidget(dragItem->index);
        //qDebug() << "WSimulationInfo: index" << temp->index;
        this->gridLayout->addItem(temp);
        temp->widget->show();
    }
    /*else
    {
        this->gridLayout->grab = false;
    }*/
}


/*void WSimulationInfo::enterEvent(QEvent *event)
{
    if (focusTracking == true)
    {
        qDebug() << "simulation info: enter";
    }
    //setFocusSlot();
    //this->clearFocus();
    emit focusOutSignal();
    this->gridLayout->grab = false;
    QMouseEvent mouseClick(QEvent::MouseButtonRelease, QPoint(-1, -1), Qt::LeftButton, Qt::LeftButton, Qt::NoModifier);
    QApplication::sendEvent(this->gridLayout, &mouseClick);
    if (this->dragItem != NULL)
    {
        MovGridLayoutItem *temp = this->gridLayout->getTempGridWidget(dragItem->index);
        this->gridLayout->addWidget(temp->widget);
        temp->widget->show();
    }
}*/


McuMemoryView* WSimulationInfo::getHexEdit()
{
    return m_hexEdit;
}


QWidget* WSimulationInfo::getParent()
{
    return this->parent;
}


void WSimulationInfo::fixHeight()
{
    this->m_hexEdit->fixHeight();
}



void WSimulationInfo::setProjectPath(QString prjPath)
{
    projectPath = prjPath;
}