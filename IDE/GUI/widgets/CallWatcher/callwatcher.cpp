/**
 * @brief
 * C++ Implementation: ...
 *
 * ...
 *
 * (C) copyright 2013, 2014 Moravia Microsystems, s.r.o.
 *
 * @author Erik Chalupa <erik.chalupa@moravia-microsystems.com>
 * @ingroup CallWatcher
 * @file callwatcher.cpp
 */


#include <QtGui>
#include "callwatcher.h"
#include "callitemwidget.h"
#include "../../../simulators/SimControl/MCUSimControl.h"
#include "../../../simulators/MCUSim/MCUSim.h"

CallWatcher::CallWatcher(QWidget *parent, MCUSimControl *controlUnit)
    : QWidget(parent)
{
    ui.setupUi(this);

    m_simControl = controlUnit;
    m_cpu = dynamic_cast<MCUSimCPU*>(m_simControl->getSimSubsys(MCUSimSubsys::ID_CPU));
    m_intAddr = 0;
    m_run = false;
    std::vector<int> mask;
    mask =  {
                MCUSimCPU::EVENT_CPU_CALL,
                MCUSimCPU::EVENT_CPU_RETURN,
                MCUSimCPU::EVENT_CPU_IRQ,
                MCUSimCPU::EVENT_CPU_RETURN_FROM_ISR
            };
    m_simControl->registerObserver(this, MCUSimSubsys::ID_CPU, mask);


    
    connect(ui.btnReturn,
            SIGNAL(clicked()),
            this,
            SLOT(returnSlot())
           );
    
    connect(m_simControl,
            SIGNAL(updateRequest(int)),
            this,
            SLOT(handleUpdateRequest(int))
           );
}


void CallWatcher::returnSlot()
{
    m_cpu->forceReturn();
    m_simControl->dispatchEvents();
}


void CallWatcher::deviceChanged()
{
    deviceReset();
}


void CallWatcher::deviceReset()
{
    ui.lstCalls->clear();
    m_callStack.clear();
}


void CallWatcher::handleEvent(int subsysId, int eventId, int locationOrReason, int detail)
{
    if (MCUSimSubsys::ID_CPU == subsysId)
    {
        //qDebug() << "CallWatcher: ID_CPU event";
        switch ( eventId )
        {
            case MCUSimCPU::EVENT_CPU_CALL:
            {
                m_callStack.insert(0,0);
                m_locationStack.insert(0, locationOrReason);
                m_detailStack.insert(0, detail);
                if (false == m_run)
                {
                    QListWidgetItem *item = new QListWidgetItem();
                    ui.lstCalls->insertItem(0, item);

                    CallItemWidget *itemWidget = new CallItemWidget(this, "CALL", locationOrReason, detail);
                    itemWidget->show();
                    
                    ui.lstCalls->setItemWidget(item, itemWidget);
                    item->setSizeHint(QSize(0,itemWidget->height()));
                }
                break;
            }
            case MCUSimCPU::EVENT_CPU_RETURN:
            {
                m_callStack.removeFirst();
                m_locationStack.removeFirst();
                m_detailStack.removeFirst();
                if (false == m_run)
                {
                    QListWidgetItem *item = ui.lstCalls->takeItem(0);
                    if (item != NULL)
                    {
                        delete item;
                    }
                }
                break;
            }
            case MCUSimCPU::EVENT_CPU_IRQ:
            {
                qDebug() << "IRQ";
                m_callStack.insert(0,1);
                m_locationStack.insert(0, locationOrReason);
                m_detailStack.insert(0, detail);
                if (false == m_run)
                {
                    QListWidgetItem *item = new QListWidgetItem();
                    ui.lstCalls->insertItem(0, item);

                    CallItemWidget *itemWidget = new CallItemWidget(this, "INTERRUPT", locationOrReason, m_intAddr);
                    itemWidget->show();

                    ui.lstCalls->setItemWidget(item, itemWidget);
                    item->setSizeHint(QSize(0,itemWidget->height()));
                }
                break;
            }
            case MCUSimCPU::EVENT_CPU_RETURN_FROM_ISR:
            {
                m_callStack.removeFirst();
                m_locationStack.removeFirst();
                m_detailStack.removeFirst();
                if (false == m_run)
                {
                    QListWidgetItem *item = ui.lstCalls->takeItem(0);
                    if (item != NULL)
                    {
                        delete item;
                    }
                }
                break;
            }
            default:
            {
                qDebug("CallWatcher: Invalid event (cpu) received, event ignored.");
                qDebug() << "CallWatcher: event " << eventId;
                break;
            }
        }
    }
}


void CallWatcher::handleUpdateRequest(int mask)
{
    //update after run
    if (MCUSimControl::UR_MEMORY_REFRESH  & mask)
    {
        if (true == m_run)
        {
            m_run = false;
        }
        qDebug() << "CallWatcher: afterRun" << m_callStack.count();
        ui.lstCalls->clear();
        for (int i = 0; i < m_callStack.count(); i++)
        {
            if (0 == m_callStack.at(i))
            {
                QListWidgetItem *item = new QListWidgetItem(ui.lstCalls);
                ui.lstCalls->insertItem(0, item);

                CallItemWidget *itemWidget = new CallItemWidget(this, "CALL", m_locationStack.at(i), m_detailStack.at(i));
                itemWidget->show();

                ui.lstCalls->setItemWidget(item, itemWidget);
                item->setSizeHint(QSize(0,itemWidget->height()));
            }
            else if (1 == m_callStack.at(i))
            {
                QListWidgetItem *item = new QListWidgetItem(ui.lstCalls);
                ui.lstCalls->insertItem(0, item);

                CallItemWidget *itemWidget = new CallItemWidget(this, "INTERRUPT", m_locationStack.at(i), m_intAddr);
                itemWidget->show();

                ui.lstCalls->setItemWidget(item, itemWidget);
                item->setSizeHint(QSize(0,itemWidget->height()));
            }
            else
            {
                qDebug() << "CallWatcher: error - junk in m_callStack";
            }
            //QListWidgetItem *item = new QListWidgetItem("Call", ui.lstCalls);
        }
    }
}


void CallWatcher::setReadOnly(bool readOnly)
{
}


void CallWatcher::setRun(bool run)
{
    m_run = run;
}


void CallWatcher::setInterruptAddr(int addr)
{
    m_intAddr = addr;
}


void CallWatcher::setSimulated(bool simulated)
{
    ui.btnReturn->setEnabled(simulated);
}