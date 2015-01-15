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
#include "../../../simulators/SimControl/MCUSimControl.h"
#include "../../../simulators/MCUSim/MCUSim.h"
#include "../../../simulators/MCUSim/PicoBlaze/PicoBlazeStatusFlags.h"
#include "../../../simulators/MCUSim/PicoBlaze/PicoBlazeInterruptController.h"
#include "../../../simulators/MCUSim/PicoBlaze/PicoBlazeStack.h"
#include "../../../simulators/MCUSim/PicoBlaze/PicoBlazeIO.h"
#include "../StackWidget/stackwidget.h"
#include "../RegistersWidget/registerswidget.h"
#include "../PortHexEdit/porthexedit.h"
#include "../sim/McuMemoryView.h"
#include "../TimeWidget/timewidget.h"
#include "../../errordialog/errordlg.h"


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
                                MCUSimCPU::EVENT_CPU_ERR_UNDEFINED_OPCODE,
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


    /*mask = {
                MCUSimMemory::EVENT_MEM_WRN_RD_UNDEFINED
           };
    controlUnit->registerObserver(this, MCUSimSubsys::ID_MEM_CODE, mask);*/

    m_layout = new QGridLayout(this);
    m_layout->setHorizontalSpacing(0);
    this->setLayout(m_layout);
    int offsetMove = 0;
    //this->lblWRK = NULL;
    
    this->memRegs = new RegistersWidget(this, controlUnit, MCUSimSubsys::SubsysId::ID_MEM_REGISTERS);
    //this->memRegs->move(10, 42);
    m_layout->addWidget(memRegs, 0,0,10,1);
    
    if ((dynamic_cast<MCUSimMemory*>(m_simControlUnit->getSimSubsys(MCUSimSubsys::SubsysId::ID_MEM_DATA)))->size() > 0)
    {
        this->memScratch = new McuMemoryView(this, controlUnit, MCUSimSubsys::SubsysId::ID_MEM_DATA);
        //this->memScratch->move(305,10);
        m_layout->addWidget(memScratch, 0,1,10,1, Qt::AlignLeft);
        //memScratch->setFixedWidth(150);
        
    }
    else
    {
        this->memScratch = NULL;
        offsetMove = 215;
    }
    
    this->memPorts = new PortHexEdit(this, controlUnit, MCUSimSubsys::SubsysId::ID_PLIO);
    //this->memPorts->move(520 - offsetMove, 19);
    m_layout->addWidget(memPorts, 0,2,10,1);

    this->memStack = new StackWidget(this, controlUnit, MCUSimSubsys::SubsysId::ID_STACK);
    //this->memStack->move(960 - offsetMove, 20);
    //this->memStack->setFixedWidth(100);
    //this->memStack->setMaximumHeight(225);
    m_layout->addWidget(memStack, 0,8,10,1);
    this->memRegs->show();
    if (NULL != this->memScratch)
    {
        this->memScratch->show();
    }
    this->memPorts->show();
    this->memStack->show();

    this->wTime = new TimeWidget(this);
    //this->wTime->move(1080 - offsetMove, 100);
    m_layout->addWidget(wTime, 5,10,1,2);
    this->wTime->show();

    //m_layout->addItem(new QSpacerItem(10,1,QSizePolicy::Maximum), 0,5);

    //this->lblRegs = new QLabel("Registers", this);
    //m_layout->addWidget(lblRegs, 0,0,Qt::AlignLeft);
    //this->lblRegs->move(40,0);
    //if (NULL != this->memScratch)
    //{
        //this->lblScratch = new QLabel("Scratchpad RAM", this);
        //this->lblScratch->move(345,0);
        //m_layout->addWidget(lblScratch, 0,1,Qt::AlignLeft);
    //}
    //else
    //{
        //this->lblScratch = NULL;
    //}
    //this->lblPortsIn = new QLabel("Input Ports", this);
    //this->lblPortsIn->move(560 - offsetMove, 0);
    //m_layout->addWidget(lblPortsIn,0,2,Qt::AlignLeft);
    //this->lblPortsOut = new QLabel("Output Ports", this);
    //this->lblPortsOut->move(770 - offsetMove, 0);
    //m_layout->addWidget(lblPortsOut,0,4,Qt::AlignRight);
    //this->lblRD = new QLabel("RD", this);
    //this->lblRD->move(710 - offsetMove, 0);
    //m_layout->addWidget(lblRD,0,3,Qt::AlignRight);
    //this->lblRD->setMaximumWidth(25);
    //this->lblRD->setFrameRect(QRect(0,0,0,0));
    //this->lblRD->setFrameShape(QFrame::Box);
    /*if ("kcpsm6" == QString::fromUtf8(controlUnit->getDeviceName()))
    {
        this->lblWRK = new QLabel("WRK", this);
        this->lblWR->move(900 - offsetMove, 0);
    }*/
    //this->lblWR = new QLabel("WR", this);
    //this->lblWR->move(925 - offsetMove, 0);
    //m_layout->addItem(new QSpacerItem(1,1,QSizePolicy::Maximum), 0,5);
    //m_layout->addWidget(lblWR, 0,7, Qt::AlignRight);
    //this->lblWR->setMaximumWidth(25);
    //this->lblWR->setFrameRect(QRect(0,0,0,0));
    //this->lblWR->setFrameShape(QFrame::NoFrame);
    //this->lblStack = new QLabel("Stack", this);
    //this->lblStack->move(960 - offsetMove, 0);
    //m_layout->addWidget(lblStack, 0, 8, Qt::AlignLeft);
    this->lblPC = new QLabel("PC", this);
    //this->lblPC->move(1080 - offsetMove, 0);
    m_layout->addWidget(lblPC, 0,10,Qt::AlignLeft);
    this->lblCycles = new QLabel("Cycles", this);
    //this->lblCycles->move(1080 - offsetMove, 20);
    m_layout->addWidget(lblCycles, 1,10, Qt::AlignLeft);
    this->lblClock = new QLabel("Clock", this);
    //this->lblClock->move(1080 - offsetMove, 40);
    m_layout->addWidget(lblClock, 2,10,Qt::AlignLeft);
    this->lblTime = new QLabel("Time", this);
    //this->lblTime->move(1080 - offsetMove, 80);
    m_layout->addWidget(lblTime, 4,10,1,1, Qt::AlignLeft|Qt::AlignTop);

    QFont leFont("Ubuntu Mono");
    leFont.setPixelSize(13);
    //this->leSP = new QLineEdit(this);
    //this->leSP->setFixedWidth(50);
    //this->leSP->setFixedHeight(17);
    //this->leSP->setFont(leFont);
    //this->leSP->setReadOnly(true);
    //this->leSP->move(1010 - offsetMove, 0);
    //m_layout->addWidget(leSP, 0,9, Qt::AlignLeft);
    this->lePC = new QLineEdit(this);
    this->lePC->setFixedWidth(80);
    this->lePC->setFixedHeight(19);
    this->lePC->setFont(leFont);
    this->lePC->setReadOnly(true);
    //this->lePC->move(1125 - offsetMove, 0);
    m_layout->addWidget(lePC, 0,11, Qt::AlignLeft);
    this->leCycles = new QLineEdit(this);
    this->leCycles->setFixedWidth(80);
    this->leCycles->setFixedHeight(19);
    this->leCycles->setFont(leFont);
    this->leCycles->setReadOnly(true);
    //this->leCycles->move(1125 - offsetMove, 20);
    m_layout->addWidget(leCycles, 1,11, Qt::AlignLeft);
    this->leClock = new QLineEdit(this);
    this->leClock->setFixedWidth(80);
    this->leClock->setFixedHeight(19);
    this->leClock->setFont(leFont);
    QRegExpValidator *doubleValidator = new QRegExpValidator(QRegExp("[0-9]+(\\.[0-9]{0,3})?"), this->leClock);
    this->leClock->setValidator(doubleValidator);
    //this->leClock->move(1125 - offsetMove, 40);
    m_layout->addWidget(leClock, 2,11, Qt::AlignLeft);

    this->cmbClock = new QComboBox(this);
    this->cmbClock->setFixedHeight(20);
    this->cmbClock->setFixedWidth(80);
    //this->cmbClock->move(1125 - offsetMove, 60);
    m_layout->addWidget(cmbClock, 3,11);
    this->cmbClock->addItem("Hz");
    this->cmbClock->addItem("kHz");
    this->cmbClock->addItem("MHz");
    this->cmbClock->setCurrentIndex(2);
    //this->cmbClock->setStyleSheet ("QComboBox::drop-down {border-width: 1px;} QComboBox::down-arrow {border-width: 1px;}");

    this->btnIntr = new QPushButton("Interrupt", this);
    this->btnIntr->setFixedHeight(19);
    //this->btnIntr->setFixedWidth(125);
    //this->btnIntr->move(1080 - offsetMove, 180);
    m_layout->addWidget(btnIntr, 6,10,1,2);
    //this->btnPorts = new QPushButton("Output", this);
    //this->btnPorts->setMaximumHeight(17);
    //this->btnPorts->setMaximumWidth(50);
    //this->btnPorts->move(615, 0);
    this->btnCarry = new QPushButton("Carry", this);
    //this->btnCarry->move(1080 - offsetMove, 160);
    this->btnCarry->setFixedHeight(19);
    //this->btnCarry->setFixedWidth(62);
    m_layout->addWidget(btnCarry, 7,10);
    this->btnZero = new QPushButton("Zero", this);
    //this->btnZero->move(1143 - offsetMove, 160);
    this->btnZero->setFixedHeight(19);
    //this->btnZero->setFixedWidth(62);
    m_layout->addWidget(btnZero, 7,11, Qt::AlignLeft);
    this->btnInte = new QPushButton("Int enable", this);
    this->btnInte->setFixedHeight(19);
    //this->btnInte->setFixedWidth(125);
    //this->btnInte->move(1080 - offsetMove, 200);
    m_layout->addWidget(btnInte, 8,10,1,2);

    m_layout->addItem(new QSpacerItem(1,1, QSizePolicy::Minimum, QSizePolicy::Expanding), 9,10,1,2);
    m_layout->addItem(new QSpacerItem(1,1, QSizePolicy::Expanding, QSizePolicy::Minimum), 0,12,9,1);

    QFont btnFont = this->btnIntr->font();
    btnFont.setPointSize(9);
    //QFont btnFont = QFont("Andale Mono", 9);
    this->btnIntr->setFont(btnFont);
    //this->btnPorts->setFont(btnFont);
    this->btnCarry->setFont(btnFont);
    this->btnZero->setFont(btnFont);
    this->btnInte->setFont(btnFont);
    
    m_layout->setColumnStretch(0,0);
    m_layout->setColumnStretch(1,0);
    m_layout->setColumnStretch(2,0);
    m_layout->setColumnStretch(3,0);
    m_layout->setColumnStretch(4,0);
    m_layout->setColumnStretch(5,0);
    m_layout->setColumnStretch(6,0);
    m_layout->setColumnStretch(7,0);
    m_layout->setColumnStretch(8,0);
    m_layout->setColumnStretch(9,0);
    m_layout->setColumnStretch(10,0);
    m_layout->setColumnStretch(11,0);
    m_layout->setRowStretch(0,0);
    m_layout->setRowStretch(1,0);
    m_layout->setRowStretch(2,0);
    m_layout->setRowStretch(3,0);
    m_layout->setRowStretch(4,0);
    m_layout->setRowStretch(5,0);
    m_layout->setRowStretch(6,0);
    m_layout->setRowStretch(7,0);
    m_layout->setRowStretch(8,0);
    m_layout->setRowStretch(9,1);
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

    this->installEventFilter(this);

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
/*void PicoBlazeGrid::setProjectPath(QString prjPath)
{
    this->projectPath = prjPath;
}*/


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
                if (true == this->interruptSet)
                {
                    this->btnIntr->setStyleSheet("color: none");
                    this->interruptSet = false;
                }
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
            case MCUSimCPU::EVENT_CPU_ERR_UNDEFINED_OPCODE:
            {
                if (true == this->warningOptions.cpuOpcode)
                {
                    error(ErrorCode::ERR_CPU_UNDEFINED_OPCODE, QString::number(locationOrReason, 16));
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
                //qDebug() << "PicoBlazeGrid: EVENT_FLAGS_IE_CHANGED" << detail;
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
                    //this->btnIntr->setStyleSheet("color: none");
                }
                else
                {
                    //this->btnIntr->setStyleSheet("color: #00ff00");
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
    /*else if (MCUSimSubsys::ID_MEM_CODE == subsysId)
    {
        switch (eventId)
        {
            case MCUSimMemory::EVENT_MEM_WRN_RD_UNDEFINED:
            {
                error(ERR_MEM_RD_UNDEFINED);
                emit stopSimSig();
                break;
            }
            default:
            {
                qDebug("PicoBlazeGrid: Invalid event (stack) received, event ignored.");
                qDebug() << "PicoBlazeGrid: stack event: " << eventId;
                break;
            }
        }
    }*/
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
        qDebug() << "PicoBlazeGrid: new scratch";
        //this->memPorts->move(520, 19);
        //this->memStack->move(960, 20);
        //this->wTime->move(1080, 100);
        //this->lblPortsIn->move(560, 0);
       // this->lblPortsOut->move(770, 0);
        //this->lblRD->move(710, 0);
        //if (MCUSim::FAMILY_KCPSM6 == m_simControlUnit->getFamily())
        //{
        //    this->lblWRK->move(890, 0);
        //}
        //this->lblWR->move(925, 0);
        //this->lblStack->move(960, 0);
        //this->lblPC->move(1080, 0);
        //this->lblCycles->move(1080, 20);
        //this->lblClock->move(1080, 40);
        //this->lblTime->move(1080, 80);
        //this->leSP->move(1010, 0);
        //this->lePC->move(1125, 0);
        //this->leCycles->move(1125, 20);
        //this->leClock->move(1125, 40);
        //this->cmbClock->move(1125, 60);
        //this->btnIntr->move(1080, 180);
        //this->btnCarry->move(1080, 160);
        //this->btnZero->move(1143, 160);
        //this->btnInte->move(1080, 200);


        /*if (NULL == this->lblScratch)
        {
            this->lblScratch = new QLabel("Scratchpad RAM", this);
            this->lblScratch->show();
        }
        this->lblScratch->move(345,0);*/
        this->memScratch = new McuMemoryView(this, m_simControlUnit, MCUSimSubsys::SubsysId::ID_MEM_DATA);
        m_layout->addWidget(memScratch, 0,1,10,1, Qt::AlignLeft);
        //this->memScratch->move(305,10);
        this->memScratch->show();

        connect(this->memScratch, SIGNAL(stopSimSig()), this, SLOT(stopSimSlot()));
    }
    else
    {
        if (this->memScratch != NULL
           && (dynamic_cast<MCUSimMemory*>(m_simControlUnit->getSimSubsys(MCUSimSubsys::SubsysId::ID_MEM_DATA)))->size() == 0 )
        {
            qDebug() << "PicoBlazeGrid: delete scratch";
            disconnect(this->memScratch, SIGNAL(stopSimSig()), this, SLOT(stopSimSlot()));
            delete this->memScratch;
            this->memScratch = NULL;
            //delete this->lblScratch;
            //this->lblScratch = NULL;


            //this->memPorts->move(520 - 215, 19);
            //this->memStack->move(960 - 215, 20);
            //this->wTime->move(1080 - 215, 100);
            //this->lblPortsIn->move(560 - 215, 0);
            //this->lblPortsOut->move(770 - 215, 0);
            //this->lblRD->move(710 - 215, 0);
            //if (MCUSim::FAMILY_KCPSM6 == m_simControlUnit->getFamily())
            //{
            //    this->lblWRK->move(890 - 215, 0);
            //}
            //this->lblWR->move(925 - 215, 0);
            //this->lblStack->move(960 - 215, 0);
            //this->lblPC->move(1080 - 215, 0);
//             this->lblCycles->move(1080 - 215, 20);
//             this->lblClock->move(1080 - 215, 40);
//             this->lblTime->move(1080 - 215, 80);
//             this->leSP->move(1010 - 215, 0);
//             this->lePC->move(1125 - 215, 0);
//             this->leCycles->move(1125 - 215, 20);
//             this->leClock->move(1125 - 215, 40);
//             this->cmbClock->move(1125 - 215, 60);
//             this->btnIntr->move(1080 - 215, 180);
//             this->btnCarry->move(1080 - 215, 160);
//             this->btnZero->move(1143 - 215, 160);
//             this->btnInte->move(1080 - 215, 200);
        }
    }
    //qDebug() << "PicoBlazeGrid: grid changed";
    
    
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
    //this->leSP->setText("0x00");
    this->wTime->setTime(0);
    this->interruptSet = false;
    //this->leClock->setText("10.0");
    //this->clock = 10.0;
    //this->clockMult = 1000000;
    this->leCycles->setText("0");
    this->setWarningOpt(GuiCfg::getInstance().getWarningsOpt());
    this->unhighlight();
    
    //qDebug() << "PicoBlazeGrid: return deviceReset()";
}


void PicoBlazeGrid::setReadOnly(bool /*readOnly*/)
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
    //qDebug() << "PicoBlazeGrid: unhighlight";
    this->lePC->setStyleSheet("background-color: none");
    //this->leSP->setStyleSheet("background-color: none");
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
    if (true == this->interruptSet)
    {
        this->btnIntr->setStyleSheet("color: #00ff00");
    }
    else
    {
        this->btnIntr->setStyleSheet("color: none");
    }
    //this->lblRD->setStyleSheet("color: none");
    //this->lblWR->setStyleSheet("color: none");
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
    this->btnIntr->setStyleSheet("color: #00ff00");
    this->interruptSet = true;
    /*if (m_flags->getInte() == true)
    {
        this->btnInte->setStyleSheet("color: none");
        m_flags->setInte(false);
    }
    else
    {
        this->btnInte->setStyleSheet("color: #00ff00");
        m_flags->setInte(true);
    }*/
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

    /*value = m_stack->getSP();
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
    }*/

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
    this->memRegs->setWarningOpt(options);
}


bool PicoBlazeGrid::eventFilter(QObject *target, QEvent *event)
{
    if (target == this->lePC)
    {
        if (QEvent::MouseButtonPress == event->type())
        {
            this->lePC->setStyleSheet("background-color: none");
        }
        //qDebug() << "PicoBlazeGrid: lePC event";
        return true;
    }
    return QWidget::eventFilter(target, event);
}