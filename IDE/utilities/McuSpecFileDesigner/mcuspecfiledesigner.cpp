#include <QtGui>
#include "mcuspecfiledesigner.h"
#include "ioregswidget.h"

McuSpecFileDesigner::McuSpecFileDesigner(QWidget *parent)
    : QWidget(parent)
{
    mainTabs = new QTabWidget(this);
    dataMemIORegsTabs = new QTabWidget(this);

    QWidget *generalWidget = new QWidget(this);
    QWidget *ioWidget = new QWidget(this);
    QWidget *instructionWidget = new QWidget(this);
    QWidget *systemControlWidget = new QWidget(this);
    QWidget *dataMemIORegsGenWidget = new QWidget(this);
    QWidget *emptyWidget = new QWidget(this);
    
    QWidget *dataMemIOMem2Widget = new QWidget(this);
    QWidget *finalizeWidget = new QWidget(this);


    //GENERAL
    generalLblTag = new QLabel("Tag", generalWidget);
    generalLblFamily = new QLabel("Family", generalWidget);
    generalLblName = new QLabel("Name", generalWidget);
    generalLblAdc = new QLabel("ADC", generalWidget);
    generalLblAnalogComp = new QLabel("Analog Comparator", generalWidget);
    generalLblClockControl = new QLabel("Clock Control", generalWidget);
    generalLblExtInterr = new QLabel("External Interrupts", generalWidget);
    generalLblFusesNLocks = new QLabel("Fuses/Locks", generalWidget);
    generalLblIsp = new QLabel("ISP", generalWidget);
    generalLblParalProg = new QLabel("Parallel Prog", generalWidget);
    generalLblProgMem = new QLabel("Program Memory", generalWidget);
    generalLblSpi = new QLabel("SPI", generalWidget);
    generalLblTimer0 = new QLabel("Timer Counter 0", generalWidget);
    generalLblTimer1 = new QLabel("Timer Counter 1", generalWidget);
    generalLblTimer2 = new QLabel("Timer Counter 2", generalWidget);
    generalLblWatchdog = new QLabel("Watchdog Timer", generalWidget);
    generalLblTwi = new QLabel("TWI", generalWidget);
    generalLblUsart = new QLabel("USART", generalWidget);
    generalLblDataEEProm = new QLabel("\tDataEEProm", generalWidget);
    generalLblDataMemory = new QLabel("\tData Memory", generalWidget);;
    generalLblBootloader = new QLabel("\tBootloader", generalWidget);;

    generalChckAdc = new QCheckBox(generalWidget);
    generalChckAnalogComp = new QCheckBox(generalWidget);
    generalChckClockControl = new QCheckBox(generalWidget);
    generalChckExtInterr = new QCheckBox(generalWidget);
    generalChckIsp = new QCheckBox(generalWidget);
    generalChckParalProg = new QCheckBox(generalWidget);
    generalChckSpi = new QCheckBox(generalWidget);
    generalChckTimer0 = new QCheckBox(generalWidget);
    generalChckTimer1 = new QCheckBox(generalWidget);
    generalChckTimer2 = new QCheckBox(generalWidget);
    generalChckWatchdog = new QCheckBox(generalWidget);
    generalChckTwi = new QCheckBox(generalWidget);
    generalChckUsart = new QCheckBox(generalWidget);
    generalChckDataEEProm = new QCheckBox(generalWidget);
    generalChckBootloader = new QCheckBox(generalWidget);

    generalLETag = new QLineEdit(generalWidget);
    generalLEFamily = new QLineEdit(generalWidget);
    generalLEName = new QLineEdit(generalWidget);
    generalLEProgMem = new QLineEdit(generalWidget);
    generalLEFuses = new QLineEdit("Fuses", generalWidget);
    generalLEFuses->setToolTip("Fuses");
    generalLELocks = new QLineEdit("Lock bits", generalWidget);
    generalLELocks->setToolTip("Lock Bits");
    generalLEFuses->setMinimumWidth(400);
    generalLEEEPromSize = new QLineEdit("Size", generalWidget);
    generalLEEEPromSize->setToolTip("EEProm Size");
    generalLEEEPromWTime = new QLineEdit("Write Time", generalWidget);
    generalLEEEPromWTime->setToolTip("EEProm Write Time");
    generalLEDataMemRegFileSize = new QLineEdit("Reg File Size", generalWidget);
    generalLEDataMemRegFileSize->setToolTip("Reg File Size");
    generalLEDataMemSRamSize = new QLineEdit("SRam Size", generalWidget);
    generalLEDataMemSRamSize->setToolTip("SRAM Size");
    generalLEDataMemIORegSize = new QLineEdit("IO Reg Size", generalWidget);
    generalLEDataMemIORegSize->setToolTip("IO Reg Size");
    generalLEDataMem2Size = new QLineEdit("Mem2 Size", generalWidget);
    generalLEDataMem2Size->setToolTip("Mem2 Size");
    generalLEDataMemSPWidth = new QLineEdit("SP Width", generalWidget);
    generalLEDataMemSPWidth->setToolTip("SP Width");
    generalLEBootMinProgTime = new QLineEdit("Min Prog Time", generalWidget);
    generalLEBootMinProgTime->setToolTip("Min Prog Time");
    generalLEBootMaxProgTime = new QLineEdit("Max Prog Time", generalWidget);
    generalLEBootMaxProgTime->setToolTip("Max Prog Time");
    generalLEBootPageSize = new QLineEdit("Page Size", generalWidget);
    generalLEBootPageSize->setToolTip("Page Size");
    generalLEBootResetAddress0 = new QLineEdit("Reset Address 0", generalWidget);
    generalLEBootResetAddress0->setToolTip("Reset Address 0");
    generalLEBootResetAddress1 = new QLineEdit("Reset Address 1", generalWidget);
    generalLEBootResetAddress1->setToolTip("Reset Address 1");
    generalLEBootResetAddress2 = new QLineEdit("Reset Address 2", generalWidget);
    generalLEBootResetAddress2->setToolTip("Reset Address 2");
    generalLEBootResetAddress3 = new QLineEdit("Reset Address 3", generalWidget);
    generalLEBootResetAddress3->setToolTip("Reset Address 3");
    generalLEBootRWWSectionSize = new QLineEdit("RWW Section Size", generalWidget);
    generalLEBootRWWSectionSize->setToolTip("RWW Section Size");

    generalLayout = new QGridLayout(generalWidget);
    generalLayout->addWidget(generalLblTag, 0,0);
    generalLayout->addWidget(generalLblFamily, 1,0);
    generalLayout->addWidget(generalLblName, 2,0);
    generalLayout->addWidget(generalLblAdc, 3,0);
    generalLayout->addWidget(generalLblAnalogComp, 4,0);
    generalLayout->addWidget(generalLblClockControl, 5,0);
    generalLayout->addWidget(generalLblExtInterr, 6,0);
    generalLayout->addWidget(generalLblFusesNLocks, 7,0);
    generalLayout->addWidget(generalLblIsp, 9,0);
    generalLayout->addWidget(generalLblParalProg, 10,0);
    generalLayout->addWidget(generalLblProgMem, 11,0);
    generalLayout->addWidget(generalLblSpi, 12,0);
    generalLayout->addWidget(generalLblTimer0, 13,0);
    generalLayout->addWidget(generalLblTimer1, 14,0);
    generalLayout->addWidget(generalLblTimer2, 15,0);
    generalLayout->addWidget(generalLblWatchdog, 16,0);
    generalLayout->addWidget(generalLblTwi, 17,0);
    generalLayout->addWidget(generalLblUsart, 18,0);
    generalLayout->addWidget(generalLblDataEEProm, 0,2);
    generalLayout->addWidget(generalLblDataMemory, 3,2);
    generalLayout->addWidget(generalLblBootloader, 8,2);

    generalLayout->addWidget(generalChckAdc, 3,1);
    generalLayout->addWidget(generalChckAnalogComp, 4,1);
    generalLayout->addWidget(generalChckClockControl, 5,1);
    generalLayout->addWidget(generalChckExtInterr, 6,1);
    generalLayout->addWidget(generalChckIsp, 9,1);
    generalLayout->addWidget(generalChckParalProg, 10,1);
    generalLayout->addWidget(generalChckSpi, 12,1);
    generalLayout->addWidget(generalChckTimer0, 13,1);
    generalLayout->addWidget(generalChckTimer1, 14,1);
    generalLayout->addWidget(generalChckTimer2, 15,1);
    generalLayout->addWidget(generalChckWatchdog, 16,1);
    generalLayout->addWidget(generalChckTwi, 17,1);
    generalLayout->addWidget(generalChckUsart, 18,1);
    generalLayout->addWidget(generalChckDataEEProm, 0,3);
    generalLayout->addWidget(generalChckBootloader, 8,3);

    generalLayout->addWidget(generalLETag, 0,1);
    generalLayout->addWidget(generalLEFamily, 1,1);
    generalLayout->addWidget(generalLEName, 2,1);
    generalLayout->addWidget(generalLEFuses, 7,1);
    generalLayout->addWidget(generalLELocks, 8,1);
    generalLayout->addWidget(generalLEProgMem, 11,1);
    generalLayout->addWidget(generalLEEEPromWTime, 1,3);
    generalLayout->addWidget(generalLEEEPromSize, 2,3);
    generalLayout->addWidget(generalLEDataMemRegFileSize, 3,3);
    generalLayout->addWidget(generalLEDataMemSRamSize, 4,3);
    generalLayout->addWidget(generalLEDataMemIORegSize, 5,3);
    generalLayout->addWidget(generalLEDataMem2Size, 6,3);
    generalLayout->addWidget(generalLEDataMemSPWidth, 7,3);
    generalLayout->addWidget(generalLEBootMinProgTime, 9,3);
    generalLayout->addWidget(generalLEBootMaxProgTime, 10,3);
    generalLayout->addWidget(generalLEBootPageSize, 11,3);
    generalLayout->addWidget(generalLEBootResetAddress0, 12,3);
    generalLayout->addWidget(generalLEBootResetAddress1, 13,3);
    generalLayout->addWidget(generalLEBootResetAddress2, 14,3);
    generalLayout->addWidget(generalLEBootResetAddress3, 15,3);
    generalLayout->addWidget(generalLEBootRWWSectionSize, 16,3);

    //INSTRUCTION SET AND INTERRUPT CONTROLER
    instructionLblPcWidth = new QLabel("PC Width", instructionWidget);
    instructionLblAvailableInst = new QLabel("Available Instructions", instructionWidget);
    instructionLblPossibleIntr = new QLabel("Possible Interrupts", instructionWidget);

    instructionLEPcWidth = new QLineEdit(instructionWidget);
    
    instructionPTEInst = new QPlainTextEdit(instructionWidget);
    instructionPTEIntr = new QPlainTextEdit(instructionWidget);

    instructionLayout = new QGridLayout(instructionWidget);
    instructionLayout->addWidget(instructionLblPcWidth, 0,0);
    instructionLayout->addWidget(instructionLblAvailableInst, 1,0);
    instructionLayout->addWidget(instructionLblPossibleIntr, 1,2);
    
    instructionLayout->addWidget(instructionLEPcWidth, 0,1);

    instructionLayout->addWidget(instructionPTEInst, 1,1);
    instructionLayout->addWidget(instructionPTEIntr, 1,3);

    //IO
    ioLblPullUpRes = new QLabel("Pull-up Resistance", ioWidget);
    ioLblHasPorts = new QLabel("Has Ports", ioWidget);
    ioLblSpecFuncMap = new QLabel("SpecFuncMap", ioWidget);
    ioLblPins = new QLabel("Available Pins", ioWidget);

    ioLEPullUpRes = new QLineEdit(ioWidget);
    ioLEHasPorts = new QLineEdit(ioWidget);

    ioPTESpecFuncMap = new QPlainTextEdit(ioWidget);
    ioPTESpecFuncMap->setPlainText("func name\nfunc name\netc.");
    ioPTESpecFuncMap->setToolTip("func name\nfunc name\netc.");
    ioPTEPins = new QPlainTextEdit(ioWidget);
    ioPTEPins->setPlainText("PIN1 PIN2 PIN3 etc.");
    ioPTEPins->setToolTip("PIN1 PIN2 PIN3 etc.");

    ioLayout = new QGridLayout(ioWidget);
    ioLayout->addWidget(ioLblPullUpRes, 0,0);
    ioLayout->addWidget(ioLblHasPorts, 1,0);
    ioLayout->addWidget(ioLblSpecFuncMap, 2,0);
    ioLayout->addWidget(ioLblPins, 2,2);

    ioLayout->addWidget(ioLEPullUpRes, 0,1);
    ioLayout->addWidget(ioLEHasPorts, 1,1);

    ioLayout->addWidget(ioPTESpecFuncMap, 2,1);
    ioLayout->addWidget(ioPTEPins, 2,3);

    //SYSTEM CONTROL
    systemLblRstTresLow = new QLabel("Reset Treshold Low", systemControlWidget);
    systemLblRstTresHigh = new QLabel("Reset Treshold High", systemControlWidget);
    systemLblMinRstPulse = new QLabel("Min Reset Pulse Width", systemControlWidget);
    systemLblPORTresFall = new QLabel("Power on Reset Treshold Falling", systemControlWidget);
    systemLblPORTresRaise = new QLabel("Power on Reset Treshold Raising", systemControlWidget);
    systemLblTBOD = new QLabel("tBOD", systemControlWidget);
    systemLblBrownOutTres0 = new QLabel("Brown Out Treshold0", systemControlWidget);
    systemLblBrownOutTres1 = new QLabel("Brown Out Treshold1", systemControlWidget);
    systemLblHasPOR = new QLabel("Has Power on Reset", systemControlWidget);
    systemLblHasBrownOut = new QLabel("Has Brown Out Reset", systemControlWidget);
    systemLblHasExtRst = new QLabel("Has External Reset", systemControlWidget);
    systemLblHasWatchdog = new QLabel("Has Watchdog", systemControlWidget);

    systemChckHasPOR = new QCheckBox(systemControlWidget);
    systemChckHasBrownOut = new QCheckBox(systemControlWidget);
    systemChckHasExtRst = new QCheckBox(systemControlWidget);
    systemChckHasWatchdog = new QCheckBox(systemControlWidget);

    systemLERstTresLow = new QLineEdit(systemControlWidget);
    systemLERstTresHigh = new QLineEdit(systemControlWidget);
    systemLEMinRstPulse = new QLineEdit(systemControlWidget);
    systemLEPORTresFall = new QLineEdit(systemControlWidget);
    systemLEPORTresRaise = new QLineEdit(systemControlWidget);
    systemLETBOD = new QLineEdit(systemControlWidget);
    systemLEBrownOutTres0 = new QLineEdit(systemControlWidget);
    systemLEBrownOutTres1 = new QLineEdit(systemControlWidget);

    systemLayout = new QGridLayout(systemControlWidget);
    systemLayout->addWidget(systemLblRstTresLow, 0,0);
    systemLayout->addWidget(systemLblRstTresHigh, 1,0);
    systemLayout->addWidget(systemLblMinRstPulse, 2,0);
    systemLayout->addWidget(systemLblPORTresFall, 3,0);
    systemLayout->addWidget(systemLblPORTresRaise, 4,0);
    systemLayout->addWidget(systemLblTBOD, 5,0);
    systemLayout->addWidget(systemLblBrownOutTres0, 6,0);
    systemLayout->addWidget(systemLblBrownOutTres1, 7,0);
    systemLayout->addWidget(systemLblHasPOR, 8,0);
    systemLayout->addWidget(systemLblHasBrownOut, 9,0);
    systemLayout->addWidget(systemLblHasExtRst, 10,0);
    systemLayout->addWidget(systemLblHasWatchdog, 11,0);

    systemLayout->addWidget(systemLERstTresLow, 0,1);
    systemLayout->addWidget(systemLERstTresHigh, 1,1);
    systemLayout->addWidget(systemLEMinRstPulse, 2,1);
    systemLayout->addWidget(systemLEPORTresFall, 3,1);
    systemLayout->addWidget(systemLEPORTresRaise, 4,1);
    systemLayout->addWidget(systemLETBOD, 5,1);
    systemLayout->addWidget(systemLEBrownOutTres0, 6,1);
    systemLayout->addWidget(systemLEBrownOutTres1, 7,1);
    
    systemLayout->addWidget(systemChckHasPOR, 8,1);
    systemLayout->addWidget(systemChckHasBrownOut, 9,1);
    systemLayout->addWidget(systemChckHasExtRst, 10,1);
    systemLayout->addWidget(systemChckHasWatchdog, 11,1);

    //data memory general tab
    dataMemLblRegFileSize = new QLabel("Reg File Size", dataMemIORegsGenWidget);
    dataMemLblSRamSize = new QLabel("SRam Size", dataMemIORegsGenWidget);
    dataMemLblIORegSize = new QLabel("IO Reg Size", dataMemIORegsGenWidget);
    dataMemLblMem2Size = new QLabel("Mem2Size", dataMemIORegsGenWidget);
    dataMemLblSPWidth = new QLabel("SPWidth", dataMemIORegsGenWidget);

    dataMemLERegFileSize = new QLineEdit(dataMemIORegsGenWidget);
    dataMemLESRamSize = new QLineEdit(dataMemIORegsGenWidget);
    dataMemLEIORegSize = new QLineEdit(dataMemIORegsGenWidget);
    dataMemLEMem2Size = new QLineEdit(dataMemIORegsGenWidget);
    dataMemLESPWidth = new QLineEdit(dataMemIORegsGenWidget);

    dataMemBtnChange = new QPushButton("Change size", dataMemIORegsGenWidget);
    connect(dataMemBtnChange, SIGNAL(clicked()), this, SLOT(reallocateIORegs()));

    dataMemLayout = new QGridLayout(dataMemIORegsGenWidget);
    dataMemLayout->addWidget(dataMemLblRegFileSize, 0,0);
    dataMemLayout->addWidget(dataMemLblSRamSize, 1,0);
    dataMemLayout->addWidget(dataMemLblIORegSize, 2,0);
    dataMemLayout->addWidget(dataMemLblMem2Size, 3,0);
    dataMemLayout->addWidget(dataMemLblSPWidth, 4,0);

    dataMemLayout->addWidget(dataMemLERegFileSize, 0,1);
    dataMemLayout->addWidget(dataMemLESRamSize, 1,1);
    dataMemLayout->addWidget(dataMemLEIORegSize, 2,1);
    dataMemLayout->addWidget(dataMemLEMem2Size, 3,1);
    dataMemLayout->addWidget(dataMemLESPWidth, 4,1);

    dataMemLayout->addWidget(dataMemBtnChange, 2,2);
    
    
    //setting layouts and adding tabs
    generalWidget->setLayout(generalLayout);
    instructionWidget->setLayout(instructionLayout);
    ioWidget->setLayout(ioLayout);
    systemControlWidget->setLayout(systemLayout);
    dataMemIORegsGenWidget->setLayout(dataMemLayout);
    mainTabs->addTab(generalWidget, "General");
    mainTabs->addTab(dataMemIORegsTabs, "Data Memory IORegs");
    mainTabs->addTab(dataMemIOMem2Widget, "Data Memory IOMem2");
    mainTabs->addTab(instructionWidget, "Instructions and Interrupts");
    mainTabs->addTab(ioWidget, "IO");
    mainTabs->addTab(systemControlWidget, "System Control");
    mainTabs->addTab(finalizeWidget, "Finalize");

    dataMemIORegsTabs->addTab(dataMemIORegsGenWidget, "General");
    dataMemIORegsTabs->addTab(emptyWidget, "IO Regs Init and Bits");
    
    this->showMaximized();
}

void McuSpecFileDesigner::getConfig()
{
}


void McuSpecFileDesigner::reallocateIORegs()
{
    int ioRegsCount = this->dataMemLEIORegSize->text().toInt();
    for (int i=1; i<=ioRegsCount; i++)
    {
        IORegsWidget *newIORegWidget = new IORegsWidget(this, i);
        ioRegs.append(newIORegWidget);
        connect(newIORegWidget->nextBtn, SIGNAL(clicked()), this, SLOT(setNextIOReg()));
        connect(newIORegWidget->previousBtn, SIGNAL(clicked()), this, SLOT(setPreviousIOReg()));
    }
    dataMemIORegsTabs->removeTab(1);
    dataMemIORegsTabs->addTab(ioRegs.at(0), "IO Regs Init and Bits");
}

void McuSpecFileDesigner::setNextIOReg()
{
    int currIOReg = ioRegs.indexOf((IORegsWidget*)(dataMemIORegsTabs->currentWidget()));
    if (currIOReg < ioRegs.count()-1)
    {
        dataMemIORegsTabs->addTab(ioRegs.at(currIOReg+1), "IO Regs Init and Bits");
        dataMemIORegsTabs->removeTab(1);
    }
}

void McuSpecFileDesigner::setPreviousIOReg()
{
    int currIOReg = ioRegs.indexOf((IORegsWidget*)(dataMemIORegsTabs->currentWidget()));
    if (currIOReg != 0)
    {
        dataMemIORegsTabs->addTab(ioRegs.at(currIOReg-1), "IO Regs Init and Bits");
        dataMemIORegsTabs->removeTab(1);
    }
}