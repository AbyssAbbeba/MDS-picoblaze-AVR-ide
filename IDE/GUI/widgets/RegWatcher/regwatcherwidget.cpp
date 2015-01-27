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


RegWatcherWidget::RegWatcherWidget(QWidget *parent, MCUSimControl *controlUnit, QString name, int type, int address, int regbank)
    : QWidget(parent)
{
    ui.setupUi(this);
    
    m_simControl = controlUnit;
    m_type = type;
    m_address = address;
    m_baseAddress = address;
    m_regbank = regbank;

    ui.lblName->setText(name);

    switch (type)
    {
        case 0:
        {
            if (0 == regbank)
            {
                ui.lblType->setText("REG_A");
            }
            else if (1 == regbank)
            {
                ui.lblType->setText("REG_B");
            }
            else
            {
                ui.lblType->setText("REG");
            }
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
    qDebug() << "devicechanged";
    switch (m_type)
    {
        case 0:
        {
            m_memory = dynamic_cast<MCUSimMemory*>(m_simControl->getSimSubsys(MCUSimSubsys::SubsysId::ID_MEM_REGISTERS));
            m_size = m_memory->size();
            if (0 == m_regbank)
            {
                if (m_address >= m_size/2)
                {
                    ui.lblAddress->setStyleSheet("QLabel { background-color : red; }");
                }
                else
                {
                    ui.lblAddress->setStyleSheet("QLabel { background-color : none; }");
                }
            }
            else if (1 == m_regbank)
            {
                m_address = m_baseAddress + m_size/2;
                if (m_address >= m_size)
                {
                    ui.lblAddress->setStyleSheet("QLabel { background-color : red; }");
                }
                else
                {
                    ui.lblAddress->setStyleSheet("QLabel { background-color : none; }");
                }
            }
            else
            {
                if (m_address >= m_size)
                {
                    ui.lblAddress->setStyleSheet("QLabel { background-color : red; }");
                }
                else
                {
                    ui.lblAddress->setStyleSheet("QLabel { background-color : none; }");
                }
            }
            break;
        }
        case 1:
        {
            m_memory = dynamic_cast<MCUSimMemory*>(m_simControl->getSimSubsys(MCUSimSubsys::SubsysId::ID_MEM_DATA));
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
            m_plio = dynamic_cast<MCUSimPureLogicIO*>(m_simControl->getSimSubsys(MCUSimSubsys::SubsysId::ID_PLIO));
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
            m_plio = dynamic_cast<MCUSimPureLogicIO*>(m_simControl->getSimSubsys(MCUSimSubsys::SubsysId::ID_PLIO));
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
            qDebug() << "device reset reg" << value;
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


void RegWatcherWidget::setReadOnly(bool /*readOnly*/)
{
}


void RegWatcherWidget::handleUpdateRequest(int mask)
{
}


void RegWatcherWidget::unhighlight()
{
    ui.lblValue->setStyleSheet("QLabel { background-color : none; }");
}


int RegWatcherWidget::getAddress()
{
    return m_baseAddress;
}


QString RegWatcherWidget::getName()
{
    return ui.lblName->text();
}


int RegWatcherWidget::getType()
{
    return m_type;
}


int RegWatcherWidget::getRegbank()
{
    return m_regbank;
}


void RegWatcherWidget::setAddress(int address)
{
    m_address = address;
    m_baseAddress = address;
    ui.lblAddress->setText("0x" + QString::number(address, 16));
}


void RegWatcherWidget::setName(QString name)
{
    ui.lblName->setText(name); 
}


void RegWatcherWidget::setType(int type)
{
    switch (type)
    {
        case 0:
        {
            if (0 == m_regbank)
            {
                ui.lblType->setText("REG_A");
            }
            else if (1 == m_regbank)
            {
                ui.lblType->setText("REG_B");
            }
            else
            {
                ui.lblType->setText("REG");
            }
            std::vector<int> mask;
            mask =  {
                        MCUSimMemory::EVENT_MEM_INF_WR_VAL_WRITTEN
                    };
            m_simControl->registerObserver(this, MCUSimSubsys::SubsysId::ID_MEM_REGISTERS, mask);
            break;
        }
        case 1:
        {
            ui.lblType->setText("RAM");
            std::vector<int> mask;
            mask =  {
                        MCUSimMemory::EVENT_MEM_INF_WR_VAL_WRITTEN
                    };
            m_simControl->registerObserver(this, MCUSimSubsys::SubsysId::ID_MEM_DATA, mask);
            break;
        }
        case 2:
        {
            ui.lblType->setText("Out PORT");
            std::vector<int> mask;
            mask =  {
                        MCUSimPureLogicIO::EVENT_PLIO_WRITE
                    };
            m_simControl->registerObserver(this, MCUSimSubsys::SubsysId::ID_PLIO, mask);
            break;
        }
        case 3:
        {
            ui.lblType->setText("In PORT");
            std::vector<int> mask;
            mask =  {
                        MCUSimPureLogicIO::EVENT_PLIO_READ
                    };
            m_simControl->registerObserver(this, MCUSimSubsys::SubsysId::ID_PLIO, mask);
            break;
        }
        default:
        {
            break;
        }
    }
}


void RegWatcherWidget::setRegbank(int regbank)
{
    m_regbank = regbank;
}