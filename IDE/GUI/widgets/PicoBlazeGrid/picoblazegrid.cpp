/**
 * @brief
 * C++ Implementation: ...
 *
 * ...
 *
 * (C) copyright 2013, 2014 Moravia Microsystems, s.r.o.
 *
 * @author Erik Chalupa <erik.chalupa@moravia-microsystems.com>
 * @ingroup PicoBlazeGrid
 * @file picoblazegrid.cpp
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

    if ( NULL == controlUnit )
    {
        qDebug() << "PicoBlazeGrid: controlUnit is NULL";
    }

    std::vector<int> mask = {
                                MCUSimCPU::EVENT_CPU_PC_CHANGED,
                                MCUSimCPU::EVENT_CPU_PC_OVERFLOW,
                                MCUSimCPU::EVENT_CPU_PC_UNDERFLOW,
                                MCUSimCPU::EVENT_CPU_SYS_FATAL_ERROR,
                                MCUSimCPU::EVENT_CPU_ERR_INVALID_OPCODE,
                                MCUSimCPU::EVENT_CPU_ERR_INVALID_JUMP,
                                MCUSimCPU::EVENT_CPU_ERR_INVALID_CALL,
                                MCUSimCPU::EVENT_CPU_WRN_INVALID_IRQ,
                                MCUSimCPU::EVENT_CPU_ERR_INVALID_RET,
                                MCUSimCPU::EVENT_CPU_ERR_INVALID_RETI,
                                MCUSimCPU::EVENT_CPU_ERR_INVALID_OPSET,
                                MCUSimCPU::EVENT_CPU_UNSUPPORTED_INST,
                                MCUSimCPU::EVENT_CPU_INST_IGNORED
                            };
    controlUnit->registerObserver(this, MCUSimSubsys::ID_CPU, mask);

    mask =  {
                PicoBlazeStatusFlags::EVENT_FLAGS_Z_CHANGED,
                PicoBlazeStatusFlags::EVENT_FLAGS_C_CHANGED,
                PicoBlazeStatusFlags::EVENT_FLAGS_IE_CHANGED,
                PicoBlazeStatusFlags::EVENT_FLAGS_INT_CHANGED
            };
    controlUnit->registerObserver(this, MCUSimSubsys::ID_FLAGS, mask);

    mask =  {
                MCUSimPureLogicIO::EVENT_PLIO_WRITE,
                MCUSimPureLogicIO::EVENT_PLIO_READ, 
                MCUSimPureLogicIO::EVENT_PLIO_WRITE_END,
                MCUSimPureLogicIO::EVENT_PLIO_READ_END
            };
    controlUnit->registerObserver(this, MCUSimSubsys::ID_PLIO, mask);

    mask =  {
                PicoBlazeStack::EVENT_STACK_SP_CHANGED
            };
    controlUnit->registerObserver(this, MCUSimSubsys::ID_STACK, mask);

    int offsetMove = 0;
    
    this->memRegs = new RegistersWidget(this, controlUnit, MCUSimSubsys::SubsysId::ID_MEM_REGISTERS);
    this->memRegs->move(10, 42);
    
    if ((dynamic_cast<MCUSimMemory*>(m_simControlUnit->getSimSubsys(MCUSimSubsys::SubsysId::ID_MEM_DATA)))->size() > 0)
    {
        this->memScratch = new McuMemoryView(this, controlUnit, MCUSimSubsys::SubsysId::ID_MEM_DATA);
        this->memScratch->move(305,10);
    }
    else
    {
        this->memScratch = NULL;
        offsetMove = 215;
    }
    
    this->memPorts = new PortHexEdit(this, controlUnit, MCUSimSubsys::SubsysId::ID_PLIO);
    this->memPorts->move(520 - offsetMove, 19);
    this->memStack = new StackWidget(this, controlUnit, MCUSimSubsys::SubsysId::ID_STACK);
    this->memStack->move(960 - offsetMove, 20);
    this->memStack->setFixedWidth(100);
    this->memStack->setMaximumHeight(225);
    this->memRegs->show();
    if (NULL != this->memScratch)
    {
        this->memScratch->show();
    }
    this->memPorts->show();
    this->memStack->show();

    this->wTime = new TimeWidget(this);
    this->wTime->move(1080 - offsetMove, 100);
    this->wTime->show();

    this->lblRegs = new QLabel("Registers", this);
    this->lblRegs->move(40,0);
    if (NULL != this->memScratch)
    {
        this->lblScratch = new QLabel("Scratchpad RAM", this);
        this->lblScratch->move(345,0);
    }
    else
    {
        this->lblScratch = NULL;
    }
    this->lblPortsIn = new QLabel("Input Ports", this);
    this->lblPortsIn->move(560 - offsetMove, 0);
    this->lblPortsOut = new QLabel("Output Ports", this);
    this->lblPortsOut->move(770 - offsetMove, 0);
    this->lblRD = new QLabel("RD", this);
    this->lblRD->move(710 - offsetMove, 0);
    //this->lblRD->setMaximumWidth(25);
    //this->lblRD->setFrameRect(QRect(0,0,0,0));
    //this->lblRD->setFrameShape(QFrame::Box);
    this->lblWR = new QLabel("WR", this);
    this->lblWR->move(925 - offsetMove, 0);
    //this->lblWR->setMaximumWidth(25);
    //this->lblWR->setFrameRect(QRect(0,0,0,0));
    //this->lblWR->setFrameShape(QFrame::NoFrame);
    this->lblStack = new QLabel("Stack", this);
    this->lblStack->move(960 - offsetMove, 0);
    this->lblPC = new QLabel("PC", this);
    this->lblPC->move(1080 - offsetMove, 0);
    this->lblCycles = new QLabel("Cycles", this);
    this->lblCycles->move(1080 - offsetMove, 20);
    this->lblClock = new QLabel("Clock", this);
    this->lblClock->move(1080 - offsetMove, 40);
    this->lblTime = new QLabel("Time", this);
    this->lblTime->move(1080 - offsetMove, 80);

    QFont leFont("Ubuntu Mono");
    leFont.setPixelSize(13);
    this->leSP = new QLineEdit(this);
    this->leSP->setFixedWidth(50);
    this->leSP->setFixedHeight(17);
    this->leSP->setFont(leFont);
    this->leSP->setReadOnly(true);
    this->leSP->move(1010 - offsetMove, 0);
    this->lePC = new QLineEdit(this);
    this->lePC->setFixedWidth(80);
    this->lePC->setFixedHeight(17);
    this->lePC->setFont(leFont);
    this->lePC->setReadOnly(true);
    this->lePC->move(1125 - offsetMove, 0);
    this->leCycles = new QLineEdit(this);
    this->leCycles->setFixedWidth(80);
    this->leCycles->setFixedHeight(17);
    this->leCycles->setFont(leFont);
    this->leCycles->setReadOnly(true);
    this->leCycles->move(1125 - offsetMove, 20);
    this->leClock = new QLineEdit(this);
    this->leClock->setFixedWidth(80);
    this->leClock->setFixedHeight(17);
    this->leClock->setFont(leFont);
    QRegExpValidator *doubleValidator = new QRegExpValidator(QRegExp("[0-9]+(\\.[0-9]{0,3})?"), this->leClock);
    this->leClock->setValidator(doubleValidator);
    this->leClock->move(1125 - offsetMove, 40);

    this->cmbClock = new QComboBox(this);
    this->cmbClock->setFixedHeight(17);
    this->cmbClock->setFixedWidth(80);
    this->cmbClock->move(1125 - offsetMove, 60);
    this->cmbClock->addItem("Hz");
    this->cmbClock->addItem("kHz");
    this->cmbClock->addItem("MHz");
    this->cmbClock->setCurrentIndex(2);
    //this->cmbClock->setStyleSheet ("QComboBox::drop-down {border-width: 1px;} QComboBox::down-arrow {border-width: 1px;}");

    this->btnIntr = new QPushButton("Interrupt", this);
    this->btnIntr->setFixedHeight(17);
    this->btnIntr->setFixedWidth(125);
    this->btnIntr->move(1080 - offsetMove, 180);
    //this->btnPorts = new QPushButton("Output", this);
    //this->btnPorts->setMaximumHeight(17);
    //this->btnPorts->setMaximumWidth(50);
    //this->btnPorts->move(615, 0);
    this->btnCarry = new QPushButton("Carry", this);
    this->btnCarry->move(1080 - offsetMove, 160);
    this->btnCarry->setFixedHeight(17);
    this->btnCarry->setFixedWidth(62);
    this->btnZero = new QPushButton("Zero", this);
    this->btnZero->move(1143 - offsetMove, 160);
    this->btnZero->setFixedHeight(17);
    this->btnZero->setFixedWidth(62);
    this->btnInte = new QPushButton("Int enable", this);
    this->btnInte->setFixedHeight(17);
    this->btnInte->setFixedWidth(125);
    this->btnInte->move(1080 - offsetMove, 200);

    QFont btnFont = this->btnIntr->font();
    btnFont.setPointSize(9);
    //QFont btnFont = QFont("Andale Mono", 9);
    this->btnIntr->setFont(btnFont);
    //this->btnPorts->setFont(btnFont);
    this->btnCarry->setFont(btnFont);
    this->btnZero->setFont(btnFont);
    this->btnInte->setFont(btnFont);

    //this->leClock->setText(
    //    (dynamic_cast<MCUSim::Clock*>controlUnit->getSimSubsys(MCUSim::Subsys::SubsysId::ID_CLK_CONTROL))->
    //);
    //connect(this->btnPorts, SIGNAL(clicked()), this, SLOT(switchPorts()));
    connect(this->btnInte,
            SIGNAL(clicked()),
            this,
            SLOT(setIntE())
           );
    connect(this->btnIntr,
            SIGNAL(clicked()),
            this,
            SLOT(interrupt())
           );
    connect(this->cmbClock,
            SIGNAL(currentIndexChanged(int)),
            this,
            SLOT(changeClockMult(int))
           );
    connect(this->leClock,
            SIGNAL(textEdited(const QString &)),
            this,
            SLOT(changeClock(const QString &))
           );
    
    
    connect(controlUnit,
            SIGNAL(updateRequest(int)),
            this,
            SLOT(handleUpdateRequest(int))
           );

    connect(this->memRegs, SIGNAL(stopSimSig()), this, SLOT(stopSimSlot()));
    connect(this->memPorts, SIGNAL(stopSimSig()), this, SLOT(stopSimSlot()));
    connect(this->memStack, SIGNAL(stopSimSig()), this, SLOT(stopSimSlot()));
    if (NULL != this->memScratch)
    {
        connect(this->memScratch, SIGNAL(stopSimSig()), this, SLOT(stopSimSlot()));
    }

    deviceChanged();
    
    //qDebug() << "PicoBlazeGrid: return PicoBlazeGrid()";
}


void PicoBlazeGrid::handleUpdateRequest(int mask)
{
    if (1 & mask)
    {
        //this->leTime->setText(QString::number(m_simControlUnit->getTotalMCycles()));
        //this->leTime->setStyleSheet("background-color: yellow");
        unsigned long long cycles = m_simControlUnit->getTotalMCycles();
        this->wTime->setTime(2*cycles/(clock*clockMult));
        this->leCycles->setText(QString::number(2*cycles, 10));
    }
    
    if (4 & mask)
    {
        this->updateWidget();
        this->memRegs->updateWidget();
        if (NULL != this->memScratch)
        {
            this->memScratch->updateWidget();
        }
        this->memStack->updateWidget();
        this->memPorts->updateWidget();
    }
}



/**
 * @brief
 * @param
 */
void PicoBlazeGrid::setProjectPath(QString prjPath)
{
    this->projectPath = prjPath;
}


/*void PicoBlazeGrid::switchPorts()
{
    if ( true == this->memPorts->visibleIn )
    {
        this->memPorts->switchPorts();
        this->btnPorts->setText("Input");
        this->btnPorts->setStyleSheet("color: none");
    }
    else
    {
        this->memPorts->switchPorts();
        this->btnPorts->setText("Output");
        this->btnPorts->setStyleSheet("color: none");
    }
}*/


void PicoBlazeGrid::handleEvent(int subsysId, int eventId, int locationOrReason, int detail)
{
    /*if ( MCUSimSubsys::ID_CPU != subsysId && MCUSimSubsys::ID_PLIO != subsysId && MCUSimSubsys::ID_FLAGS != subsysId
        && MCUSimSubsys::ID_STACK != subsysId)
    {
        qDebug("Invalid event received, event ignored.");
        return;
    }*/

    //qDebug() << "PicoBlazeGrid: event";
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
                    this->lePC->setText("0x" + QString::number(value, 16).toUpper());
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
            case MCUSimCPU::EVENT_CPU_PC_OVERFLOW:
            {
                if (true == this->warningOptions.cpuPcOverflow)
                {
                    error(ErrorCode::ERR_CPU_PC_OVERFLOW);
                    emit stopSimSig();
                }
                break;
            }
            case MCUSimCPU::EVENT_CPU_PC_UNDERFLOW:
            {
                if (true == this->warningOptions.cpuPcUnderflow)
                {
                    error(ErrorCode::ERR_CPU_PC_UNDERFLOW);
                    emit stopSimSig();
                }
                break;
            }
            case MCUSimCPU::EVENT_CPU_SYS_FATAL_ERROR:
            {
                error(ErrorCode::ERR_CPU_SYS_FATAL);
                emit stopSimSig();
                break;
            }
            case MCUSimCPU::EVENT_CPU_ERR_INVALID_OPCODE:
            {
                if (true == this->warningOptions.cpuOpcode)
                {
                    error(ErrorCode::ERR_CPU_INVALID_OPCODE);
                    emit stopSimSig();
                }
                break;
            }
            case MCUSimCPU::EVENT_CPU_ERR_INVALID_JUMP:
            {
                if (true == this->warningOptions.cpuJump)
                {
                    error(ErrorCode::ERR_CPU_INVALID_JUMP);
                    emit stopSimSig();
                }
                break;
            }
            case MCUSimCPU::EVENT_CPU_ERR_INVALID_CALL:
            {
                if (true == this->warningOptions.cpuCall)
                {
                    error(ErrorCode::ERR_CPU_INVALID_CALL);
                    emit stopSimSig();
                }
                break;
            }
            case MCUSimCPU::EVENT_CPU_WRN_INVALID_IRQ:
            {
                if (true == this->warningOptions.cpuIRQ)
                {
                    error(ErrorCode::ERR_CPU_INVALID_IRQ);
                    emit stopSimSig();
                }
                break;
            }
            case MCUSimCPU::EVENT_CPU_ERR_INVALID_RET:
            {
                if (true == this->warningOptions.cpuRet)
                {
                    error(ErrorCode::ERR_CPU_INVALID_RET);
                    emit stopSimSig();
                }
                break;
            }
            case MCUSimCPU::EVENT_CPU_ERR_INVALID_RETI:
            {
                if (true == this->warningOptions.cpuReti)
                {
                    error(ErrorCode::ERR_CPU_INVALID_RETI);
                    emit stopSimSig();
                }
                break;
            }
            case MCUSimCPU::EVENT_CPU_ERR_INVALID_OPSET:
            {
                if (true == this->warningOptions.cpuOpset)
                {
                    error(ErrorCode::ERR_CPU_INVALID_OPSET);
                    emit stopSimSig();
                }
                break;
            }
            case MCUSimCPU::EVENT_CPU_UNSUPPORTED_INST:
            {
                if (true == this->warningOptions.cpuInsUnsupported)
                {
                    error(ErrorCode::ERR_CPU_UNSUPPORTED_INST);
                    emit stopSimSig();
                }
                break;
            }
            case MCUSimCPU::EVENT_CPU_INST_IGNORED:
            {
                if (true == this->warningOptions.cpuInsIgnored)
                {
                    error(ErrorCode::ERR_CPU_INST_IGNORED);
                    emit stopSimSig();
                }
                break;
            }
            default:
            {
                qDebug("PicoBlazeGrid: Invalid event (cpu) received, event ignored.");
                qDebug() << "PicoBlazeGrid: event " << eventId;
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
                /*if (this->btnPorts->text() == "Output")
                {
                    this->btnPorts->setStyleSheet("background-color: yellow");
                }*/
                break;
            }
            case MCUSimPureLogicIO::EVENT_PLIO_READ:
            {
                this->lblRD->setStyleSheet("color: #00ff00");
                /*if (this->btnPorts->text() == "Input")
                {
                    this->btnPorts->setStyleSheet("color: background-color: yellow");
                }*/
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
                qDebug("PicoBlazeGrid: Invalid event (plio) received, event ignored.");
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
                qDebug("PicoBlazeGrid: Invalid event (stats) received, event ignored.");
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
                if (locationOrReason > 0xF)
                {
                    this->leSP->setText("0x" + QString::number(locationOrReason, 16).toUpper());
                }
                else
                {
                    this->leSP->setText("0x0" + QString::number(locationOrReason, 16).toUpper());
                }
                this->leSP->setStyleSheet("background-color: yellow");
                break;
            }
            default:
            {
                qDebug("PicoBlazeGrid: Invalid event (stack) received, event ignored.");
                qDebug() << "PicoBlazeGrid: stack event: " << eventId;
                break;
            }
        }
    }
    else
    {
        qDebug("PicoBlazeGrid: Invalid event received, event ignored.");
    }
}


void PicoBlazeGrid::deviceChanged()
{
    //qDebug() << "PicoBlazeGrid: deviceChanged";
    /*if (NULL == this->memScratch)
    {
        qDebug() << "PicoBlazeGrid: null scratch";
    }
    else
    {
        qDebug() << "PicoBlazeGrid: not null scratch";
    }*/
    //qDebug() << "PicoBlazeGrid: size" << (dynamic_cast<MCUSimMemory*>(m_simControlUnit->getSimSubsys(MCUSimSubsys::SubsysId::ID_MEM_DATA)))->size();
    m_cpu = dynamic_cast<MCUSimCPU*>(m_simControlUnit->getSimSubsys(MCUSimSubsys::ID_CPU));
    m_flags = dynamic_cast<PicoBlazeStatusFlags*>(m_simControlUnit->getSimSubsys(MCUSimSubsys::ID_FLAGS));
    m_interrupt = dynamic_cast<PicoBlazeInterruptController*>(m_simControlUnit->getSimSubsys(MCUSimSubsys::ID_INTERRUPTS));
    m_stack = dynamic_cast<PicoBlazeStack*>(m_simControlUnit->getSimSubsys(MCUSimSubsys::ID_STACK));

    if (  this->memScratch == NULL
       && (dynamic_cast<MCUSimMemory*>(m_simControlUnit->getSimSubsys(MCUSimSubsys::SubsysId::ID_MEM_DATA)))->size() > 0 )
    {
        //qDebug() << "PicoBlazeGrid: new scratch";
        this->memPorts->move(520, 19);
        this->memStack->move(960, 20);
        this->wTime->move(1080, 100);
        this->lblPortsIn->move(560, 0);
        this->lblPortsOut->move(770, 0);
        this->lblRD->move(710, 0);
        this->lblWR->move(925, 0);
        this->lblStack->move(960, 0);
        this->lblPC->move(1080, 0);
        this->lblCycles->move(1080, 20);
        this->lblClock->move(1080, 40);
        this->lblTime->move(1080, 80);
        this->leSP->move(1010, 0);
        this->lePC->move(1125, 0);
        this->leCycles->move(1125, 20);
        this->leClock->move(1125, 40);
        this->cmbClock->move(1125, 60);
        this->btnIntr->move(1080, 180);
        this->btnCarry->move(1080, 160);
        this->btnZero->move(1143, 160);
        this->btnInte->move(1080, 200);


        this->lblScratch = new QLabel("Scratchpad RAM", this);
        this->lblScratch->move(345,0);
        this->memScratch = new McuMemoryView(this, m_simControlUnit, MCUSimSubsys::SubsysId::ID_MEM_DATA);
        this->memScratch->move(305,10);
        this->memScratch->show();

        connect(this->memScratch, SIGNAL(stopSimSig()), this, SLOT(stopSimSlot()));
    }
    else
    {
        if (this->memScratch != NULL
           && (dynamic_cast<MCUSimMemory*>(m_simControlUnit->getSimSubsys(MCUSimSubsys::SubsysId::ID_MEM_DATA)))->size() == 0 )
        {
            //qDebug() << "PicoBlazeGrid: delete scratch";
            disconnect(this->memScratch, SIGNAL(stopSimSig()), this, SLOT(stopSimSlot()));
            delete this->memScratch;
            this->memScratch = NULL;
            delete this->lblScratch;
            this->lblScratch = NULL;


            this->memPorts->move(520 - 215, 19);
            this->memStack->move(960 - 215, 20);
            this->wTime->move(1080 - 215, 100);
            this->lblPortsIn->move(560 - 215, 0);
            this->lblPortsOut->move(770 - 215, 0);
            this->lblRD->move(710 - 215, 0);
            this->lblWR->move(925 - 215, 0);
            this->lblStack->move(960 - 215, 0);
            this->lblPC->move(1080 - 215, 0);
            this->lblCycles->move(1080 - 215, 20);
            this->lblClock->move(1080 - 215, 40);
            this->lblTime->move(1080 - 215, 80);
            this->leSP->move(1010 - 215, 0);
            this->lePC->move(1125 - 215, 0);
            this->leCycles->move(1125 - 215, 20);
            this->leClock->move(1125 - 215, 40);
            this->cmbClock->move(1125 - 215, 60);
            this->btnIntr->move(1080 - 215, 180);
            this->btnCarry->move(1080 - 215, 160);
            this->btnZero->move(1143 - 215, 160);
            this->btnInte->move(1080 - 215, 200);
        }
    }
    
    
    deviceReset();
    //qDebug() << "PicoBlazeGrid: return deviceChanged";
}


void PicoBlazeGrid::deviceReset()
{
    //qDebug() << "PicoBlazeGrid: deviceReset()";
    int value = m_cpu->getProgramCounter();
    if (value > 0xFF)
    {
        this->lePC->setText("0x" + QString::number(value, 16).toUpper());
    }
    else if (value > 0xF)
    {
        this->lePC->setText("0x0" + QString::number(value, 16).toUpper());
    }
    else
    {
        this->lePC->setText("0x00" + QString::number(value, 16).toUpper());
    }
    this->leSP->setText("0x00");
    this->wTime->setTime(0);
    this->leClock->setText("10.0");
    this->clock = 10.0;
    this->clockMult = 1000000;
    this->leCycles->setText("0");
    this->setWarningOpt(GuiCfg::getInstance().getWarningsOpt());
    this->unhighlight();
    
    //qDebug() << "PicoBlazeGrid: return deviceReset()";
}


void PicoBlazeGrid::setReadOnly(bool readOnly)
{
    
}


void PicoBlazeGrid::fixHeight()
{
    if (NULL != this->memScratch)
    {
        this->memScratch->fixHeight();
    }
    this->memPorts->fixHeight();
}


void PicoBlazeGrid::unhighlight()
{
    this->lePC->setStyleSheet("background-color: none");
    this->leSP->setStyleSheet("background-color: none");
    this->leCycles->setStyleSheet("background-color: none");
    this->btnZero->setStyleSheet("color: none");
    this->btnCarry->setStyleSheet("color: none");
    //this->btnPorts->setStyleSheet("color: none");
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
    if (NULL != this->memScratch)
    {
        this->memScratch->unhighlight();
    }
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
    m_interrupt->irq();   
}


void PicoBlazeGrid::stopSimSlot()
{
    emit stopSimSig();
}


void PicoBlazeGrid::updateWidget()
{
    uint value = m_cpu->getProgramCounter();
    if (value != (uint)this->lePC->text().toInt(0, 16))
    {
        this->lePC->setStyleSheet("background-color: yellow");
        if (value > 0xFF)
        {
            this->lePC->setText("0x" + QString::number(value, 16).toUpper());
        }
        else if (value > 0xF)
        {
            this->lePC->setText("0x0" + QString::number(value, 16).toUpper());
        }
        else
        {
            this->lePC->setText("0x00" + QString::number(value, 16).toUpper());
        }
    }
    else
    {
        this->lePC->setStyleSheet("background-color: none");
    }

    value = m_stack->getSP();
    if (value != (uint)this->leSP->text().toInt(0, 16))
    {
        this->leSP->setStyleSheet("background-color: yellow");
        if (value > 0xF)
        {
            this->leSP->setText("0x" + QString::number(value, 16).toUpper());
        }
        else
        {
            this->leSP->setText("0x0" + QString::number(value, 16).toUpper());
        }
    }

    if (m_flags->getInte() == true)
    {
        this->btnInte->setStyleSheet("color: #00ff00");
    }
    else
    {
        this->btnInte->setStyleSheet("color: none");
    }
}


void PicoBlazeGrid::changeClock(const QString &text)
{
    this->clock = text.toDouble();
    emit clockChanged(this->clock, (int)this->clockMult);
}


void PicoBlazeGrid::changeClockMult(int index)
{
    this->clockMult = qPow(1000.0, index);
    emit clockChanged(this->clock, (int)this->clockMult);
}


void PicoBlazeGrid::setClock(double clock, int clockMult)
{
    this->clock = clock;
    this->clockMult = clockMult;
    this->leClock->setText(QString::number(clock, 'f', 3));
    if (1 == clockMult)
    {
        this->cmbClock->setCurrentIndex(0);
    }
    else if (1000 == clockMult)
    {
        this->cmbClock->setCurrentIndex(1);
    }
    else
    {
        this->cmbClock->setCurrentIndex(2);
    }
}


void PicoBlazeGrid::setWarningOpt(GuiCfg::WarningsOpt options)
{
    this->warningOptions = options;
}