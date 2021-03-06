#include "stopwatch.h"
#include "core.h"
#include "ui_stopwatch.h"
#include <QtGui>

StopWatch::StopWatch(QWidget *parent, MCUSimControl *controlUnit) :
    QWidget(parent),
    ui(new Ui::StopWatch)
{
    m_simControlUnit = controlUnit;
    ui->setupUi(this);  

    this->setWindowTitle("Stopwatch");
    this->setWindowFlags(Qt::WindowStaysOnTopHint);
    connectSignals();

    ui->lineProgram->setVisible(false);
    ui->lineProgram2->setVisible(false);
    ui->lineProgram3->setVisible(false);
    ui->pushProgram->setVisible(false);
    ui->pushProgram2->setVisible(false);
    ui->label_8->setVisible(false);

    std::vector<int> mask;
    mask =  {
                MCUSimCPU::EVENT_CPU_CALL,
                MCUSimCPU::EVENT_CPU_RETURN,
                MCUSimCPU::EVENT_CPU_IRQ,
                MCUSimCPU::EVENT_CPU_RETURN_FROM_ISR
            };

    m_simControlUnit->registerObserver(this, MCUSimSubsys::ID_CPU, mask);

    connect(m_simControlUnit, SIGNAL(updateRequest(int)), this, SLOT(handleUpdateRequest(int)));
    connect(m_simControlUnit, SIGNAL(breakpointReached()), this, SLOT(breakpointReachedSlot()));
    connect(m_simControlUnit, SIGNAL(quotaReached(int)), this, SLOT(quotaReachedSlot(int)));

    ui->labelStahp->setStyleSheet("QLabel { color : red }");
    if ( core.getShutDownStatus() == false )
    {
        ui->labelStahp->setText("");
        ui->pushStart->setIcon(QPixmap(":/resources/icons/pause.png"));
    }
    else
    {
        ui->labelStahp->setText("STOPPED");
        ui->pushStart->setIcon(QPixmap(":/resources/icons/bullet_arrow_right.png"));
    }
    qDebug() << m_simControlUnit->quotasEnabled() << "<< QUOTAS";
    //qDebug() << m_simControl->getDeviceName();
    //ui->labelDevice->setText(m_simControl->getDeviceName());

}

StopWatch::~StopWatch()
{
    delete ui;
}

void StopWatch::quotaReachedSlot(int Quota)
{
    // 0 = cycles, 1 = interrupt, 2 = subroutines,
    // 3 = returns, 4 = int ret, 5 = breakpoints
    switch ( Quota)
    {
        case 0:
            m_simControlUnit->setQuota(MCUSimControl::QTP_CYCLES,-1);
            qDebug() << "aa";
            break;
    }
}


void StopWatch::saveAsText()
{
    outPath.clear();
    outPath = QFileDialog::getSaveFileName(this,"Output file","", tr("Text document (*.txt);; All files (*.*)"));
    if ( outPath == NULL)
        return;
    outFile.setFileName(outPath);
    //QFile outFile("C:/hratky_s_qt/VHDLgen/testsoubory/xml1.txt");
    outFile.open(QIODevice::WriteOnly | QIODevice::Text);
    QTextStream out(&outFile);
    qDebug() << outPath;

    // print to file
    for ( int i = 0; i < 6; i++)
    {
        out << "\t";
    }
    out << "Current" << "\t" << "\t" << "Stop after" << "\t" << "Overall" << endl;
    out << "Nano seconds"<< "\t"<< "\t" << "\t" << "\t" << "\t" << QString::number(core.structPtrCurrent->nanoSecs, 10)
        << "\t" << "\t" << QString::number(core.structPtrStop->nanoSecs, 10)
        << "\t" << "\t" << QString::number(core.structPtrOverall->nanoSecs, 10) << endl;
    out << "Clock cycles" << "\t"<< "\t" << "\t" << "\t" << "\t" << QString::number(core.structPtrCurrent->clockCycles, 10)
        << "\t" << "\t" << QString::number(core.structPtrStop->clockCycles, 10)
        << "\t" << "\t" << QString::number(core.structPtrOverall->clockCycles, 10) << endl;
    out << "Instruction cycles" << "\t" << "\t" << "\t" << "\t" << QString::number(core.structPtrCurrent->instructions, 10)
        << "\t" << "\t" << QString::number(core.structPtrStop->instructions, 10)
        << "\t" << "\t" << QString::number(core.structPtrOverall->instructions, 10) << endl;
    out << "Program bytes"<< "\t"<< "\t" << "\t" << "\t" << "\t" << QString::number(core.structPtrCurrent->programBytes, 10)
        << "\t" << "\t" << QString::number(core.structPtrStop->programBytes, 10)
        << "\t" << "\t" << QString::number(core.structPtrOverall->programBytes, 10) << endl;
    out << "Interrupts"<< "\t"<< "\t" << "\t" << "\t" << "\t" << QString::number(core.structPtrCurrent->interrupts, 10)
        << "\t" << "\t" << QString::number(core.structPtrStop->interrupts, 10)
        << "\t" << "\t" << QString::number(core.structPtrOverall->interrupts, 10) << endl;
    out << "Subprogram calls" << "\t" << "\t" << "\t" << "\t" << QString::number(core.structPtrCurrent->subPrograms, 10)
        << "\t" << "\t" << QString::number(core.structPtrStop->subPrograms, 10)
        << "\t" << "\t" << QString::number(core.structPtrOverall->subPrograms, 10) << endl;
    out << "Returns" << "\t" << "\t" << "\t" << "\t" << "\t" << "\t" << QString::number(core.structPtrCurrent->returns, 10)
        << "\t" << "\t" << QString::number(core.structPtrStop->returns, 10)
        << "\t" << "\t" << QString::number(core.structPtrOverall->returns, 10) << endl;
    out << "Interrupt returns" << "\t" << "\t" << "\t" << "\t" << QString::number(core.structPtrCurrent->interruptReturns, 10)
        << "\t" << "\t" << QString::number(core.structPtrStop->interruptReturns, 10)
        << "\t" << "\t" << QString::number(core.structPtrOverall->interruptReturns, 10) << endl;
    out << "Breakpoints" << "\t" << "\t"<< "\t" << "\t" << "\t" << QString::number(core.structPtrCurrent->breakpoints, 10)
        << "\t" << "\t" << QString::number(core.structPtrStop->breakpoints, 10)
        << "\t" << "\t" << QString::number(core.structPtrOverall->breakpoints, 10) << endl;
    out << "Stopwatch, " << endl;
    out << "Generated by MDS  ( www.moravia-microsystems.com )";
    qDebug() << "y";
    outFile.close();
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
    core.clockDif += core.structPtrCurrent->clockCycles;
    core.structPtrCurrent->clockCycles = 0;

    ui->lineInstr->setText("0");
    core.instrDif += core.structPtrCurrent->instructions;
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
}

void StopWatch::setStop()
{
    bool flag;
    flag = core.getShutDownStatus();
    flag = !flag;
    core.shutDown(flag);

    if ( core.getShutDownStatus() == false )
    {
      //  emit stopSim();
        ui->labelStahp->setText("");
        ui->pushStart->setIcon(QPixmap(":/resources/icons/pause.png"));
    }
    else
    {
        ui->labelStahp->setText("STOPPED");
        ui->pushStart->setIcon(QPixmap(":/resources/icons/bullet_arrow_right.png"));
    }

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
                core.clockDif += core.structPtrCurrent->clockCycles;
                core.structPtrCurrent->clockCycles = 0;
            break;
        case 14: ui->lineCycles2->setText("0");
                updateCore();
                core.structPtrStop->clockCycles= 0;
            break;
        case 16: ui->lineInstr->setText("0");
                updateCore();
                core.instrDif += core.structPtrCurrent->instructions;
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



bool StopWatch::eventFilter(QObject *obj, QEvent *event)
{
    if(event->type() == QEvent::MouseButtonPress)
    {
        ui->lineBreak->setStyleSheet("QLineEdit{background: white;}");
        ui->lineNano->setStyleSheet("QLineEdit{background: white;}");
        ui->lineNano2->setStyleSheet("QLineEdit{background: white;}");
        ui->lineNano3->setStyleSheet("QLineEdit{background: white;}");
        ui->lineBreak->setStyleSheet("QLineEdit{background: white;}");
        ui->lineBreak2->setStyleSheet("QLineEdit{background: white;}");
        ui->lineBreak3->setStyleSheet("QLineEdit{background: white;}");
        ui->lineCalls->setStyleSheet("QLineEdit{background: white;}");
        ui->lineCalls2->setStyleSheet("QLineEdit{background: white;}");
        ui->lineCalls3->setStyleSheet("QLineEdit{background: white;}");
        ui->lineCycles->setStyleSheet("QLineEdit{background: white;}");
        ui->lineCycles2->setStyleSheet("QLineEdit{background: white;}");
        ui->lineCycles3->setStyleSheet("QLineEdit{background: white;}");
        ui->lineInstr->setStyleSheet("QLineEdit{background: white;}");
        ui->lineInstr2->setStyleSheet("QLineEdit{background: white;}");
        ui->lineInstr3->setStyleSheet("QLineEdit{background: white;}");
        ui->lineInterrupt->setStyleSheet("QLineEdit{background: white;}");
        ui->lineInterrupt2->setStyleSheet("QLineEdit{background: white;}");
        ui->lineInterrupt3->setStyleSheet("QLineEdit{background: white;}");
        ui->lineIntRet->setStyleSheet("QLineEdit{background: white;}");
        ui->lineIntRet2->setStyleSheet("QLineEdit{background: white;}");
        ui->lineIntRet3->setStyleSheet("QLineEdit{background: white;}");
        ui->lineProgram->setStyleSheet("QLineEdit{background: white;}");
        ui->lineProgram2->setStyleSheet("QLineEdit{background: white;}");
        ui->lineProgram3->setStyleSheet("QLineEdit{background: white;}");
        ui->lineReturns->setStyleSheet("QLineEdit{background: white;}");
        ui->lineReturns2->setStyleSheet("QLineEdit{background: white;}");
        ui->lineReturns3->setStyleSheet("QLineEdit{background: white;}");

        if ( obj == ui->lineNano3)
        {
            ui->lineNano3->setStyleSheet("QLineEdit{background: #46c6e2;}");
            ui->lineNano2->setStyleSheet("QLineEdit{background: #b0e0e6;}");
            ui->lineNano->setStyleSheet("QLineEdit{background: #b0e0e6;}");
            return true;
        }
        if ( obj == ui->lineBreak3)
        {
            ui->lineBreak3->setStyleSheet("QLineEdit{background: #46c6e2;}");
            ui->lineBreak2->setStyleSheet("QLineEdit{background: #b0e0e6;}");
            ui->lineBreak->setStyleSheet("QLineEdit{background: #b0e0e6;}");
            return true;
        }
        if ( obj == ui->lineCalls3)
        {
            ui->lineCalls3->setStyleSheet("QLineEdit{background: #46c6e2;}");
            ui->lineCalls2->setStyleSheet("QLineEdit{background: #b0e0e6;}");
            ui->lineCalls->setStyleSheet("QLineEdit{background: #b0e0e6;}");
            return true;
        }
        if ( obj == ui->lineReturns3)
        {
            ui->lineReturns3->setStyleSheet("QLineEdit{background: #46c6e2;}");
            ui->lineReturns2->setStyleSheet("QLineEdit{background: #b0e0e6;}");
            ui->lineReturns->setStyleSheet("QLineEdit{background: #b0e0e6;}");
            return true;
        }
        if ( obj == ui->lineCycles3)
        {
            ui->lineCycles3->setStyleSheet("QLineEdit{background: #46c6e2;}");
            ui->lineCycles2->setStyleSheet("QLineEdit{background: #b0e0e6;}");
            ui->lineCycles->setStyleSheet("QLineEdit{background: #b0e0e6;}");
            return true;
        }
        if ( obj == ui->lineInstr3)
        {
            ui->lineInstr3->setStyleSheet("QLineEdit{background: #46c6e2;}");
            ui->lineInstr2->setStyleSheet("QLineEdit{background: #b0e0e6;}");
            ui->lineInstr->setStyleSheet("QLineEdit{background: #b0e0e6;}");
            return true;
        }
        if ( obj == ui->lineInterrupt3)
        {
            ui->lineInterrupt3->setStyleSheet("QLineEdit{background: #46c6e2;}");
            ui->lineInterrupt2->setStyleSheet("QLineEdit{background: #b0e0e6;}");
            ui->lineInterrupt->setStyleSheet("QLineEdit{background: #b0e0e6;}");
            return true;
        }
        if ( obj == ui->lineIntRet3)
        {
            ui->lineIntRet3->setStyleSheet("QLineEdit{background: #46c6e2;}");
            ui->lineIntRet2->setStyleSheet("QLineEdit{background: #b0e0e6;}");
            ui->lineIntRet->setStyleSheet("QLineEdit{background: #b0e0e6;}");
            return true;
        }
        if ( obj == ui->lineProgram3)
        {
            ui->lineProgram3->setStyleSheet("QLineEdit{background: #46c6e2;}");
            ui->lineProgram2->setStyleSheet("QLineEdit{background: #b0e0e6;}");
            ui->lineProgram->setStyleSheet("QLineEdit{background: #b0e0e6;}");
            return true;
        }
//           uuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuu
        if ( obj == ui->lineReturns)
        {
            ui->lineReturns->setStyleSheet("QLineEdit{background: #46c6e2;}");
            ui->lineReturns2->setStyleSheet("QLineEdit{background: #b0e0e6;}");
            ui->lineReturns3->setStyleSheet("QLineEdit{background: #b0e0e6;}");
            return true;
        }
        if ( obj == ui->lineNano2)
        {
            ui->lineNano2->setStyleSheet("QLineEdit{background: #46c6e2;}");
            ui->lineNano->setStyleSheet("QLineEdit{background: #b0e0e6;}");
            ui->lineNano3->setStyleSheet("QLineEdit{background: #b0e0e6;}");
        }
        if ( obj == ui->lineReturns)
        {
            ui->lineReturns->setStyleSheet("QLineEdit{background: #46c6e2;}");
            ui->lineReturns2->setStyleSheet("QLineEdit{background: #b0e0e6;}");
            ui->lineReturns3->setStyleSheet("QLineEdit{background: #b0e0e6;}");
            return true;
        }
        if ( obj == ui->lineReturns2)
        {
            ui->lineReturns2->setStyleSheet("QLineEdit{background: #46c6e2;}");
            ui->lineReturns->setStyleSheet("QLineEdit{background: #b0e0e6;}");
            ui->lineReturns3->setStyleSheet("QLineEdit{background: #b0e0e6;}");
            return true;
        }
        if ( obj == ui->lineProgram)
        {
            ui->lineProgram->setStyleSheet("QLineEdit{background: #46c6e2;}");
            ui->lineProgram2->setStyleSheet("QLineEdit{background: #b0e0e6;}");
            ui->lineProgram3->setStyleSheet("QLineEdit{background: #b0e0e6;}");
            return true;
        }
        if ( obj == ui->lineProgram2)
        {
            ui->lineProgram2->setStyleSheet("QLineEdit{background: #46c6e2;}");
            ui->lineProgram->setStyleSheet("QLineEdit{background: #b0e0e6;}");
            ui->lineProgram3->setStyleSheet("QLineEdit{background: #b0e0e6;}");
            return true;
        }
        if ( obj == ui->lineIntRet)
        {
            ui->lineIntRet->setStyleSheet("QLineEdit{background: #46c6e2;}");
            ui->lineIntRet2->setStyleSheet("QLineEdit{background: #b0e0e6;}");
            ui->lineIntRet3->setStyleSheet("QLineEdit{background: #b0e0e6;}");
            return true;
        }
        if ( obj == ui->lineIntRet2)
        {
            ui->lineIntRet2->setStyleSheet("QLineEdit{background: #46c6e2;}");
            ui->lineIntRet->setStyleSheet("QLineEdit{background: #b0e0e6;}");
            ui->lineIntRet3->setStyleSheet("QLineEdit{background: #b0e0e6;}");
            return true;
        }
        if ( obj == ui->lineInterrupt)
        {
            ui->lineInterrupt->setStyleSheet("QLineEdit{background: #46c6e2;}");
            ui->lineInterrupt2->setStyleSheet("QLineEdit{background: #b0e0e6;}");
            ui->lineInterrupt3->setStyleSheet("QLineEdit{background: #b0e0e6;}");
            return true;
        }
        if ( obj == ui->lineInterrupt2)
        {
            ui->lineInterrupt2->setStyleSheet("QLineEdit{background: #46c6e2;}");
            ui->lineInterrupt->setStyleSheet("QLineEdit{background: #b0e0e6;}");
            ui->lineInterrupt3->setStyleSheet("QLineEdit{background: #b0e0e6;}");
            return true;
        }
        if ( obj == ui->lineInstr)
        {
            ui->lineInstr->setStyleSheet("QLineEdit{background: #46c6e2;}");
            ui->lineInstr2->setStyleSheet("QLineEdit{background: #b0e0e6;}");
            ui->lineInstr3->setStyleSheet("QLineEdit{background: #b0e0e6;}");
            return true;
        }
        if ( obj == ui->lineInstr2)
        {
            ui->lineInstr2->setStyleSheet("QLineEdit{background: #46c6e2;}");
            ui->lineInstr->setStyleSheet("QLineEdit{background: #b0e0e6;}");
            ui->lineInstr3->setStyleSheet("QLineEdit{background: #b0e0e6;}");
            return true;
        }
        if ( obj == ui->lineCycles)
        {
            ui->lineCycles->setStyleSheet("QLineEdit{background: #46c6e2;}");
            ui->lineCycles2->setStyleSheet("QLineEdit{background: #b0e0e6;}");
            ui->lineCycles3->setStyleSheet("QLineEdit{background: #b0e0e6;}");
            return true;
        }
        if ( obj == ui->lineCycles2)
        {
            ui->lineCycles2->setStyleSheet("QLineEdit{background: #46c6e2;}");
            ui->lineCycles->setStyleSheet("QLineEdit{background: #b0e0e6;}");
            ui->lineCycles3->setStyleSheet("QLineEdit{background: #b0e0e6;}");
            return true;
        }
        if ( obj == ui->lineCalls)
        {
            ui->lineCalls->setStyleSheet("QLineEdit{background: #46c6e2;}");
            ui->lineCalls2->setStyleSheet("QLineEdit{background: #b0e0e6;}");
            ui->lineCalls3->setStyleSheet("QLineEdit{background: #b0e0e6;}");
            return true;
        }
        if ( obj == ui->lineCalls2)
        {
            ui->lineCalls2->setStyleSheet("QLineEdit{background: #46c6e2;}");
            ui->lineCalls->setStyleSheet("QLineEdit{background: #b0e0e6;}");
            ui->lineCalls3->setStyleSheet("QLineEdit{background: #b0e0e6;}");
            return true;
        }
        if ( obj == ui->lineBreak)
        {
            ui->lineBreak->setStyleSheet("QLineEdit{background: #46c6e2;}");
            ui->lineBreak2->setStyleSheet("QLineEdit{background: #b0e0e6;}");
            ui->lineBreak3->setStyleSheet("QLineEdit{background: #b0e0e6;}");
            return true;
        }
        if ( obj == ui->lineBreak2)
        {
            ui->lineBreak2->setStyleSheet("QLineEdit{background: #46c6e2;}");
            ui->lineBreak->setStyleSheet("QLineEdit{background: #b0e0e6;}");
            ui->lineBreak3->setStyleSheet("QLineEdit{background: #b0e0e6;}");
            return true;
        }
        return true;
    }
    return false;
}

void StopWatch::keyPressEvent(QKeyEvent * key)
{
    if ( key->key() == Qt::Key_Return || key->key() == Qt::Key_Enter)
    {
        readButton(0);
    }
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
    if ( ui->checkSimulation->isChecked() == true )
        m_simControlUnit->setQuota(MCUSimControl::QTP_CYCLES, arg1.toInt(0,10));
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
    if ( core.getShutDownStatus() == true )
    {
        core.structPtrOverall->breakpoints += 1;
        update(8);
        return;
    }
    else
    {
        core.structPtrOverall->breakpoints += 1;
        core.addBreakPoint();
        update(8);
        if ( core.getShutDownStatus() == false )
        {
            ui->labelStahp->setText("");
            ui->pushStart->setIcon(QPixmap(":/resources/icons/pause.png"));
        }
        else
        {

            ui->labelStahp->setText("STOPPED");
            ui->pushStart->setIcon(QPixmap(":/resources/icons/bullet_arrow_right.png"));
        }
    }
}

void StopWatch::handleUpdateRequest(int mask)
{
    //qDebug() << "--------------------";
    if (1 & mask)
    {
        //this->leTime->setText(QString::number(m_simControlUnit->getTotalMCycles()));
        //this->leTime->setStyleSheet("background-color: yellow");
        unsigned long long cycles;
        //unsigned long long * ptrCycles;
       // ptrCycles = &cycles;

        cycles = m_simControlUnit->getTotalMCycles();
        qDebug() << cycles << " << clocl cycles total";
        //qDebug() << ptrCycles << "clock dif";

        core.structPtrOverall->instructions = cycles;
        core.structPtrOverall->clockCycles  = cycles * 2;
        if ( core.getShutDownStatus() == true )
        {
            update(2);
            return;
        }
        else
        {
            core.addClockCycle(cycles);
            update(2);
            if ( core.getShutDownStatus() == false )
            {
                ui->labelStahp->setText("");
                ui->pushStart->setIcon(QPixmap(":/resources/icons/pause.png"));
            }
            else
            {
                ui->labelStahp->setText("STOPPED");
                ui->pushStart->setIcon(QPixmap(":/resources/icons/bullet_arrow_right.png"));
            }
        }
        //this->wTime->setTime(2*cycles/(clock*clockMult));
        //this->leCycles->setText(QString::number(2*cycles, 10));
    }
}


void StopWatch::handleEvent(int subsysId, int eventId, int /*locationOrReason*/, int /*detail*/)
{
    if (MCUSimSubsys::ID_CPU == subsysId)
    {
        switch ( eventId )
        {
            case MCUSimCPU::EVENT_CPU_CALL:
            {
                if ( core.getShutDownStatus() == true )
                {
                    core.structPtrOverall->subPrograms += 1;
                    update(5);
                    return;
                }
                else
                {
                    core.structPtrOverall->subPrograms += 1;
                    core.addSubProg();
                    if ( core.getShutDownStatus() == false )
                    {
                        ui->labelStahp->setText("");
                        ui->pushStart->setIcon(QPixmap(":/resources/icons/pause.png"));
                    }
                    else
                    {
                        ui->labelStahp->setText("STOPPED");
                        ui->pushStart->setIcon(QPixmap(":/resources/icons/bullet_arrow_right.png"));
                    }
                    update(5);
                }
                break;
            }
            case MCUSimCPU::EVENT_CPU_RETURN:
            {
                if ( core.getShutDownStatus() == true )
                {
                    core.structPtrOverall->returns += 1;
                    update(6);
                    return;
                }
                else
                {
                    core.structPtrOverall->returns += 1;
                    core.addReturn();
                    if ( core.getShutDownStatus() == false )
                    {
                        ui->labelStahp->setText("");
                        ui->pushStart->setIcon(QPixmap(":/resources/icons/pause.png"));
                    }
                    else
                    {
                        ui->labelStahp->setText("STOPPED");
                        ui->pushStart->setIcon(QPixmap(":/resources/icons/bullet_arrow_right.png"));
                    }
                    update(6);
                }
                break;
            }
            case MCUSimCPU::EVENT_CPU_IRQ:
            {
                if ( core.getShutDownStatus() == true )
                {
                    core.structPtrOverall->interrupts += 1;
                    update(4);
                    return;
                }
                else
                {
                    core.structPtrOverall->interrupts += 1;
                    core.addInterrupt();
                    if ( core.getShutDownStatus() == false )
                    {
                        ui->labelStahp->setText("");
                        ui->pushStart->setIcon(QPixmap(":/resources/icons/pause.png"));
                    }
                    else
                    {
                        ui->labelStahp->setText("STOPPED");
                        ui->pushStart->setIcon(QPixmap(":/resources/icons/bullet_arrow_right.png"));
                    }
                    update(4);
                }
                break;
            }
            case MCUSimCPU::EVENT_CPU_RETURN_FROM_ISR:
            {
                if ( core.getShutDownStatus() == true )
                {
                    core.structPtrOverall->interruptReturns += 1;
                    update(7);
                    return;
                }
                else
                {
                    core.structPtrOverall->interruptReturns += 1;
                    core.addInterruptReturn();
                    update(7);
                    if ( core.getShutDownStatus() == false )
                    {
                        ui->labelStahp->setText("");
                        ui->pushStart->setIcon(QPixmap(":/resources/icons/pause.png"));
                    }
                    else
                    {
                        ui->labelStahp->setText("STOPPED");
                        ui->pushStart->setIcon(QPixmap(":/resources/icons/bullet_arrow_right.png"));
                    }
                }
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
    deviceReset();
}


void StopWatch::deviceReset()
{
 ui->lineNano->setText("0");

ui->lineNano2->setText("0");

   ui->lineBreak->setText("0");

 ui->lineBreak2->setText("0");

 ui->lineCalls->setText("0");

 ui->lineCalls2->setText("0");

 ui->lineCycles->setText("0");

 ui->lineCycles2->setText("0");

ui->lineInstr->setText("0");

    ui->lineInstr2->setText("0");

 ui->lineInterrupt->setText("0");

 ui->lineInterrupt2->setText("0");

     ui->lineIntRet->setText("0");

     ui->lineIntRet2->setText("0");

 ui->lineProgram->setText("0");

 ui->lineProgram2->setText("0");

ui->lineReturns->setText("0");

    ui->lineReturns2->setText("0");

    qDebug() << "device reset";
    core.structPtrCurrent->nanoSecs = 0;
    core.structPtrCurrent->clockCycles = 0;
    core.structPtrCurrent->instructions = 0;
    core.structPtrCurrent->programBytes = 0;
    core.structPtrCurrent->interrupts = 0;
    core.structPtrCurrent->subPrograms = 0;
    core.structPtrCurrent->returns = 0;
    core.structPtrCurrent->interruptReturns = 0;
    core.structPtrCurrent->breakpoints = 0;

    core.structPtrStop->nanoSecs = 0;
    core.structPtrStop->clockCycles = 0;
    core.structPtrStop->instructions = 0;
    core.structPtrStop->programBytes = 0;
    core.structPtrStop->interrupts = 0;
    core.structPtrStop->subPrograms = 0;
    core.structPtrStop->returns = 0;
    core.structPtrStop->interruptReturns = 0;
    core.structPtrStop->breakpoints = 0;

    core.structPtrOverall->nanoSecs = 0;
    core.structPtrOverall->clockCycles = 0;
    core.structPtrOverall->instructions = 0;
    core.structPtrOverall->programBytes = 0;
    core.structPtrOverall->interrupts = 0;
    core.structPtrOverall->subPrograms = 0;
    core.structPtrOverall->returns = 0;
    core.structPtrOverall->interruptReturns = 0;
    core.structPtrOverall->breakpoints = 0;
}


//lineEdit->selectAll();

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

    ui->lineBreak->installEventFilter(this);
    ui->lineNano->installEventFilter(this);
    ui->lineNano2->installEventFilter(this);
    ui->lineNano3->installEventFilter(this);
    ui->lineBreak->installEventFilter(this);
    ui->lineBreak2->installEventFilter(this);
    ui->lineBreak3->installEventFilter(this);
    ui->lineCalls->installEventFilter(this);
    ui->lineCalls2->installEventFilter(this);
    ui->lineCalls3->installEventFilter(this);
    ui->lineCycles->installEventFilter(this);
    ui->lineCycles2->installEventFilter(this);
    ui->lineCycles3->installEventFilter(this);
    ui->lineInstr->installEventFilter(this);
    ui->lineInstr2->installEventFilter(this);
    ui->lineInstr3->installEventFilter(this);
    ui->lineInterrupt->installEventFilter(this);
    ui->lineInterrupt2->installEventFilter(this);
    ui->lineInterrupt3->installEventFilter(this);
    ui->lineIntRet->installEventFilter(this);
    ui->lineIntRet2->installEventFilter(this);
    ui->lineIntRet3->installEventFilter(this);
    ui->lineProgram->installEventFilter(this);
    ui->lineProgram2->installEventFilter(this);
    ui->lineProgram3->installEventFilter(this);
    ui->lineReturns->installEventFilter(this);
    ui->lineReturns2->installEventFilter(this);
    ui->lineReturns3->installEventFilter(this);
      //  ui->lineNano->installEventFilter(this);

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

