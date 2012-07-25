#include "filemgr.h"
#include "../mainform/project.h"

FileMgr::FileMgr(QWidget *parentWidget, Project *currProject)
{
    this->parent = parentWidget;
    this->project = currProject;
    fileList = new QListWidget(this);
    deleteBut = new QPushButton("Delete", this);
    newBut = new QPushButton("New", this);
    addBut = new QPushButton("Add", this);
    setMainBut = new QPushButton("Set Main", this);
    //btnLayout = new QVboxLayout(this);
    //btnLayout->addWidget(newBut);
    //btnLayout->addWidget(addBut);
    //btnLayout->addWidget(setMainBut);
    //btnLayout->addWidget(deleteBut);
    layout = new QGridLayout(this);
    layout->addWidget(fileList, 0,0,Qt::AlignJustify);
    layout->addWidget(newBut, 0,1,Qt::AlignJustify);
    layout->addWidget(addBut, 1,1,Qt::AlignJustify);
    layout->addWidget(setMainBut, 2,1,Qt::AlignJustify);
    layout->addWidget(deleteBut, 3,1,Qt::AlignJustify);
    this->setLayout(layout);

    this->adjustSize();
}
