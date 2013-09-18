/**
 * @brief C++ file of WDockManager class
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



/**
 * @brief Constructor. Inits docking and tabbar.
 * @param mainWindow Parent window.
 */
WDockManager::WDockManager(QWidget *parent, QWidget *centralWidget)
    : QObject(parent)
{
    //qDebug() << "WDockManager: WDockManager()";
    //wMainWindow = mainWindow;
    QVBoxLayout *layout = new QVBoxLayout(centralWidget);
    wTab = new TabBar(centralWidget);
    splitter = new QSplitter(centralWidget);
    splitter->show();
    wTab->setTabsClosable(true);
    wTab->setMovable(true);
    wRight = NULL;
    wLeft = NULL;
    wBottom = NULL;
    bookmarkList = NULL;
    breakpointList = NULL;
    activeCodeEdit = NULL;
    centralBase = NULL;
    this->dockWidgets = false;
    connect(wTab, SIGNAL(tabCloseRequested(int)),this, SLOT(closeTab(int)));
    connect(wTab, SIGNAL(currentChanged(int)), this, SLOT(changeCodeEditor(int)));
    connect(wTab, SIGNAL(tabMoved(int, int)), this, SLOT(moveEditorsSlot(int, int)));

    layout->addWidget(wTab);
    layout->addWidget(splitter);
    centralWidget->setLayout(layout);
    //qDebug() << "WDockManager: return WDockManager()";
}



/**
 * @brief Handles change of active CodeEdit (another CodeEdit recieves focus)
 * @param editor New active editor
 */
void WDockManager::changeActiveCodeEdit(CodeEdit *editor)
{
    //qDebug() << "WDockManager: changeActiveCodeEdit()";
    if (this->activeCodeEdit != editor)
    {
        //qDebug() << "wdockmanager - change active Code Editor";
        this->activeCodeEdit = editor;
        if (breakpointList != NULL && bookmarkList != NULL)
        {
            breakpointList->disconnect();
            bookmarkList->disconnect();
            breakpointList->reload(activeCodeEdit->getBreakpointList());
            bookmarkList->reload(activeCodeEdit->getBookmarkList());
            updateAnalysersSlot(this->activeCodeEdit);
            connect(this->activeCodeEdit, SIGNAL(bookmarkListAdd(int)), bookmarkList, SLOT(bookmarkListAddSlot(int)));
            connect(this->activeCodeEdit, SIGNAL(bookmarkListRemove(int)), bookmarkList, SLOT(bookmarkListRemoveSlot(int)));
            connect(this->activeCodeEdit, SIGNAL(breakpointListAdd(int)), breakpointList, SLOT(breakpointListAddSlot(int)));
            connect(this->activeCodeEdit, SIGNAL(breakpointListRemove(int)), breakpointList, SLOT(breakpointListRemoveSlot(int)));
        }
        wTab->setCurrentIndex(codeEditList.indexOf(this->activeCodeEdit->getParentCodeEdit()));
    }
    //qDebug() << "WDockManager: return changeActiveCodeEdit()";
}


/**
 * @brief Handles tab change of CodeEdit
 * @param index Position of base CodeEdit in QList
 */
void WDockManager::changeCodeEditor(int index)
{
    //qDebug() << "WDockManager: changeCodeEditor()";
    if (activeCodeEdit != NULL && index >= 0)
    {
        //qDebug() << "wdockmanager - change Code Editor";
        //qDebug() << "index: " << index;
        //qDebug() << "size: " << openCentralWidgets.count();
        CodeEdit *editor = openCentralWidgets.at(index)->getCodeEdit();
        activeCodeEdit->loadCodeEdit(editor);
        if (breakpointList != NULL && bookmarkList != NULL)
        {
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
    //qDebug() << "WDockManager: return changeCodeEditor()";
}


/**
 * @brief
 * @param
 * @param
 * @param
 */
void WDockManager::changeTabStatusSlot(QString name, QString path, bool changed)
{
    //qDebug() << "WDockManager: changeTabStatusSlot()";
    //wTab->setTabText(wTab->indexOf(editor), name);
    for (int i = 0; i < wTab->count(); i++)
    {
        if (wTab->tabText(i) == name && wTab->tabToolTip(i) == path)
        {
            //qDebug() << "wdockmanager: change tab status slot";
            wTab->tabChanged(i, changed);
            wTab->update();
            break;
        }
    }
    //qDebug() << "WDockManager: return changeTabStatusSlot()";
}


void WDockManager::closeTab(int index)
{
    //qDebug() << "WDockManager: closeTab()";
    emit saveCodeEdit(openCentralWidgets.at(index)->getCodeEdit());
    openCentralWidgets.removeAt(index);
    wTab->removeTab(index);
    wTab->tabRemoved(index);
    if (wTab->currentIndex() < 0)
    {
        delete centralBase;
        centralBase = NULL;
        this->hideDockWidgetArea(1);
        this->hideDockWidgetArea(2);
    }
    //qDebug() << "WDockManager: return closeTab()";
}



void WDockManager::setTabChanged()
{
    //qDebug() << "WDockManager: setTabChanged()";
    //openCentralWidgets.at(wTab->currentIndex())->getCodeEdit()->setChanged();
    codeEditList.at(wTab->currentIndex())->setChanged();
    //qDebug() << "WDockManager: return setTabChanged()";
}


void WDockManager::setTabSaved()
{
    //qDebug() << "WDockManager: setTabSaved()";
    //qDebug() << "wdockmanager: set tab saved";
    codeEditList.at(wTab->currentIndex())->setSaved();
    //qDebug() << "WDockManager: return setTabSaved()";
}


WTextEdit* WDockManager::getCentralTextEdit()
{
    return activeCodeEdit->getTextEdit();
}



WTextEdit* WDockManager::getTabTextEdit(int index)
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


/**
 * @brief
 */
QString WDockManager::getCentralName()
{
    return openCentralWidgets.at(wTab->currentIndex())->getCodeEdit()->getName();
}


/**
 * @brief
 */
QString WDockManager::getCentralPath()
{
    return openCentralWidgets.at(wTab->currentIndex())->getCodeEdit()->getPath();
}


/**
 * @brief
 * @param wName
 */
void WDockManager::setCentralName(QString wName)
{
    //qDebug() << "WDockManager: setCentralName()";
    openCentralWidgets.at(wTab->currentIndex())->getCodeEdit()->setName(wName);
    wTab->setTabText(wTab->currentIndex(), wName);
    //qDebug() << "WDockManager: return setCentralName()";
}


/**
 * @brief
 * @param wPath
 */
void WDockManager::setCentralPath(QString wPath)
{
    //qDebug() << "WDockManager: setCentralPath()";
    openCentralWidgets.at(wTab->currentIndex())->getCodeEdit()->setPath(wPath);
    wTab->setTabToolTip(wTab->currentIndex(), wPath);
    //qDebug() << "WDockManager: return setCentralPath()";
}



void WDockManager::addUntrackedCentralWidget(QString wName, QString wPath)
{
    //qDebug() << "WDockManager: addUntrackedCentralWidget()";
    bool found = false;
    for (int i = 0; i < wTab->count(); i++)
    {
        if (wTab->tabText(i) == wName && wTab->tabToolTip(i) == wPath)
        {
            found = true;
            break;
        }
    }
    if (found != true)
    {
        CodeEdit *newEditor = new CodeEdit((QWidget *)(this->parent()), true, wName, wPath, NULL);
        this->codeEditList.append(newEditor);
        BaseEditor *newBaseEditor;
        if (centralBase == NULL)
        {
            //qDebug() << "WDockManager: Create centralBase";
            newBaseEditor = new BaseEditor(NULL, this, newEditor, false);
            //qDebug() << "WDockManager: Created BaseEditor1";
            centralBase = new BaseEditor(splitter, this, newEditor, false);
            //qDebug() << "WDockManager: Created BaseEditor2";
            activeCodeEdit = centralBase->getCodeEdit();
            //qDebug() << "WDockManager: activeCodeEdit assigned";
            splitter->addWidget(centralBase);
            //qDebug() << "WDockManager: Create splitter";
        }
        else
        {
            newBaseEditor = new BaseEditor(NULL, this, newEditor, false);
            activeCodeEdit->loadCodeEdit(newBaseEditor->getCodeEdit());
        }
        openCentralWidgets.append(newBaseEditor);
        wTab->addTab(wName);
        wTab->tabAdded();
        wTab->setCurrentIndex(wTab->count()-1);
        wTab->setTabToolTip(wTab->currentIndex(), wPath);
        //testovaci nazev
        //wTab->setTabText(wTab->currentIndex(), "aa.asm");
        //add tab tooltip with path
        newEditor->connectAct();
        activeCodeEdit->connectAct();
        connect(newEditor, SIGNAL(changedTabStatus(QString, QString, bool)), this, SLOT(changeTabStatusSlot(QString, QString, bool)));
        connect(activeCodeEdit, SIGNAL(changedTabStatus(QString, QString, bool)), this, SLOT(changeTabStatusSlot(QString, QString, bool)));
    }
    //qDebug() << "WDockManager: return addUntrackedCentralWidget()";
}


void WDockManager::addUntrackedCentralWidget(QString wName, QString wPath, QStringList text)
{
    //qDebug() << "WDockManager: addUntrackedCentralWidget(text)";
    bool found = false;
    for (int i = 0; i < wTab->count(); i++)
    {
        if (wTab->tabText(i) == wName && wTab->tabToolTip(i) == wPath)
        {
            found = true;
            break;
        }
    }
    if (found != true)
    {
        CodeEdit *newEditor = new CodeEdit((QWidget *)(this->parent()), true, wName, wPath, NULL);
        for (int i = 0; i < text.size(); i++)
        {
            newEditor->getTextEdit()->append(text.at(i));
        }
        this->codeEditList.append(newEditor);
        BaseEditor *newBaseEditor;
        if (centralBase == NULL)
        {
            //qDebug() << "WDockManager: Create centralBase";
            newBaseEditor = new BaseEditor(NULL, this, newEditor, false);
            //qDebug() << "WDockManager: Created BaseEditor1";
            centralBase = new BaseEditor(splitter, this, newEditor, false);
            //qDebug() << "WDockManager: Created BaseEditor2";
            activeCodeEdit = centralBase->getCodeEdit();
            //qDebug() << "WDockManager: activeCodeEdit assigned";
            splitter->addWidget(centralBase);
            //qDebug() << "WDockManager: Create splitter";
        }
        else
        {
            newBaseEditor = new BaseEditor(NULL, this, newEditor, false);
            activeCodeEdit->loadCodeEdit(newBaseEditor->getCodeEdit());
        }
        openCentralWidgets.append(newBaseEditor);
        wTab->addTab(wName);
        wTab->tabAdded();
        wTab->setCurrentIndex(wTab->count()-1);
        wTab->setTabToolTip(wTab->currentIndex(), wPath);
        newEditor->connectAct();
        activeCodeEdit->connectAct();
        //testovaci nazev
        //wTab->setTabText(wTab->currentIndex(), "aa.asm");
        //add tab tooltip with path
        connect(newEditor, SIGNAL(changedTabStatus(QString, QString, bool)), this, SLOT(changeTabStatusSlot(QString, QString, bool)));
        connect(activeCodeEdit, SIGNAL(changedTabStatus(QString, QString, bool)), this, SLOT(changeTabStatusSlot(QString, QString, bool)));
        newEditor->setChanged();
    }
    //qDebug() << "WDockManager: return addUntrackedCentralWidget(text)";
}




void WDockManager::addCentralWidget(QString wName, QString wPath)
{
    //qDebug() << "WDockManager: addCentralWidget()";
    bool found = false;
    for (int i = 0; i < wTab->count(); i++)
    {
        if (wTab->tabText(i) == wName && wTab->tabToolTip(i) == wPath)
        {
            found = true;
            break;
        }
    }
    //qDebug() << "WDockManager: Found test";
    if (found != true)
    {
        CodeEdit *newEditor = new CodeEdit((QWidget *)(this->parent()), true, wName, wPath, NULL);
        this->codeEditList.append(newEditor);
        BaseEditor *newBaseEditor;
        if (centralBase == NULL)
        {
            //qDebug() << "WDockManager: Create centralBase";
            newBaseEditor = new BaseEditor(NULL, this, newEditor, false);
            //qDebug() << "WDockManager: Created BaseEditor1";
            centralBase = new BaseEditor(splitter, this, newEditor, false);
            //qDebug() << "WDockManager: Created BaseEditor2";
            activeCodeEdit = centralBase->getCodeEdit();
            /*QPalette palette = activeCodeEdit->palette();
            palette.setColor(QPalette::Base, QColor::fromRgbF(1, 1, 0, 0.05));
            activeCodeEdit->setPalette(palette);*/
            //qDebug() << "WDockManager: activeCodeEdit assigned";
            splitter->addWidget(centralBase);
            //qDebug() << "WDockManager: Create splitter";
            if (wName != NULL && wPath != NULL)
            {
                if (this->dockWidgets == false)
                {
                    emit createDockWidgets();
                    //wMainWindow->CreateDockWidgets();
                }
                else
                {
                    this->showDockWidgetArea(1);
                    this->showDockWidgetArea(2);
                }
            }
        }
        else
        {
            newBaseEditor = new BaseEditor(NULL, this, newEditor, false);
            activeCodeEdit->loadCodeEdit(newBaseEditor->getCodeEdit());
        }
        newEditor->changeHeight();
        openCentralWidgets.append(newBaseEditor);
        wTab->addTab(wName);
        wTab->tabAdded();
        wTab->setCurrentIndex(wTab->count()-1);
        wTab->setTabToolTip(wTab->currentIndex(), wPath);
        newEditor->connectAct();
        activeCodeEdit->connectAct();
        //testovaci nazev
        //wTab->setTabText(wTab->currentIndex(), "aa.asm");
        //add tab tooltip with path
        connect(newEditor, SIGNAL(changedTabStatus(QString, QString, bool)), this, SLOT(changeTabStatusSlot(QString, QString, bool)));
        connect(activeCodeEdit, SIGNAL(changedTabStatus(QString, QString, bool)), this, SLOT(changeTabStatusSlot(QString, QString, bool)));
        //connect(newEditor, SIGNAL(updateAnalysers(CodeEdit*)), this, SLOT(updateAnalysersSlot(CodeEdit*)));
    }
    //qDebug() << "WDockManager: return addCentralWidget()";
}


void WDockManager::addDockWidget(int code)
{
    //qDebug() << "WDockManager: addDockWidget()";
    WDock *newWDock;
    if (code == wSimulationInfo)
    {
        this->addSimDockWidgetP1();
        //qDebug() << "WDockManager: return addDockWidget()";
        return;
        //newWDock = new WDock(this, code, (QWidget *)(this->parent()));
    }
    else
    {
        newWDock = new WDock(this, code, (QWidget *)(this->parent()));
    }
    if (getDockWidgetArea(newWDock->getArea()) != NULL)
    {
        emit tabifyDockWidget(getDockWidgetArea(newWDock->getArea()), newWDock->getQDockWidget());
        //wMainWindow->tabifyDockWidget(getDockWidgetArea(newWDock->getArea()), newWDock->getQDockWidget());
    }
    openDockWidgets.append(newWDock);
    //qDebug() << "WDockManager: return addDockWidget()";
}


void WDockManager::addSimDockWidgetP1()
{
    //qDebug() << "WDockManager: addSimDockWidgetP1()";
    emit getSimProjectData();
    //qDebug() << "WDockManager: return addSimDockWidgetP1()";
}


void WDockManager::addSimDockWidgetP2(QString path, MCUSimControl* simControl)
{
    //qDebug() << "WDockManager: addSimDockWidgetP2()";
    WDock *newWDock = new WDock(this, wSimulationInfo, (QWidget *)(this->parent()), path, simControl);
    if (getDockWidgetArea(newWDock->getArea()) != NULL)
    {
        emit tabifyDockWidget(getDockWidgetArea(newWDock->getArea()), newWDock->getQDockWidget());
        //wMainWindow->tabifyDockWidget(getDockWidgetArea(newWDock->getArea()), newWDock->getQDockWidget());
    }
    openDockWidgets.append(newWDock);
    //qDebug() << "WDockManager: return addSimDockWidgetP2()";
}


QDockWidget* WDockManager::getDockWidget(int code)
{
    //qDebug() << "WDockManager: getDockWidget()";
    QList<WDock*>::iterator i;
    for (i = openDockWidgets.begin(); i != openDockWidgets.end(); i++)
    {
        if ((*i)->cmpCode(code) == true)
        {
            //qDebug() << "WDockManager: return getDockWidget()";
            return (*i)->getQDockWidget();
        }
    }
    //qDebug() << "WDockManager: return getDockWidget()";
    return NULL;
}


QDockWidget* WDockManager::getDockWidgetArea(int area)
{
    //qDebug() << "WDockManager: getDockWidgetArea()";
    QList<WDock*>::iterator i;
    for (i = openDockWidgets.begin(); i != openDockWidgets.end(); i++)
    {
        if ((*i)->cmpArea(area) == true)
        {
            //qDebug() << "WDockManager: return getDockWidgetArea()";
            return (*i)->getQDockWidget();
        }
    }
    //qDebug() << "WDockManager: return getDockWidgetArea()";
    return NULL;
}


void WDockManager::hideDockWidgetArea(int area)
{
    //qDebug() << "WDockManager: hideDockWidgetArea()";
    QList<WDock*>::iterator i;
    for (i = openDockWidgets.begin(); i != openDockWidgets.end(); i++)
    {
        if ((*i)->cmpArea(area) == true)
        {
            (*i)->getQDockWidget()->hide();
        }
    }
    //qDebug() << "WDockManager: return hideDockWidgetArea()";
}

void WDockManager::showDockWidgetArea(int area)
{
    //qDebug() << "WDockManager: showDockWidgetArea()";
    QList<WDock*>::iterator i;
    for (i = openDockWidgets.begin(); i != openDockWidgets.end(); i++)
    {
        if ((*i)->cmpArea(area) == true)
        {
            (*i)->getQDockWidget()->show();
        }
    }
    //qDebug() << "WDockManager: return showDockWidgetArea()";
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
    //qDebug() << "WDockManager: createBreakpointList()";
    breakpointList = new BreakpointList(wDockWidget);
    //qDebug() << "WDockManager: return createBreakpointList()";
}

void WDockManager::createBookmarkList(QDockWidget *wDockWidget)
{
    //qDebug() << "WDockManager: createBookmarkList()";
    bookmarkList = new BookmarkList(wDockWidget);
    //qDebug() << "WDockManager: return createBookmarkList()";
}


void WDockManager::updateAnalysersSlot(CodeEdit *editor)
{
    //qDebug() << "WDockManager: updateAnalysersSlot()";
    if (this->getDockWidget(wAnalysVar) != NULL && this->getDockWidget(wAnalysFunc) != NULL)
    {
        Analys *analyser = new Analys(*(editor->getTextEdit()->document()));
        ((AnalyserWidget*)(this->getDockWidget(wAnalysVar)->widget()))->fill(analyser->getVar());
        ((AnalyserWidget*)(this->getDockWidget(wAnalysFunc)->widget()))->fill(analyser->getFunc());
    }
    //qDebug() << "WDockManager: return updateAnalysersSlot()";
}


void WDockManager::changeLine(QListWidgetItem *item)
{
    //qDebug() << "WDockManager: changeLine()";
    QString a = item->text().right(item->text().length() - item->text().lastIndexOf('[') - 1);
    a = a.left(a.length() - 1);
    this->getCentralWidget()->getTextEdit()->setPosition(a.toInt());
    //qDebug() << "WDockManager: return changeLine()";
    
}


void WDockManager::moveEditorsSlot(int from, int to)
{
    //qDebug() << "WDockManager: moveEditorsSlot()";
    codeEditList.swap(from, to);
    openCentralWidgets.swap(from, to);
    //qDebug() << "WDockManager: return moveEditorsSlot()";
}


void WDockManager::setCentralByName(QString fileName)
{
    //qDebug() << "WDockManager: setCentralByName()";
    if (fileName != "")
    {
        for (int i = 0; i < this->getTabCount(); i++)
        {
            //qDebug() << "WDockManager: tabtext" << wTab->tabText(i) << "fileName" << fileName;
            if (wTab->tabText(i) == fileName)
            {
                //qDebug() << "WDockManager: CodeEdit found";
                if (wTab->currentIndex() != i)
                {
                    this->changeCodeEditor(i);
                    wTab->setCurrentIndex(i);
                }
                return;
            }
        }
    }
    //qDebug() << "WDockManager: return setCentralByName()";
}


void WDockManager::setEditorsReadOnly(bool readonly)
{
    for (int i = 0; i < codeEditList.count(); i++)
    {
        activeCodeEdit->getTextEdit()->setReadOnly(readonly);
    }
}


void WDockManager::addDockW(Qt::DockWidgetArea area, QDockWidget* dockWidget)
{
    emit addDockWidget(area, dockWidget);
}


/*void WDockManager::dockWidgetsCreated()
{
    this->dockWidgets = true;
}*/




/////
///// WDock
/////

WDock::WDock(WDockManager *parent, int code, QWidget *parentWindow)
{
    //qDebug() << "WDock: WDock()";
    switch (code)
    {
        case wBookmarkList:
        {
            wDockWidget = new QDockWidget("Bookmarks", parentWindow);
            wDockWidget->setAllowedAreas(Qt::RightDockWidgetArea);
            parent->addDockW(Qt::RightDockWidgetArea, wDockWidget);
            //mainWindow->addDockWidget(Qt::RightDockWidgetArea, wDockWidget);
            parent->createBookmarkList(wDockWidget);
            area = 1;
            wDockWidget->setWidget(parent->getBookmarkList());
	        break;
        }
        case wBreakpointList:
        {
            wDockWidget = new QDockWidget("Breakpoints", parentWindow);
            wDockWidget->setAllowedAreas(Qt::RightDockWidgetArea);
            parent->addDockW(Qt::RightDockWidgetArea, wDockWidget);
            //mainWindow->addDockWidget(Qt::RightDockWidgetArea, wDockWidget);
            parent->createBreakpointList(wDockWidget);
            area = 1;
            wDockWidget->setWidget(parent->getBreakpointList());
	        break;
        }
        case wCompileInfo:
        {
            wDockWidget = new QDockWidget("Compiler Info", parentWindow);
            wDockWidget->setAllowedAreas(Qt::BottomDockWidgetArea);
            parent->addDockW(Qt::BottomDockWidgetArea, wDockWidget);
            //mainWindow->addDockWidget(Qt::BottomDockWidgetArea, wDockWidget);
            QPlainTextEdit *newWidget = new QPlainTextEdit(wDockWidget);
            newWidget->setFont(QFont("Andale Mono", 10));
            area = 2;
            newWidget->setReadOnly(true);
            wDockWidget->setWidget(newWidget);
	        break;
        }
        case wSimulationInfo:
        {
            qDebug() << "WDock: invalid usage of constructor";
            /*wDockWidget = new QDockWidget("Simulation Info", parentWindow);
            wDockWidget->setAllowedAreas(Qt::BottomDockWidgetArea);
            parent->addDockW(Qt::BottomDockWidgetArea, wDockWidget);
            //mainWindow->addDockWidget(Qt::BottomDockWidgetArea, wDockWidget);
            PicoBlazeGrid *newWidget = new PicoBlazeGrid(wDockWidget, mainWindow->getProjectMan()->getActive()->getSimControl());
            newWidget->setProjectPath(mainWindow->getProjectMan()->getActive()->prjPath);
            area = 2;
            wDockWidget->setWidget(newWidget);
            newWidget->fixHeight();
            qDebug() << "WSimulationInfo: height fixed";*/
	        break;
        }
        case wAnalysVar:
        {
            wDockWidget = new QDockWidget("Variables", parentWindow);
            wDockWidget->setAllowedAreas(Qt::RightDockWidgetArea);
            parent->addDockW(Qt::RightDockWidgetArea, wDockWidget);
            //mainWindow->addDockWidget(Qt::RightDockWidgetArea, wDockWidget);
            AnalyserWidget *newDock = new AnalyserWidget(wDockWidget);
            area = 1;
            wDockWidget->setWidget(newDock);
            newDock->connect(newDock, SIGNAL(itemDoubleClicked(QListWidgetItem *)), parent, SLOT(changeLine(QListWidgetItem *)));
            break;
        }
        case wAnalysFunc:
        {
            wDockWidget = new QDockWidget("Functions", parentWindow);
            wDockWidget->setAllowedAreas(Qt::RightDockWidgetArea);
            parent->addDockW(Qt::RightDockWidgetArea, wDockWidget);
            //mainWindow->addDockWidget(Qt::RightDockWidgetArea, wDockWidget);
            AnalyserWidget *newDock = new AnalyserWidget(wDockWidget);
            area = 1;
            wDockWidget->setWidget(newDock);
            break;
        }
    }
    this->code=code;
    //qDebug() << "WDock: return WDock()";
}



WDock::WDock(WDockManager *parent, int code, QWidget *parentWindow, QString path, MCUSimControl* simControl)
{
    switch (code)
    {
        case wSimulationInfo:
        {
            wDockWidget = new QDockWidget("Simulation Info", parentWindow);
            wDockWidget->setAllowedAreas(Qt::BottomDockWidgetArea);
            parent->addDockW(Qt::BottomDockWidgetArea, wDockWidget);
            //mainWindow->addDockWidget(Qt::BottomDockWidgetArea, wDockWidget);
            PicoBlazeGrid *newWidget = new PicoBlazeGrid(wDockWidget, simControl);
            newWidget->setProjectPath(path);
            area = 2;
            wDockWidget->setWidget(newWidget);
            wDockWidget->show();
            newWidget->fixHeight();
            //qDebug() << "WSimulationInfo: height fixed";
            break;
        }
    }
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
