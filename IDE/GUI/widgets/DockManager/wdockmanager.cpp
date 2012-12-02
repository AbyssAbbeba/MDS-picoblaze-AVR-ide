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
#include "wdockmanager.h"


WDockManager::WDockManager(MainForm *mainWindow)
{
    wMainWindow = mainWindow;
    QWidget *centralWidget = new QWidget(wMainWindow);
    QVBoxLayout *layout = new QVBoxLayout(centralWidget);
    wTab = new QTabWidget(centralWidget);
    splitter = new QSplitter(centralWidget);
    splitter->show();
    wTab->setTabsClosable(true);
    wTab->setMovable(true);
    wRight = NULL;
    wLeft = NULL;
    wBottom = NULL;
    activeCodeEdit = NULL;
    centralBase = NULL;
    connect(wTab, SIGNAL(tabCloseRequested(int)),this, SLOT(closeTab(int)));
    connect(wTab, SIGNAL(currentChanged(int)), this, SLOT(changeCodeEditor(int)));

    layout->addWidget(wTab);
    layout->addWidget(splitter);
    centralWidget->setLayout(layout);
    wMainWindow->setCentralWidget(centralWidget);
}



void WDockManager::changeActiveCodeEdit(CodeEdit *editor)
{
    qDebug() << "wdockmanager - change active Code Editor";
    this->activeCodeEdit = editor;
}


void WDockManager::changeCodeEditor(int index)
{
    if (activeCodeEdit != NULL)
    {
        qDebug() << "wdockmanager - change Code Editor";
        qDebug() << "index: " << index;
        qDebug() << "size: " << openCentralWidgets.count();
        activeCodeEdit->loadCodeEdit(openCentralWidgets.at(index)->getCodeEdit());
    }
}


void WDockManager::changeTabName(CodeEdit *editor, QString name)
{
    //wTab->setTabText(wTab->indexOf(editor), name);
}


void WDockManager::closeTab(int index)
{
    wMainWindow->saveFile(openCentralWidgets.at(index)->getCodeEdit());
    openCentralWidgets.removeAt(index);
    wTab->removeTab(index);
}



void WDockManager::setTabChanged()
{
    openCentralWidgets.at(wTab->currentIndex())->getCodeEdit()->setChanged();;
}


void WDockManager::setTabSaved()
{
    openCentralWidgets.at(wTab->currentIndex())->getCodeEdit()->setSaved();
}


QTextEdit* WDockManager::getCentralTextEdit()
{
    return activeCodeEdit->getTextEdit();
}



QTextEdit* WDockManager::getTabTextEdit(int index)
{
    return openCentralWidgets.at(index)->getCodeEdit()->getTextEdit();
}



CodeEdit* WDockManager::getCentralWidget()
{
    return activeCodeEdit;
}



CodeEdit* WDockManager::getTabWidget(int index)
{
    return openCentralWidgets.at(index)->getCodeEdit();
}



int WDockManager::getTabCount()
{
    return wTab->count();
}



QString WDockManager::getCentralName()
{
    return openCentralWidgets.at(wTab->currentIndex())->getCodeEdit()->getName();
}

QString WDockManager::getCentralPath()
{
    return openCentralWidgets.at(wTab->currentIndex())->getCodeEdit()->getPath();
}


void WDockManager::setCentralName(QString wName)
{
    openCentralWidgets.at(wTab->currentIndex())->getCodeEdit()->setName(wName);
    wTab->setTabText(wTab->currentIndex(), wName);
}


void WDockManager::setCentralPath(QString wPath)
{
    openCentralWidgets.at(wTab->currentIndex())->getCodeEdit()->setPath(wPath);
}



void WDockManager::addCentralWidget(QString wName, QString wPath)
{
    CodeEdit *newEditor = new CodeEdit(0, true, wName, wPath);
    BaseEditor *newBaseEditor;
    if (centralBase == NULL)
    {
        newBaseEditor = new BaseEditor(NULL, this, newEditor, false); 
        centralBase = new BaseEditor(splitter, this, newEditor, true);
        activeCodeEdit = centralBase->getCodeEdit();
        splitter->addWidget(centralBase);
    }
    else
    {
        newBaseEditor = new BaseEditor(NULL, this, newEditor, true);
        activeCodeEdit->loadCodeEdit(newBaseEditor->getCodeEdit());
    }
    QWidget *empty = new QWidget(wTab);
    empty->setMaximumWidth(0);
    empty->setMaximumHeight(0);
    openCentralWidgets.append(newBaseEditor);
    wTab->addTab(empty, wName);
    wTab->setCurrentIndex(wTab->count()-1);
    //if (wTab->count() > 1)
    //{
        //qDebug() << "hiding";
    //    openCentralWidgets.at(wTab->count()-2)->hide();
    //}
    //add tab tooltip with path
    connect(newBaseEditor->getCodeEdit(), SIGNAL(changedTabName(CodeEdit*, QString)), this, SLOT(changeTabName(CodeEdit*, QString)));
    //connect(centralBase->getCodeEdit(), SIGNAL(changedTabName(CodeEdit*, QString)), this, SLOT(changeTabName(CodeEdit*, QString)));
}

void WDockManager::addDockWidget(int code)
{
    WDock *newWDock = new WDock(code, wMainWindow);
    if (getDockWidgetArea(newWDock->getArea())!=NULL)
        wMainWindow->tabifyDockWidget(getDockWidgetArea(newWDock->getArea()), newWDock->getQDockWidget());
    openDockWidgets.append(newWDock);
}


QDockWidget* WDockManager::getDockWidget(int code)
{
    QList<WDock*>::iterator i;
    for (i = openDockWidgets.begin(); i != openDockWidgets.end(); i++)
        if ((*i)->cmpCode(code) == true)
            return (*i)->getQDockWidget();
     return NULL;
}


QDockWidget* WDockManager::getDockWidgetArea(int area)
{
    QList<WDock*>::iterator i;
    for (i = openDockWidgets.begin(); i != openDockWidgets.end(); i++)
        if ((*i)->cmpArea(area) == true)
            return (*i)->getQDockWidget();
     return NULL;
}

void WDockManager::hideDockWidgetArea(int area)
{
    QList<WDock*>::iterator i;
    for (i = openDockWidgets.begin(); i != openDockWidgets.end(); i++)
        if ((*i)->cmpArea(area) == true)
            (*i)->getQDockWidget()->hide();
}

void WDockManager::showDockWidgetArea(int area)
{
    QList<WDock*>::iterator i;
    for (i = openDockWidgets.begin(); i != openDockWidgets.end(); i++)
        if ((*i)->cmpArea(area) == true)
            (*i)->getQDockWidget()->show();
}


bool WDockManager::isEmpty()
{
    return openCentralWidgets.empty();
}






WDock::WDock(int code, MainForm *mainWindow)
{
    switch (code)
    {
        case wListCode:
        {
            wDockWidget = new QDockWidget("List", mainWindow);
            wDockWidget->setAllowedAreas(Qt::LeftDockWidgetArea);
            mainWindow->addDockWidget(Qt::LeftDockWidgetArea, wDockWidget);
            QListWidget *newWidget = new QListWidget(wDockWidget);
            area = 0;
            wDockWidget->setWidget(newWidget);
	    break;
        }
        case wListCode2:
        {
            wDockWidget = new QDockWidget("List2", mainWindow);
            wDockWidget->setAllowedAreas(Qt::RightDockWidgetArea);
            mainWindow->addDockWidget(Qt::RightDockWidgetArea, wDockWidget);
            QListWidget *newWidget = new QListWidget(wDockWidget);
            area = 1;
            wDockWidget->setWidget(newWidget);
	    break;
        }
	case wCompileInfo:
        {
            wDockWidget = new QDockWidget("Compiler Info", mainWindow);
            wDockWidget->setAllowedAreas(Qt::BottomDockWidgetArea);
            mainWindow->addDockWidget(Qt::BottomDockWidgetArea, wDockWidget);
            QPlainTextEdit *newWidget = new QPlainTextEdit(wDockWidget);
            newWidget->setFont(QFont("Andale Mono", 10));
            area = 2;
            newWidget->setReadOnly(true);
            wDockWidget->setWidget(newWidget);
	    break;
        }
        case wSimulationInfo:
        {
            wDockWidget = new QDockWidget("Simulation Info", mainWindow);
            wDockWidget->setAllowedAreas(Qt::BottomDockWidgetArea);
            mainWindow->addDockWidget(Qt::BottomDockWidgetArea, wDockWidget);
            WSimulationInfo *newWidget = new WSimulationInfo(mainWindow->getProjectMan()->getActive()->getSimControl(), wDockWidget);
            area = 2;
            wDockWidget->setWidget(newWidget);
	    break;
        }
    }
    this->code=code;
}

WDock::~WDock()
{
    delete wDockWidget;
}



QDockWidget* WDock::getQDockWidget()
{
    return wDockWidget;
}



bool WDock::cmpCode(int code)
{
    return (this->code==code);
}


bool WDock::cmpArea(int area)
{
    return (this->area==area);
}


int WDock::getArea()
{
    return this->area;
}
