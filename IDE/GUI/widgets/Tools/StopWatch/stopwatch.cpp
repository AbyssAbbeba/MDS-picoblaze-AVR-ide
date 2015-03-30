#include "stopwatch.h"
#include "core.h"
#include "ui_stopwatch.h"
#include <QtGui>

StopWatch::StopWatch(QWidget *parent, MCUSimControl *controlUnit) :
    QWidget(parent),
    ui(new Ui::StopWatch)
{
    m_simControl = controlUnit;
    ui->setupUi(this);  
    this->setWindowTitle("Stopwatch");
    connectSignals();

    
    std::vector<int> mask;
    mask =  {
                MCUSimCPU::EVENT_CPU_CALL,
                MCUSimCPU::EVENT_CPU_RETURN,
                MCUSimCPU::EVENT_CPU_IRQ,
                MCUSimCPU::EVENT_CPU_RETURN_FROM_ISR
            };
    m_simControl->registerObserver(this, MCUSimSubsys::ID_CPU, mask);


    connect(m_simControlUnit, SIGNAL(updateRequest(int)), this, SLOT(handleUpdateRequest(int)));
    connect(m_simControlUnit, SIGNAL(breakpointReached()), this, SLOT(breakpointReachedSlot()));

    
}

StopWatch::~StopWatch()
{
    delete ui;
}


void StopWatch::saveAsText()
{

}

void StopWatch::clearStop()
{
    ui->lineNano2->setText("0");
    core.structPtrStop->nanoSecs = 0;

    ui->lineBreak2->setText("0");
    core.structPtrStop->breakpoints = 0;

    ui->lineCalls2->setText("0");
    core.structPtrStop->subPrograms = 0;

    ui->lineCycles2->setText("0");
    core.structPtrStop->clockCycles = 0;

    ui->lineInstr2->setText("0");
    core.structPtrStop->instructions = 0;

    ui->lineInterrupt2->setText("0");
    core.structPtrStop->interrupts = 0;

    ui->lineIntRet2->setText("0");
    core.structPtrStop->interruptReturns = 0;

    ui->lineProgram2->setText("0");
    core.structPtrStop->programBytes= 0;

    ui->lineReturns2->setText("0");
    core.structPtrStop->returns = 0;

}
void StopWatch::clearCurrent()
{
    ui->lineNano->setText("0");
    core.structPtrCurrent->nanoSecs = 0;

    ui->lineBreak->setText("0");
    core.structPtrCurrent->breakpoints = 0;

    ui->lineCalls->setText("0");
    core.structPtrCurrent->subPrograms = 0;

    ui->lineCycles->setText("0");
    core.structPtrCurrent->clockCycles = 0;

    ui->lineInstr->setText("0");
    core.structPtrCurrent->instructions = 0;

    ui->lineInterrupt->setText("0");
    core.structPtrCurrent->interrupts = 0;

    ui->lineIntRet->setText("0");
    core.structPtrCurrent->interruptReturns = 0;

    ui->lineProgram->setText("0");
    core.structPtrCurrent->programBytes= 0;

    ui->lineReturns->setText("0");
    core.structPtrCurrent->returns = 0;
}

void StopWatch::updateCore()
{

}

void StopWatch::update(int i)
{
    switch ( i )
    {
        case 1:
            ui->lineNano->setText(QString::number(core.getData(0),10));
            ui->lineNano3->setText(QString::number(core.getData(1),10));
        break;
    //    core.cntCurrent.nanoSecs
        case 2:
            ui->lineCycles->setText(QString::number(core.getData(2),10));
            ui->lineCycles3->setText(QString::number(core.getData(3),10));
            ui->lineInstr->setText(QString::number(core.getData(4),10));
            ui->lineInstr3->setText(QString::number(core.getData(5),10));
        break;
        case 3:
            ui->lineProgram->setText(QString::number(core.getData(6),10));
            ui->lineProgram3->setText(QString::number(core.getData(7),10));
        break;
        case 4:
            ui->lineInterrupt->setText(QString::number(core.getData(8),10));
            ui->lineInterrupt3->setText(QString::number(core.getData(9),10));
        break;
        case 5:
            ui->lineCalls->setText(QString::number(core.getData(10),10));
            ui->lineCalls3->setText(QString::number(core.getData(11),10));
        break;
        case 6:
            ui->lineReturns->setText(QString::number(core.getData(12),10));
            ui->lineReturns3->setText(QString::number(core.getData(13),10));
        break;
        case 7:
            ui->lineIntRet->setText(QString::number(core.getData(14),10));
            ui->lineIntRet3->setText(QString::number(core.getData(15),10));
        break;
        case 8:
            ui->lineBreak->setText(QString::number(core.getData(16),10));
            ui->lineBreak3->setText(QString::number(core.getData(17),10));
        break;
        }
    qDebug() << core.isCoreStoped();
}

void StopWatch::setStop()
{
    bool flag;
    ui->labelStahp->setText("STAHP");
    flag = core.getShutDownStatus();
    qDebug() << core.getShutDownStatus();
    flag = !flag;
    core.shutDown(flag);

}

void StopWatch::readButton(int button)
{
    switch ( button )
    {
        case 0: setStop();          break;
        case 1: saveAsText();       break;
        case 2: clearCurrent();     break;
        case 3: clearStop();        break;
        // clear buttons for line edits
        case 4: ui->lineNano->setText("0");
                updateCore();
                core.structPtrCurrent->nanoSecs = 0;
            break;
        case 5: ui->lineNano2->setText("0");
                updateCore();
                core.structPtrStop->nanoSecs = 0;
            break;
        case 7: ui->lineBreak->setText("0");
                updateCore();
                core.structPtrCurrent->breakpoints = 0;
            break;
        case 8: ui->lineBreak2->setText("0");
                updateCore();
                core.structPtrStop->breakpoints = 0;
            break;
        case 10: ui->lineCalls->setText("0");
                updateCore();
                core.structPtrCurrent->subPrograms = 0;
            break;
        case 11: ui->lineCalls2->setText("0");
                updateCore();
                core.structPtrStop->subPrograms = 0;
            break;
        case 13: ui->lineCycles->setText("0");
                updateCore();
                core.structPtrCurrent->clockCycles = 0;
            break;
        case 14: ui->lineCycles2->setText("0");
                updateCore();
                core.structPtrStop->clockCycles= 0;
            break;
        case 16: ui->lineInstr->setText("0");
                updateCore();
                core.structPtrCurrent->instructions = 0;
            break;
        case 17: ui->lineInstr2->setText("0");
                updateCore();
                core.structPtrStop->instructions= 0;
            break;
        case 19: ui->lineInterrupt->setText("0");
                updateCore();
                core.structPtrCurrent->interrupts = 0;
            break;
        case 20: ui->lineInterrupt2->setText("0");
                updateCore();
                core.structPtrStop->interrupts = 0;
            break;
        case 22: ui->lineIntRet->setText("0");
                updateCore();
                core.structPtrCurrent->interruptReturns = 0;
            break;
        case 23: ui->lineIntRet2->setText("0");
                updateCore();
                core.structPtrStop->interruptReturns = 0;
            break;
        case 25: ui->lineProgram->setText("0");
                updateCore();
                core.structPtrCurrent->programBytes= 0;
            break;
        case 26: ui->lineProgram2->setText("0");
                updateCore();
                core.structPtrStop->programBytes= 0;
            break;
        case 28: ui->lineReturns->setText("0");
                updateCore();
                core.structPtrCurrent->returns = 0;
            break;
        case 29: ui->lineReturns2->setText("0");
                updateCore();
                core.structPtrStop->returns = 0;
            break;
    }
}


void StopWatch::connectSignals()
{
    signalMapper = new QSignalMapper(this);
    signalMapper->setMapping(ui->pushStart, 0);
    signalMapper->setMapping(ui->pushSave, 1);
    signalMapper->setMapping(ui->pushClearCur, 2);
    signalMapper->setMapping(ui->pushClearStop, 3);
    // line 1
    signalMapper->setMapping(ui->pushNano, 4);
    signalMapper->setMapping(ui->pushNano2, 5);
    // line 2
    signalMapper->setMapping(ui->pushBreak, 7);
    signalMapper->setMapping(ui->pushBreak2, 8);
    // line 3
    signalMapper->setMapping(ui->pushCalls, 10);
    signalMapper->setMapping(ui->pushCalls2, 11);
    // line 4
    signalMapper->setMapping(ui->pushCycles, 13);
    signalMapper->setMapping(ui->pushCycles2, 14);
    // line 5
    signalMapper->setMapping(ui->pushInstr, 16);
    signalMapper->setMapping(ui->pushInstr2, 17);
    // line 6
    signalMapper->setMapping(ui->pushInterrupt, 19);
    signalMapper->setMapping(ui->pushInterrupt2, 20);
    // line 7
    signalMapper->setMapping(ui->pushIntRet, 22);
    signalMapper->setMapping(ui->pushIntRet2, 23);
    // line 8
    signalMapper->setMapping(ui->pushProgram, 25);
    signalMapper->setMapping(ui->pushProgram2, 26);
    // line 9
    signalMapper->setMapping(ui->pushReturns, 28);
    signalMapper->setMapping(ui->pushReturns2, 29);

    connect(ui->pushStart,          SIGNAL(clicked()), signalMapper, SLOT(map()));
    connect(ui->pushSave,          SIGNAL(clicked()), signalMapper, SLOT(map()));
    connect(ui->pushClearCur,          SIGNAL(clicked()), signalMapper, SLOT(map()));
    connect(ui->pushClearStop,          SIGNAL(clicked()), signalMapper, SLOT(map()));
    connect(ui->pushNano,          SIGNAL(clicked()), signalMapper, SLOT(map()));
    connect(ui->pushNano2,          SIGNAL(clicked()), signalMapper, SLOT(map()));
    connect(ui->pushBreak,          SIGNAL(clicked()), signalMapper, SLOT(map()));
    connect(ui->pushBreak2,          SIGNAL(clicked()), signalMapper, SLOT(map()));
    connect(ui->pushCalls,          SIGNAL(clicked()), signalMapper, SLOT(map()));
    connect(ui->pushCalls2,          SIGNAL(clicked()), signalMapper, SLOT(map()));
    connect(ui->pushCycles,          SIGNAL(clicked()), signalMapper, SLOT(map()));
    connect(ui->pushCycles2,          SIGNAL(clicked()), signalMapper, SLOT(map()));
    connect(ui->pushInstr,          SIGNAL(clicked()), signalMapper, SLOT(map()));
    connect(ui->pushInstr2,          SIGNAL(clicked()), signalMapper, SLOT(map()));
    connect(ui->pushInterrupt,          SIGNAL(clicked()), signalMapper, SLOT(map()));
    connect(ui->pushInterrupt2,          SIGNAL(clicked()), signalMapper, SLOT(map()));
    connect(ui->pushIntRet,          SIGNAL(clicked()), signalMapper, SLOT(map()));
    connect(ui->pushIntRet2,          SIGNAL(clicked()), signalMapper, SLOT(map()));
    connect(ui->pushProgram,          SIGNAL(clicked()), signalMapper, SLOT(map()));
    connect(ui->pushProgram2,          SIGNAL(clicked()), signalMapper, SLOT(map()));
    connect(ui->pushReturns,            SIGNAL(clicked()), signalMapper, SLOT(map()));
    connect(ui->pushReturns2,           SIGNAL(clicked()), signalMapper, SLOT(map()));
    connect(signalMapper, SIGNAL(mapped(int)),this, SLOT(readButton(int)));

    valueValidator =    new QRegExpValidator(QRegExp("[0-9]{50}"), this);

    ui->lineBreak->setValidator(valueValidator);
    ui->lineNano->setValidator(valueValidator);
    ui->lineNano2->setValidator(valueValidator);
    ui->lineNano3->setValidator(valueValidator);
    ui->lineBreak->setValidator(valueValidator);
    ui->lineBreak2->setValidator(valueValidator);
    ui->lineBreak3->setValidator(valueValidator);
    ui->lineCalls->setValidator(valueValidator);
    ui->lineCalls2->setValidator(valueValidator);
    ui->lineCalls3->setValidator(valueValidator);
    ui->lineCycles->setValidator(valueValidator);
    ui->lineCycles2->setValidator(valueValidator);
    ui->lineCycles3->setValidator(valueValidator);
    ui->lineInstr->setValidator(valueValidator);
    ui->lineInstr2->setValidator(valueValidator);
    ui->lineInstr3->setValidator(valueValidator);
    ui->lineInterrupt->setValidator(valueValidator);
    ui->lineInterrupt2->setValidator(valueValidator);
    ui->lineInterrupt3->setValidator(valueValidator);
    ui->lineIntRet->setValidator(valueValidator);
    ui->lineIntRet2->setValidator(valueValidator);
    ui->lineIntRet3->setValidator(valueValidator);
    ui->lineProgram->setValidator(valueValidator);
    ui->lineProgram2->setValidator(valueValidator);
    ui->lineProgram3->setValidator(valueValidator);
    ui->lineReturns->setValidator(valueValidator);
    ui->lineReturns2->setValidator(valueValidator);
    ui->lineReturns3->setValidator(valueValidator);


    //ui->push// cancel.png, breakpoint_disable.png
    ui->pushBreak->setIcon(QPixmap(":/resources/icons/breakpoint_disable.png"));
    ui->pushBreak2->setIcon(QPixmap(":/resources/icons/breakpoint_disable.png"));
    ui->pushCalls->setIcon(QPixmap(":/resources/icons/breakpoint_disable.png"));
    ui->pushCalls2->setIcon(QPixmap(":/resources/icons/breakpoint_disable.png"));
    ui->pushCycles->setIcon(QPixmap(":/resources/icons/breakpoint_disable.png"));
    ui->pushCycles2->setIcon(QPixmap(":/resources/icons/breakpoint_disable.png"));
    ui->pushInstr->setIcon(QPixmap(":/resources/icons/breakpoint_disable.png"));
    ui->pushInstr2->setIcon(QPixmap(":/resources/icons/breakpoint_disable.png"));
    ui->pushInterrupt->setIcon(QPixmap(":/resources/icons/breakpoint_disable.png"));
    ui->pushInterrupt2->setIcon(QPixmap(":/resources/icons/breakpoint_disable.png"));
    ui->pushIntRet->setIcon(QPixmap(":/resources/icons/breakpoint_disable.png"));
    ui->pushIntRet2->setIcon(QPixmap(":/resources/icons/breakpoint_disable.png"));
    ui->pushNano->setIcon(QPixmap(":/resources/icons/breakpoint_disable.png"));
    ui->pushNano2->setIcon(QPixmap(":/resources/icons/breakpoint_disable.png"));
    ui->pushProgram->setIcon(QPixmap(":/resources/icons/breakpoint_disable.png"));
    ui->pushProgram2->setIcon(QPixmap(":/resources/icons/breakpoint_disable.png"));
    ui->pushReturns->setIcon(QPixmap(":/resources/icons/breakpoint_disable.png"));
    ui->pushReturns2->setIcon(QPixmap(":/resources/icons/breakpoint_disable.png"));
    ui->pushSave->setIcon(QPixmap(":/resources/icons/disk.png"));
    ui->pushStart->setIcon(QPixmap(":/resources/icons/bullet_arrow_right.png"));
    ui->pushClearStop->setIcon(QPixmap(":/resources/icons/cancel.png"));
    ui->pushClearCur->setIcon(QPixmap(":/resources/icons/cancel.png"));

}


void StopWatch::on_lineNano2_textChanged(const QString &arg1)
{
    core.structPtrStop->nanoSecs = arg1.toULongLong(0,10);
}

void StopWatch::on_lineCycles2_textChanged(const QString &arg1)
{
    core.structPtrStop->clockCycles = arg1.toULongLong(0,10);
}

void StopWatch::on_lineInstr2_textChanged(const QString &arg1)
{
    core.structPtrStop->instructions = arg1.toULongLong(0,10);
}

void StopWatch::on_lineProgram2_textChanged(const QString &arg1)
{
    core.structPtrStop->programBytes = arg1.toULongLong(0,10);
}

void StopWatch::on_lineInterrupt2_textChanged(const QString &arg1)
{
    core.structPtrStop->interrupts = arg1.toULongLong(0,10);
}

void StopWatch::on_lineCalls2_textChanged(const QString &arg1)
{
    core.structPtrStop->subPrograms = arg1.toULongLong(0,10);
}

void StopWatch::on_lineReturns2_textChanged(const QString &arg1)
{
    core.structPtrStop->returns = arg1.toULongLong(0,10);
}

void StopWatch::on_lineIntRet2_textChanged(const QString &arg1)
{
    core.structPtrStop->interruptReturns = arg1.toULongLong(0,10);
}

void StopWatch::on_lineBreak2_textChanged(const QString &arg1)
{
    core.structPtrStop->breakpoints = arg1.toULongLong(0,10);
}

void StopWatch::breakpointReachedSlot()
{
    core.addBreakPoint();
    if ( core.getShutDownStatus() )
    update(8);
}

void StopWatch::handleUpdateRequest(int mask)
{
    qDebug() << "--------------------";
    if (1 & mask)
    {
        //this->leTime->setText(QString::number(m_simControlUnit->getTotalMCycles()));
        //this->leTime->setStyleSheet("background-color: yellow");
        unsigned long long cycles;
        cycles = m_simControlUnit->getTotalMCycles();
        qDebug() << cycles;

        core.addClockCycle();
        if ( core.getShutDownStatus() )
        update(2);
        //this->wTime->setTime(2*cycles/(clock*clockMult));
        //this->leCycles->setText(QString::number(2*cycles, 10));
    }
}


void StopWatch::handleEvent(int subsysId, int eventId, int locationOrReason, int detail)
{
    if (MCUSimSubsys::ID_CPU == subsysId)
    {
        qDebug() << "CallWatcher: ID_CPU event";
        switch ( eventId )
        {
            case MCUSimCPU::EVENT_CPU_CALL:
            {
                core.addSubProg();

                qDebug() << core.getShutDownStatus();
                if ( core.getShutDownStatus() )
                update(5);
                break;
            }
            case MCUSimCPU::EVENT_CPU_RETURN:
            {
                core.addReturn();
                if ( core.getShutDownStatus() )
                update(6);
                break;
            }
            case MCUSimCPU::EVENT_CPU_IRQ:
            {
                core.addInterrupt();
                if ( core.getShutDownStatus() )
                update(4);
                break;
            }
            case MCUSimCPU::EVENT_CPU_RETURN_FROM_ISR:
            {
                core.addInterruptReturn();
                if ( core.getShutDownStatus() )
                update(7);
                break;
            }
            default:
            {
                break;
            }
        }
    }
}


void StopWatch::deviceChanged()
{
}


void StopWatch::deviceReset()
{
}


void StopWatch::setReadOnly(bool) //readOnly)
{
}

// void TimeWidget::setTime ( long double time )
// {
//     long long unsigned int picosec = round ( time * 1000000000000.0 );
// 
//     ui.lblMin -> setText ( QString::number ( picosec / 60000000000000ULL          , 10 ) );
//     ui.lblSec -> setText ( QString::number ( picosec /  1000000000000ULL %   60ULL, 10 ) );
//     ui.lblMs  -> setText ( QString::number ( picosec /     1000000000ULL % 1000ULL, 10 ) );
//     ui.lblUs  -> setText ( QString::number ( picosec /        1000000ULL % 1000ULL, 10 ) );
//     ui.lblNs  -> setText ( QString::number ( picosec /           1000ULL % 1000ULL, 10 ) );
//     ui.lblPs  -> setText ( QString::number ( picosec                     % 1000ULL, 10 ) );
// }

