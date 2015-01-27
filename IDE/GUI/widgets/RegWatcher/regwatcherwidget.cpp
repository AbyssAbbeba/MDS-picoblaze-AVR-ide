/**
 * @brief
 * C++ Implementation: ...
 *
 * ...
 *
 * (C) copyright 2013, 2014 Moravia Microsystems, s.r.o.
 *
 * @author Erik Chalupa <erik.chalupa@moravia-microsystems.com>
 * @ingroup RegWatcherWidget
 * @file regwatcherwidget.cpp
 */


#include <QtGui>
#include "regwatcherwidget.h"
#include "../../../simulators/SimControl/MCUSimControl.h"
#include "../../../simulators/MCUSim/MCUSim.h"
#include "../../../simulators/MCUSim/MCUSimPureLogicIO.h"
#include "../../../simulators/MCUSim/PicoBlaze/PicoBlazeIO.h"


RegWatcherWidget::RegWatcherWidget(QWidget *parent, MCUSimControl *controlUnit, QString name, int type, int address)
    : QWidget(parent)
{
    ui.setupUi(this);

    ui.lblName->setText(name);

    switch (type)
    {
        case 0:
        {
            ui.lblType->setText("REG");
            std::vector<int> mask;
            mask =  {
                        MCUSimMemory::EVENT_MEM_INF_WR_VAL_WRITTEN
                    };
            controlUnit->registerObserver(this, MCUSimSubsys::SubsysId::ID_MEM_REGISTERS, mask);
            break;
        }
        case 1:
        {
            ui.lblType->setText("RAM");
            std::vector<int> mask;
            mask =  {
                        MCUSimMemory::EVENT_MEM_INF_WR_VAL_WRITTEN
                    };
            controlUnit->registerObserver(this, MCUSimSubsys::SubsysId::ID_MEM_DATA, mask);
            break;
        }
        case 2:
        {
            ui.lblType->setText("Out PORT");
            std::vector<int> mask;
            mask =  {
                        MCUSimPureLogicIO::EVENT_PLIO_WRITE
                    };
            controlUnit->registerObserver(this, MCUSimSubsys::SubsysId::ID_PLIO, mask);
            break;
        }
        case 3:
        {
            ui.lblType->setText("In PORT");
            std::vector<int> mask;
            mask =  {
                        MCUSimPureLogicIO::EVENT_PLIO_READ
                    };
            controlUnit->registerObserver(this, MCUSimSubsys::SubsysId::ID_PLIO, mask);
            break;
        }
        default:
        {
            break;
        }
    }

    ui.lblAddress->setText("0x" + QString::number(address, 16));
    
    m_simControl = controlUnit;
    m_type = type;
    m_address = address;
    
    deviceChanged();
}


void RegWatcherWidget::handleEvent(int subsysId, int eventId, int locationOrReason, int detail)
{

    switch (eventId)
    {
        case MCUSimMemory::EVENT_MEM_INF_WR_VAL_WRITTEN:
        {
            if (locationOrReason == m_address && m_address < m_size)
            {
                uint value = 0;
                m_memory->directRead(m_address, value);
                ui.lblValue->setText("0x" + QString::number(value, 16).toUpper());
                ui.lblValue->setStyleSheet("QLabel { background-color : yellow; }");
            }
            break;
        }
        case MCUSimPureLogicIO::EVENT_PLIO_WRITE:
        {
            if (locationOrReason == m_address && m_address < m_size)
            {
                //qDebug() << (unsigned char) m_plio->getOutputArray()[m_address];
                ui.lblValue->setText("0x" + QString::number((unsigned char)m_plio->getOutputArray()[m_address], 16).toUpper());
                ui.lblValue->setStyleSheet("QLabel { background-color : yellow; }");
            }
            break;
        }
        case MCUSimPureLogicIO::EVENT_PLIO_READ:
        {
            if (locationOrReason == m_address && m_address < m_size)
            {
                ui.lblValue->setText("0x" + QString::number((unsigned char)m_plio->getInputArray()[m_address], 16).toUpper());
                ui.lblValue->setStyleSheet("QLabel { background-color : yellow; }");
            }
            break;
        }
        default:
        {
            break;
        }
    }
}



void RegWatcherWidget::deviceChanged()
{
    switch (m_type)
    {
        case 0:
        {
            m_memory = dynamic_cast<MCUSimMemory*>(m_simControlUnit->getSimSubsys(MCUSimSubsys::SubsysId::ID_MEM_REGISTERS));
            m_size = m_memory->size();
            if (m_address >= m_size)
            {
                ui.lblAddress->setStyleSheet("QLabel { background-color : red; }");
            }
            else
            {
                ui.lblAddress->setStyleSheet("QLabel { background-color : none; }");
            }
            break;
        }
        case 1:
        {
            m_memory = dynamic_cast<MCUSimMemory*>(m_simControlUnit->getSimSubsys(MCUSimSubsys::SubsysId::ID_MEM_DATA));
            m_size = m_memory->size();
            if (m_address >= m_size)
            {
                ui.lblAddress->setStyleSheet("QLabel { background-color : red; }");
            }
            else
            {
                ui.lblAddress->setStyleSheet("QLabel { background-color : none; }");
            }
            break;
        }
        case 2:
        {
            m_plio = dynamic_cast<MCUSimPureLogicIO*>(m_simControlUnit->getSimSubsys(MCUSimSubsys::SubsysId::ID_PLIO));
            m_size = m_plio->getNumberOfPorts();
            if (m_address >= m_size)
            {
                ui.lblAddress->setStyleSheet("QLabel { background-color : red; }");
            }
            else
            {
                ui.lblAddress->setStyleSheet("QLabel { background-color : none; }");
            }
            break;
        }
        case 3:
        {
            m_plio = dynamic_cast<MCUSimPureLogicIO*>(m_simControlUnit->getSimSubsys(MCUSimSubsys::SubsysId::ID_PLIO));
            m_size = m_plio->getNumberOfPorts();
            if (m_address >= m_size)
            {
                ui.lblAddress->setStyleSheet("QLabel { background-color : red; }");
            }
            else
            {
                ui.lblAddress->setStyleSheet("QLabel { background-color : none; }");
            }
            break;
        }
        default:
        {
            break;
        }
    }
    deviceReset();
}


void RegWatcherWidget::deviceReset()
{
    switch (m_type)
    {
        case 0:
        {
            uint value = 0;
            m_memory->directRead(m_address, value);
            ui.lblValue->setText("0x" + QString::number(value, 16).toUpper());
            break;
        }
        case 1:
        {
            uint value = 0;
            m_memory->directRead(m_address, value);
            ui.lblValue->setText("0x" + QString::number(value, 16).toUpper());
            break;
        }
        case 2:
        {
            ui.lblValue->setText("0x" + QString::number(m_plio->getOutputArray()[m_address], 16).toUpper());
            
            break;
        }
        case 3:
        {
            ui.lblValue->setText("0x" + QString::number(m_plio->getInputArray()[m_address], 16).toUpper());
            break;
        }
        default:
        {
            break;
        }
    }
    ui.lblValue->setStyleSheet("QLabel { background-color : none; }");
}


void RegWatcherWidget::setReadOnly(bool readOnly)
{
}


void RegWatcherWidget::handleUpdateRequest(int mask)
{
}