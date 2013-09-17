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
    qDebug() << "PicoBlazeGrid: PicoBlazeGrid()";

    std::vector<int> mask;
    mask.push_back(MCUSimCPU::EVENT_CPU_PC_CHANGED);
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
    controlUnit->registerObserver(this, MCUSimSubsys::ID_PLIO, mask);
    
    if ( NULL == controlUnit )
    {
        qDebug() << "PicoBlazeGrid: controlUnit is NULL";
    }
    
    this->memRegs = new RegistersWidget(this, controlUnit, MCUSimSubsys::SubsysId::ID_MEM_REGISTERS);
    this->memRegs->move(10, 25);
    this->memScratch = new McuMemoryView(this, controlUnit, MCUSimSubsys::SubsysId::ID_MEM_DATA);
    this->memScratch->move(305,5);
    this->memPorts = new PortHexEdit(this, controlUnit, MCUSimSubsys::SubsysId::ID_PLIO);
    this->memPorts->move(520,15);
    this->memStack = new StackWidget(this, controlUnit, MCUSimSubsys::SubsysId::ID_STACK);
    this->memStack->move(760, 25);
    this->memStack->setMaximumWidth(100);
    this->memStack->setMaximumHeight(270);
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
    this->lblRW = new QLabel("WR", this);
    this->lblRW->move(705, 0);
    //this->lblRW->setMaximumWidth(25);
    //this->lblRW->setFrameRect(QRect(0,0,0,0));
    //this->lblRW->setFrameShape(QFrame::NoFrame);
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
    this->lePC->setReadOnly(true);
    this->lePC->move(920, 0);
    this->leTime = new QLineEdit(this);
    this->leTime->setMaximumWidth(50);
    this->leTime->setMaximumHeight(17);
    this->leTime->setReadOnly(true);
    this->leTime->move(920, 20);
    this->leClock = new QLineEdit(this);
    this->leClock->setMaximumWidth(50);
    this->leClock->setMaximumHeight(17);
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

    QFont btnFont = this->btnIntr->font();
    btnFont.setPointSize(9);
    this->btnIntr->setFont(btnFont);
    this->btnPorts->setFont(btnFont);

    //this->leClock->setText(
    //    (dynamic_cast<MCUSim::Clock*>controlUnit->getSimSubsys(MCUSim::Subsys::SubsysId::ID_CLK_CONTROL))->
    //);
    connect(this->btnPorts, SIGNAL(clicked()), this, SLOT(switchPorts()));

    deviceChanged();
    
    qDebug() << "PicoBlazeGrid: return PicoBlazeGrid()";
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
        //this->lblRD->setFrameShape(QFrame::NoFrame);
        //this->lblRW->setFrameShape(QFrame::Box);
    }
    else
    {
        this->memPorts->switchPorts();
        this->btnPorts->setText("Input");
        //this->lblRD->setFrameShape(QFrame::Box);
        //this->lblRW->setFrameShape(QFrame::NoFrame);
    }
}


void PicoBlazeGrid::handleEvent(int subsysId, int eventId, int locationOrReason, int detail)
{
    if ( MCUSimSubsys::ID_CPU != subsysId && MCUSimSubsys::ID_PLIO != subsysId && MCUSimSubsys::ID_FLAGS != subsysId )
    {
        qDebug("Invalid event received, event ignored.");
        return;
    }

    if (MCUSimSubsys::ID_CPU == subsysId)
    { 
        switch ( eventId )
        {
            case MCUSimCPU::EVENT_CPU_PC_CHANGED:
            {
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
                QPalette lePalette = this->lePC->palette();
                lePalette.setColor(this->lePC->backgroundRole(), QColor(Qt::yellow));
                lePC->setPalette(lePalette);
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
        switch ( eventId )
        {
            case MCUSimPureLogicIO::EVENT_PLIO_WRITE:
            {
                QPalette palette = this->lblRW->palette();
                palette.setColor(this->lblRW->foregroundRole(), Qt::green);
                this->lblRW->setPalette(palette);
                break;
            }
            case MCUSimPureLogicIO::EVENT_PLIO_READ:
            {
                QPalette palette = this->lblRD->palette();
                palette.setColor(this->lblRD->foregroundRole(), Qt::green);
                this->lblRD->setPalette(palette);
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
        switch ( eventId )
        {
            case PicoBlazeStatusFlags::EVENT_FLAGS_Z_CHANGED:
            {
                QPalette palette = this->btnZero->palette();
                if (palette.color(this->btnZero->foregroundRole()) ==  Qt::green)
                {
                    palette.setColor(this->btnZero->foregroundRole(), Qt::gray);
                }
                else
                {
                    palette.setColor(this->btnZero->foregroundRole(), Qt::green);
                }
                this->btnZero->setPalette(palette);
                break;
            }
            case PicoBlazeStatusFlags::EVENT_FLAGS_C_CHANGED:
            {
                QPalette palette = this->btnCarry->palette();
                if (palette.color(this->btnCarry->foregroundRole()) ==  Qt::green)
                {
                    palette.setColor(this->btnCarry->foregroundRole(), Qt::gray);
                }
                else
                {
                    palette.setColor(this->btnCarry->foregroundRole(), Qt::green);
                }
                this->btnCarry->setPalette(palette);
                break;
            }
            case PicoBlazeStatusFlags::EVENT_FLAGS_IE_CHANGED:
            {
                break;
            }
            case PicoBlazeStatusFlags::EVENT_FLAGS_INT_CHANGED:
            {
                QPalette palette = this->btnIntr->palette();
                if (palette.color(this->btnIntr->foregroundRole()) ==  Qt::green)
                {
                    palette.setColor(this->btnIntr->foregroundRole(), Qt::gray);
                }
                else
                {
                    palette.setColor(this->btnIntr->foregroundRole(), Qt::green);
                }
                this->btnIntr->setPalette(palette);
                break;
            }
            default:
            {
                qDebug("Invalid event received, event ignored.");
                break;
            }
        }
    }
}


void PicoBlazeGrid::deviceChanged()
{
    m_cpu = dynamic_cast<MCUSimCPU*>(m_simControlUnit->getSimSubsys(MCUSimSubsys::ID_CPU));
    deviceReset();
}


void PicoBlazeGrid::deviceReset()
{
    qDebug() << "PicoBlazeGrid: deviceReset()";
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

    QPalette lePalette = this->lePC->palette();
    lePalette.setColor(lePC->backgroundRole(), lePalette.base().color());
    lePC->setPalette(lePalette);
    
    qDebug() << "PicoBlazeGrid: return deviceReset()";
}


void PicoBlazeGrid::setReadOnly(bool readOnly)
{
    
}


void PicoBlazeGrid::fixHeight()
{
    memScratch->fixHeight();
    memPorts->fixHeight();
}