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
#include "../../../simulators/SimControl/MCUSimControl.h"
#include "../../../simulators/MCUSim/MCUSim.h"

CallWatcher::CallWatcher(QWidget *parent, MCUSimControl *controlUnit)
    : QWidget(parent)
{
    ui.setupUi(this);

    m_run = false;
    std::vector<int> mask;
    mask =  {
                MCUSimCPU::EVENT_CPU_CALL,
                MCUSimCPU::EVENT_CPU_RETURN,
                MCUSimCPU::EVENT_CPU_IRQ,
                MCUSimCPU::EVENT_CPU_RETURN_FROM_ISR
            };
    controlUnit->registerObserver(this, MCUSimSubsys::ID_CPU, mask);


    
    connect(ui.btnReturn, SIGNAL(clicked()), this, SLOT(returnSlot()));
}


void CallWatcher::returnSlot()
{
    
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
                if (true == m_run)
                {
                    m_callStack.append(0);
                }
                else
                {
                    QListWidgetItem *item = new QListWidgetItem("Call", ui.lstCalls);
                    ui.lstCalls->insertItem(0, item);
                }
                break;
            }
            case MCUSimCPU::EVENT_CPU_RETURN:
            {
                if (true == m_run)
                {
                    m_callStack.append(3);
                }
                else
                {
                    for (int i = 0; i < ui.lstCalls->count(); i++)
                    {
                        if ("Call" == ui.lstCalls->item(i)->text())
                        {
                            ui.lstCalls->takeItem(i);
                            break;
                        }
                    }
                }
                break;
            }
            case MCUSimCPU::EVENT_CPU_IRQ:
            {
                if (true == m_run)
                {
                    m_callStack.append(1);
                }
                else
                {
                    QListWidgetItem *item = new QListWidgetItem("Interrupt", ui.lstCalls);
                    ui.lstCalls->insertItem(0, item);
                }
                break;
            }
            case MCUSimCPU::EVENT_CPU_RETURN_FROM_ISR:
            {
                if (true == m_run)
                {
                    m_callStack.append(4);
                }
                else
                {
                    for (int i = 0; i < ui.lstCalls->count(); i++)
                    {
                        if ("Interrupt" == ui.lstCalls->item(i)->text())
                        {
                            ui.lstCalls->takeItem(i);
                            break;
                        }
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
    if (4 & mask)
    {
        ui.lstCalls->clear();
        m_run = false;
        for (int i = m_callStack.count() -1; i >=0; i--)
        {
            QListWidgetItem *item = new QListWidgetItem("Call", ui.lstCalls);
        }
    }
}


void CallWatcher::setReadOnly(bool readOnly)
{
}