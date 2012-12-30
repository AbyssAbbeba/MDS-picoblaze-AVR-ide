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
    wTab = new TabBar(centralWidget);
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
    if (this->activeCodeEdit != editor)
    {
        qDebug() << "wdockmanager - change active Code Editor";
        this->activeCodeEdit = editor;
        breakpointList->disconnect();
        bookmarkList->disconnect();
        breakpointList->reload(activeCodeEdit->getBreakpointList());
        bookmarkList->reload(activeCodeEdit->getBookmarkList());
        updateAnalysersSlot(this->activeCodeEdit);
        connect(this->activeCodeEdit, SIGNAL(bookmarkListAdd(int)), bookmarkList, SLOT(bookmarkListAddSlot(int)));
        connect(this->activeCodeEdit, SIGNAL(bookmarkListRemove(int)), bookmarkList, SLOT(bookmarkListRemoveSlot(int)));
        connect(this->activeCodeEdit, SIGNAL(breakpointListAdd(int)), breakpointList, SLOT(breakpointListAddSlot(int)));
        connect(this->activeCodeEdit, SIGNAL(breakpointListRemove(int)), breakpointList, SLOT(breakpointListRemoveSlot(int)));
        wTab->setCurrentIndex(codeEditList.indexOf(this->activeCodeEdit->getParentCodeEdit()));
    }
}


void WDockManager::changeCodeEditor(int index)
{
    if (activeCodeEdit != NULL && index >= 0)
    {
        qDebug() << "wdockmanager - change Code Editor";
        qDebug() << "index: " << index;
        qDebug() << "size: " << openCentralWidgets.count();
        CodeEdit *editor = openCentralWidgets.at(index)->getCodeEdit();
        activeCodeEdit->loadCodeEdit(editor);
        breakpointList->disconnect();
        bookmarkList->disconnect();
        breakpointList->reload(editor->getBreakpointList());
        bookmarkList->reload(editor->getBookmarkList());
        updateAnalysersSlot(this->activeCodeEdit);
        connect(this->activeCodeEdit, SIGNAL(bookmarkListAdd(int)), bookmarkList, SLOT(bookmarkListAddSlot(int)));
        connect(this->activeCodeEdit, SIGNAL(bookmarkListRemove(int)), bookmarkList, SLOT(bookmarkListRemoveSlot(int)));
        connect(this->activeCodeEdit, SIGNAL(breakpointListAdd(int)), breakpointList, SLOT(breakpointListAddSlot(int)));
        connect(this->activeCodeEdit, SIGNAL(breakpointListRemove(int)), breakpointList, SLOT(breakpointListRemoveSlot(int)));
    }
}


void WDockManager::changeTabName(CodeEdit *editor, QString name)
{
    //wTab->setTabText(wTab->indexOf(editor), name);
}


void WDockManager::closeTab(int index)
{
    emit saveCodeEdit(openCentralWidgets.at(index)->getCodeEdit());
    openCentralWidgets.removeAt(index);
    wTab->removeTab(index);
    if (wTab->currentIndex() < 0)
    {
        delete centralBase;
        centralBase = NULL;
        this->hideDockWidgetArea(1);
        this->hideDockWidgetArea(2);
    }
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
    CodeEdit *newEditor = new CodeEdit(wMainWindow, true, wName, wPath, NULL);
    this->codeEditList.append(newEditor);
    BaseEditor *newBaseEditor;
    if (centralBase == NULL)
    {
        newBaseEditor = new BaseEditor(NULL, this, newEditor, false);
        centralBase = new BaseEditor(splitter, this, newEditor, false);
        activeCodeEdit = centralBase->getCodeEdit();
        splitter->addWidget(centralBase);
        if (wMainWindow->dockWidgets == false)
        {
            wMainWindow->CreateDockWidgets();
        }
        else
        {
            this->showDockWidgetArea(1);
            this->showDockWidgetArea(2);
        }
    }
    else
    {
        newBaseEditor = new BaseEditor(NULL, this, newEditor, false);
        activeCodeEdit->loadCodeEdit(newBaseEditor->getCodeEdit());
    }
    openCentralWidgets.append(newBaseEditor);
    wTab->addTab(wName);
    wTab->setCurrentIndex(wTab->count()-1);
    //testovaci nazev
    //wTab->setTabText(wTab->currentIndex(), "aa.asm");
    //add tab tooltip with path
    connect(newEditor, SIGNAL(changedTabName(CodeEdit*, QString)), this, SLOT(changeTabName(CodeEdit*, QString)));
    connect(newEditor, SIGNAL(updateAnalysers(CodeEdit*)), this, SLOT(updateAnalysersSlot(CodeEdit*)));
}

void WDockManager::addDockWidget(int code)
{
    WDock *newWDock = new WDock(this, code, wMainWindow);
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


BookmarkList* WDockManager::getBookmarkList()
{
    return bookmarkList;
}

BreakpointList* WDockManager::getBreakpointList()
{
    return breakpointList;
}


void WDockManager::createBreakpointList(QDockWidget *wDockWidget)
{
    breakpointList = new BreakpointList(wDockWidget);
}

void WDockManager::createBookmarkList(QDockWidget *wDockWidget)
{
    bookmarkList = new BookmarkList(wDockWidget);
}


void WDockManager::updateAnalysersSlot(CodeEdit *editor)
{
    Analys *analyser = new Analys(*(editor->getTextEdit()->document()));
    ((AnalyserWidget*)(this->getDockWidget(wAnalysVar)->widget()))->fill(analyser->getVar());
    ((AnalyserWidget*)(this->getDockWidget(wAnalysFunc)->widget()))->fill(analyser->getFunc());
}


/////
///// WDock
/////

WDock::WDock(WDockManager *parent, int code, MainForm *mainWindow)
{
    switch (code)
    {
        case wBookmarkList:
        {
            wDockWidget = new QDockWidget("Bookmarks", mainWindow);
            wDockWidget->setAllowedAreas(Qt::RightDockWidgetArea);
            mainWindow->addDockWidget(Qt::RightDockWidgetArea, wDockWidget);
            parent->createBookmarkList(wDockWidget);
            area = 1;
            wDockWidget->setWidget(parent->getBookmarkList());
	        break;
        }
        case wBreakpointList:
        {
            wDockWidget = new QDockWidget("Breakpoints", mainWindow);
            wDockWidget->setAllowedAreas(Qt::RightDockWidgetArea);
            mainWindow->addDockWidget(Qt::RightDockWidgetArea, wDockWidget);
            parent->createBreakpointList(wDockWidget);
            area = 1;
            wDockWidget->setWidget(parent->getBreakpointList());
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
        case wAnalysVar:
        {
            wDockWidget = new QDockWidget("Variables", mainWindow);
            wDockWidget->setAllowedAreas(Qt::RightDockWidgetArea);
            mainWindow->addDockWidget(Qt::RightDockWidgetArea, wDockWidget);
            AnalyserWidget *newDock = new AnalyserWidget(wDockWidget);
            area = 1;
            wDockWidget->setWidget(newDock);
            break;
        }
        case wAnalysFunc:
        {
            wDockWidget = new QDockWidget("Functions", mainWindow);
            wDockWidget->setAllowedAreas(Qt::RightDockWidgetArea);
            mainWindow->addDockWidget(Qt::RightDockWidgetArea, wDockWidget);
            AnalyserWidget *newDock = new AnalyserWidget(wDockWidget);
            area = 1;
            wDockWidget->setWidget(newDock);
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
