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


/*PicoBlazeGrid::PicoBlazeGrid(QWidget *parent, MCUSimControl *controlUnit)
    : QWidget(parent)
{
    this->layout = new QHBoxLayout(this);
    this->layout->setSpacing(0);
    //this->layout->setHorizontalSpacing(0);
    //this->layout->setVerticalSpacing(0);

    QWidget *wRegs = new QWidget(this);
    //wRegs->setMaximumHeight(200);
    QVBoxLayout *vRegs = new QVBoxLayout(wRegs);
    vRegs->setSpacing(0);

    QWidget *wScratch = new QWidget(this);
    //wRegs->setMaximumHeight(200);
    QVBoxLayout *vScratch = new QVBoxLayout(wScratch);
    vScratch->setSpacing(0);

    QWidget *wPorts = new QWidget(this);
    //wRegs->setMaximumHeight(200);
    QVBoxLayout *vPorts = new QVBoxLayout(wPorts);

    QWidget *wStack = new QWidget(this);
    //wRegs->setMaximumHeight(200);
    QVBoxLayout *vStack = new QVBoxLayout(wStack);
    
    this->memRegs = new McuMemoryView(wRegs, controlUnit, MCUSim::Subsys::SubsysId::ID_MEM_REGISTERS);
    //this->memRegs->fixHeight();
    this->memScratch = new McuMemoryView(wScratch, controlUnit, MCUSim::Subsys::SubsysId::ID_MEM_DATA);
    this->memPorts = new McuMemoryView(wPorts, controlUnit, MCUSim::Subsys::SubsysId::ID_MEM_DATA);
    this->memStack = new McuMemoryView(wStack, controlUnit, MCUSim::Subsys::SubsysId::ID_MEM_DATA);
    this->memRegs->show();
    this->memScratch->show();
    this->memPorts->show();
    this->memStack->show();
    
    this->lblRegs = new QLabel("Registers", this);
    this->lblScratch = new QLabel("Scratch", this);
    this->lblPorts = new QLabel("Ports", this);
    this->lblStack = new QLabel("Stack", this);
    this->lblORD = new QLabel("ORD", this);
    this->lblORW = new QLabel("ORW", this);
    this->lblPC = new QLabel("PC", this);
    this->lblTime = new QLabel("Time", this);
    this->lblClock = new QLabel("Clock", this);


    vRegs->addWidget(this->lblRegs);
    vRegs->addWidget(this->memRegs);
    wRegs->setLayout(vRegs);

    vScratch->addWidget(this->lblScratch);
    vScratch->addWidget(this->memScratch);
    wScratch->setLayout(vScratch);

    vPorts->addWidget(this->lblPorts);
    vPorts->addWidget(this->memPorts);
    wPorts->setLayout(vPorts);

    vStack->addWidget(this->lblStack);
    vStack->addWidget(this->memStack);
    wStack->setLayout(vStack);

    this->layout->addWidget(wRegs);
    this->layout->addWidget(wScratch);
    this->layout->addWidget(wPorts);
    this->layout->addWidget(wStack);
    
    //this->layout->addWidget(this->lblRegs);//, 0,0, Qt::AlignHCenter);
    //this->layout->addWidget(this->lblScratch);
    //this->layout->addWidget(this->lblPorts);
    //this->layout->addWidget(this->lblStack);

    //this->layout->addWidget(this->memRegs);//, 1,0);
    //this->layout->addWidget(this->memScratch);
    //this->layout->addWidget(this->memPorts);
    //this->layout->addWidget(this->memStack);

    QWidget *wOR = new QWidget(this);
    QVBoxLayout *vOR = new QVBoxLayout(wOR);
    vOR->addWidget(this->lblORD);
    vOR->addWidget(this->lblORW);
    wOR->setLayout(vOR);
    this->layout->addWidget()

    QWidget *wInfo = new QWidget(this);
    QGridLayout *gInfo = new QGridLayout(wInfo);
    wInfo->setLayout(gInfo);
    this->layout->addWidget(wInfo);
    
    this->setLayout(this->layout);
}*/


PicoBlazeGrid::PicoBlazeGrid(QWidget *parent, MCUSimControl *controlUnit)
    : QWidget(parent)
{

    this->memRegs = new McuMemoryView(this, controlUnit, MCUSim::Subsys::SubsysId::ID_MEM_REGISTERS);
    this->memRegs->move(0, 5);
    this->memRegs->fixHeight();
    this->memScratch = new McuMemoryView(this, controlUnit, MCUSim::Subsys::SubsysId::ID_MEM_DATA);
    this->memScratch->move(220,5);
    this->memPorts = new McuMemoryView(this, controlUnit, MCUSim::Subsys::SubsysId::ID_MEM_DATA);
    this->memPorts->move(440,5);
    this->memStack = new QListWidget(this);
    this->memStack->move(680, 37);
    this->memStack->setMaximumWidth(100);
    this->memRegs->show();
    this->memScratch->show();
    this->memPorts->show();
    this->memStack->show();

    this->lblRegs = new QLabel("Registers", this);
    this->lblRegs->move(40,0);
    this->lblScratch = new QLabel("Scratch", this);
    this->lblScratch->move(260,0);
    this->lblPorts = new QLabel("Ports", this);
    this->lblPorts->move(480,0);
    this->lblRD = new QLabel("RD", this);
    this->lblRD->move(590, 0);
    this->lblRW = new QLabel("RW", this);
    this->lblRW->move(625, 0);
    this->lblStack = new QLabel("Stack", this);
    this->lblStack->move(680,0);
    this->lblPC = new QLabel("PC", this);
    this->lblPC->move(800,0);
    this->lblTime = new QLabel("Time", this);
    this->lblTime->move(800,20);
    this->lblClock = new QLabel("Clock", this);
    this->lblClock->move(800,40);

    this->leSP = new QLineEdit(this);
    this->leSP->setMaximumWidth(25);
    this->leSP->setMaximumHeight(17);
    this->leSP->setFont(QFont("Andale Mono", 9));
    this->leSP->move(730, 0);
    this->lePC = new QLineEdit(this);
    this->lePC->setMaximumWidth(50);
    this->lePC->setMaximumHeight(17);
    this->lePC->move(830, 0);
}



void PicoBlazeGrid::setProjectPath(QString prjPath)
{
    this->projectPath = prjPath;
}