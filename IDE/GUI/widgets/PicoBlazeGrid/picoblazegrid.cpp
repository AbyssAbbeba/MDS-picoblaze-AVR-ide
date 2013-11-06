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
#include "picoblazegrid.h"


/**
 * @brief
 * @param
 * @param
 */
PicoBlazeGrid::PicoBlazeGrid(QWidget *parent, MCUSimControl *controlUnit)
    : QWidget(parent)
{
    //qDebug() << "PicoBlazeGrid: PicoBlazeGrid()";

    std::vector<int> mask;
    mask.push_back(MCUSimCPU::EVENT_CPU_PC_CHANGED);
    mask.push_back(MCUSimCPU::EVENT_CPU_PC_OVERFLOW);
    mask.push_back(MCUSimCPU::EVENT_CPU_PC_UNDERFLOW);
    mask.push_back(MCUSimCPU::EVENT_CPU_SYS_FATAL_ERROR);
    mask.push_back(MCUSimCPU::EVENT_CPU_ERR_INVALID_OPCODE);
    mask.push_back(MCUSimCPU::EVENT_CPU_ERR_INVALID_JUMP);
    mask.push_back(MCUSimCPU::EVENT_CPU_ERR_INVALID_CALL);
    mask.push_back(MCUSimCPU::EVENT_CPU_WRN_INVALID_IRQ);
    mask.push_back(MCUSimCPU::EVENT_CPU_ERR_INVALID_RET);
    mask.push_back(MCUSimCPU::EVENT_CPU_ERR_INVALID_RETI);
    mask.push_back(MCUSimCPU::EVENT_CPU_ERR_INVALID_OPSET);
    mask.push_back(MCUSimCPU::EVENT_CPU_UNSUPPORTED_INST);
    mask.push_back(MCUSimCPU::EVENT_CPU_INST_IGNORED);
    controlUnit->registerObserver(this, MCUSimSubsys::ID_CPU, mask);

    mask.clear();
    mask.push_back(PicoBlazeStatusFlags::EVENT_FLAGS_Z_CHANGED);
    mask.push_back(PicoBlazeStatusFlags::EVENT_FLAGS_C_CHANGED);
    mask.push_back(PicoBlazeStatusFlags::EVENT_FLAGS_IE_CHANGED);
    mask.push_back(PicoBlazeStatusFlags::EVENT_FLAGS_INT_CHANGED);
    controlUnit->registerObserver(this, MCUSimSubsys::ID_FLAGS, mask);

    mask.clear();
    mask.push_back(MCUSimPureLogicIO::EVENT_PLIO_WRITE);
    mask.push_back(MCUSimPureLogicIO::EVENT_PLIO_READ);
    mask.push_back(MCUSimPureLogicIO::EVENT_PLIO_WRITE_END);
    mask.push_back(MCUSimPureLogicIO::EVENT_PLIO_READ_END);
    controlUnit->registerObserver(this, MCUSimSubsys::ID_PLIO, mask);

    mask.clear();
    mask.push_back(PicoBlazeStack::EVENT_STACK_SP_CHANGED);
    controlUnit->registerObserver(this, MCUSimSubsys::ID_STACK, mask);
    
    if ( NULL == controlUnit )
    {
        qDebug() << "PicoBlazeGrid: controlUnit is NULL";
    }
    
    this->memRegs = new RegistersWidget(this, controlUnit, MCUSimSubsys::SubsysId::ID_MEM_REGISTERS);
    this->memRegs->move(10, 42);
    this->memScratch = new McuMemoryView(this, controlUnit, MCUSimSubsys::SubsysId::ID_MEM_DATA);
    this->memScratch->move(305,10);
    this->memPorts = new PortHexEdit(this, controlUnit, MCUSimSubsys::SubsysId::ID_PLIO);
    this->memPorts->move(520,19);
    this->memStack = new StackWidget(this, controlUnit, MCUSimSubsys::SubsysId::ID_STACK);
    this->memStack->move(760, 20);
    this->memStack->setMaximumWidth(100);
    this->memStack->setMaximumHeight(225);
    this->memRegs->show();
    this->memScratch->show();
    this->memPorts->show();
    this->memStack->show();

    this->lblRegs = new QLabel("Registers", this);
    this->lblRegs->move(40,0);
    this->lblScratch = new QLabel("Scratch", this);
    this->lblScratch->move(345,0);
    this->lblPorts = new QLabel("Ports", this);
    this->lblPorts->move(560,0);
    this->lblRD = new QLabel("RD", this);
    this->lblRD->move(680, 0);
    //this->lblRD->setMaximumWidth(25);
    //this->lblRD->setFrameRect(QRect(0,0,0,0));
    //this->lblRD->setFrameShape(QFrame::Box);
    this->lblWR = new QLabel("WR", this);
    this->lblWR->move(705, 0);
    //this->lblWR->setMaximumWidth(25);
    //this->lblWR->setFrameRect(QRect(0,0,0,0));
    //this->lblWR->setFrameShape(QFrame::NoFrame);
    this->lblStack = new QLabel("Stack", this);
    this->lblStack->move(760,0);
    this->lblPC = new QLabel("PC", this);
    this->lblPC->move(880,0);
    this->lblTime = new QLabel("Time", this);
    this->lblTime->move(880,20);
    this->lblClock = new QLabel("Clock", this);
    this->lblClock->move(880,40);

    this->leSP = new QLineEdit(this);
    this->leSP->setMaximumWidth(50);
    this->leSP->setMaximumHeight(17);
    this->leSP->setFont(QFont("Andale Mono", 9));
    this->leSP->setReadOnly(true);
    this->leSP->move(810, 0);
    this->lePC = new QLineEdit(this);
    this->lePC->setMaximumWidth(50);
    this->lePC->setMaximumHeight(17);
    this->lePC->setFont(QFont("Andale Mono", 9));
    this->lePC->setReadOnly(true);
    this->lePC->move(920, 0);
    this->leTime = new QLineEdit(this);
    this->leTime->setMaximumWidth(50);
    this->leTime->setMaximumHeight(17);
    this->leTime->setFont(QFont("Andale Mono", 9));
    this->leTime->setReadOnly(true);
    this->leTime->move(920, 20);
    this->leClock = new QLineEdit(this);
    this->leClock->setMaximumWidth(50);
    this->leClock->setMaximumHeight(17);
    this->leClock->setFont(QFont("Andale Mono", 9));
    this->leClock->setReadOnly(true);
    this->leClock->move(920, 40);

    this->btnIntr = new QPushButton("Interrupt", this);
    this->btnIntr->setMaximumHeight(17);
    this->btnIntr->setMaximumWidth(80);
    this->btnIntr->move(880, 80);
    this->btnPorts = new QPushButton("Output", this);
    this->btnPorts->setMaximumHeight(17);
    this->btnPorts->setMaximumWidth(50);
    this->btnPorts->move(615, 0);
    this->btnCarry = new QPushButton("Carry", this);
    this->btnCarry->move(880,60);
    this->btnCarry->setMaximumHeight(17);
    this->btnCarry->setMaximumWidth(40);
    this->btnZero = new QPushButton("Zero", this);
    this->btnZero->move(920,60);
    this->btnZero->setMaximumHeight(17);
    this->btnZero->setMaximumWidth(40);
    this->btnInte = new QPushButton("Int enable", this);
    this->btnInte->setMaximumHeight(17);
    this->btnInte->setMaximumWidth(80);
    this->btnInte->move(880, 100);

    QFont btnFont = this->btnIntr->font();
    btnFont.setPointSize(9);
    this->btnIntr->setFont(btnFont);
    this->btnPorts->setFont(btnFont);
    this->btnCarry->setFont(btnFont);
    this->btnZero->setFont(btnFont);
    this->btnInte->setFont(btnFont);

    //this->leClock->setText(
    //    (dynamic_cast<MCUSim::Clock*>controlUnit->getSimSubsys(MCUSim::Subsys::SubsysId::ID_CLK_CONTROL))->
    //);
    connect(this->btnPorts, SIGNAL(clicked()), this, SLOT(switchPorts()));
    connect(this->btnInte, SIGNAL(clicked()), this, SLOT(setIntE()));
    connect(this->btnIntr, SIGNAL(clicked()), this, SLOT(interrupt()));

    deviceChanged();
    
    //qDebug() << "PicoBlazeGrid: return PicoBlazeGrid()";
}



/**
 * @brief
 * @param
 */
void PicoBlazeGrid::setProjectPath(QString prjPath)
{
    this->projectPath = prjPath;
}


void PicoBlazeGrid::switchPorts()
{
    if ( true == this->memPorts->visibleIn )
    {
        this->memPorts->switchPorts();
        this->btnPorts->setText("Output");
    }
    else
    {
        this->memPorts->switchPorts();
        this->btnPorts->setText("Input");
    }
}


void PicoBlazeGrid::handleEvent(int subsysId, int eventId, int locationOrReason, int detail)
{
    /*if ( MCUSimSubsys::ID_CPU != subsysId && MCUSimSubsys::ID_PLIO != subsysId && MCUSimSubsys::ID_FLAGS != subsysId
        && MCUSimSubsys::ID_STACK != subsysId)
    {
        qDebug("Invalid event received, event ignored.");
        return;
    }*/

    qDebug() << "PicoBlazeGrid: event";
    if (MCUSimSubsys::ID_CPU == subsysId)
    {
        //qDebug() << "PicoBlazeGrid: ID_CPU event";
        switch ( eventId )
        {
            case MCUSimCPU::EVENT_CPU_PC_CHANGED:
            {
                int value = m_cpu->getProgramCounter();
                if (value > 0xFF)
                {
                    this->lePC->setText("0x" + QString::number(value, 16).toUpper() + "h");
                }
                else if (value > 0xF)
                {
                    this->lePC->setText("0x0" + QString::number(value, 16).toUpper());
                }
                else
                {
                    this->lePC->setText("0x00" + QString::number(value, 16).toUpper());
                }
                this->lePC->setStyleSheet("background-color: yellow");
                break;
            }
            default:
            {
                qDebug("Invalid event received, event ignored.");
                break;
            }
        }
    }
    else if (MCUSimSubsys::ID_PLIO == subsysId)
    {
        //qDebug() << "PicoBlazeGrid: ID_PLIO event";
        switch ( eventId )
        {
            case MCUSimPureLogicIO::EVENT_PLIO_WRITE:
            {
                this->lblWR->setStyleSheet("color: #00ff00");
                break;
            }
            case MCUSimPureLogicIO::EVENT_PLIO_READ:
            {
                this->lblRD->setStyleSheet("color: #00ff00");
                break;
            }
            case MCUSimPureLogicIO::EVENT_PLIO_WRITE_END:
            {
                this->lblWR->setStyleSheet("color: none");
                break;
            }
            case MCUSimPureLogicIO::EVENT_PLIO_READ_END:
            {
                this->lblRD->setStyleSheet("color: none");
                break;
            }
            default:
            {
                qDebug("Invalid event received, event ignored.");
                break;
            }
        }
    }
    else if (MCUSimSubsys::ID_FLAGS == subsysId)
    {
        //qDebug() << "PicoBlazeGrid: ID_FLAGS event";
        switch ( eventId )
        {
            case PicoBlazeStatusFlags::EVENT_FLAGS_Z_CHANGED:
            {
                //qDebug() << "PicoBlazeGrid: EVENT_FLAGS_Z_CHANGED";
                if (detail == 0)
                {
                    this->btnZero->setStyleSheet("color: none");
                }
                else
                {
                    this->btnZero->setStyleSheet("color: #00ff00");
                }
                break;
            }
            case PicoBlazeStatusFlags::EVENT_FLAGS_C_CHANGED:
            {
                //qDebug() << "PicoBlazeGrid: EVENT_FLAGS_C_CHANGED";
                if (detail == 0)
                {
                    this->btnCarry->setStyleSheet("color: none");
                }
                else
                {
                    this->btnCarry->setStyleSheet("color: #00ff00");
                }
                break;
            }
            case PicoBlazeStatusFlags::EVENT_FLAGS_IE_CHANGED:
            {
                if (detail == 0)
                {
                    this->btnInte->setStyleSheet("color: none");
                }
                else
                {
                    this->btnInte->setStyleSheet("color: #00ff00");
                }
                break;
            }
            case PicoBlazeStatusFlags::EVENT_FLAGS_INT_CHANGED:
            {
                if (detail == 0)
                {
                    this->btnIntr->setStyleSheet("color: none");
                }
                else
                {
                    this->btnIntr->setStyleSheet("color: #00ff00");
                }
                break;
            }
            default:
            {
                qDebug("Invalid event received, event ignored.");
                break;
            }
        }
    }
    else if (MCUSimSubsys::ID_STACK == subsysId)
    {
        switch ( eventId )
        {
            case PicoBlazeStack::EVENT_STACK_SP_CHANGED:
            {
                this->leSP->setText(QString::number(locationOrReason, 16));
                this->leSP->setStyleSheet("background-color: yellow");
            }
            default:
            {
                qDebug("Invalid event received, event ignored.");
                break;
            }
        }
    }
    else
    {
        qDebug("Invalid event received, event ignored.");
    }
}


void PicoBlazeGrid::deviceChanged()
{
    m_cpu = dynamic_cast<MCUSimCPU*>(m_simControlUnit->getSimSubsys(MCUSimSubsys::ID_CPU));
    m_flags = dynamic_cast<PicoBlazeStatusFlags*>(m_simControlUnit->getSimSubsys(MCUSimSubsys::ID_FLAGS));
    deviceReset();
}


void PicoBlazeGrid::deviceReset()
{
    //qDebug() << "PicoBlazeGrid: deviceReset()";
    int value = m_cpu->getProgramCounter();
    if (value > 0xFF)
    {
        this->lePC->setText(QString::number(value, 16).toUpper() + "h");
    }
    else if (value > 0xF)
    {
        this->lePC->setText("0" + QString::number(value, 16).toUpper() + "h");
    }
    else
    {
        this->lePC->setText("00" + QString::number(value, 16).toUpper() + "h");
    }
    this->leSP->setText("0");
    this->unhighlight();
    
    //qDebug() << "PicoBlazeGrid: return deviceReset()";
}


void PicoBlazeGrid::setReadOnly(bool readOnly)
{
    
}


void PicoBlazeGrid::fixHeight()
{
    memScratch->fixHeight();
    memPorts->fixHeight();
}


void PicoBlazeGrid::unhighlight()
{
    this->lePC->setStyleSheet("background-color: none");
    this->leSP->setStyleSheet("background-color: none");
    this->btnZero->setStyleSheet("color: none");
    this->btnCarry->setStyleSheet("color: none");
    if (m_flags->getInte() == true)
    {
        this->btnInte->setStyleSheet("color: #00ff00");
    }
    else
    {
        this->btnInte->setStyleSheet("color: none");
    }
    this->btnIntr->setStyleSheet("color: none");
    this->lblRD->setStyleSheet("color: none");
    this->lblWR->setStyleSheet("color: none");
    this->memRegs->unhighlight();
    this->memScratch->unhighlight();
    this->memPorts->unhighlight();
    this->memStack->unhighlight();
}


void PicoBlazeGrid::setIntE()
{
    if (m_flags->getInte() == true)
    {
        this->btnInte->setStyleSheet("color: none");
        m_flags->setInte(false);
    }
    else
    {
        this->btnInte->setStyleSheet("color: #00ff00");
        m_flags->setInte(true);
    }
}


void PicoBlazeGrid::interrupt()
{
    //qDebug() << "PicoBlazeGrid: interrupt()";
    m_flags->interrupt();
}