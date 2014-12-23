/**
 * @brief
 * C++ Implementation: ...
 *
 * ...
 *
 * (C) copyright 2013, 2014 Moravia Microsystems, s.r.o.
 *
 * @author Erik Chalupa <erik.chalupa@moravia-microsystems.com>
 * @ingroup WDockManager
 * @file wdockmanager.cpp
 */


#include <QtGui>
#include "wdockmanager.h"
#include "../Editor/wtextedit.h"
#include "../WelcomeScr/welcomescr.h"
#include "../AsmMacroAnalyser/asmmacroanalyser.h"
#include "../BreakpointList/breakpointlist.h"
#include "../BookmarkList/bookmarklist.h"
#include "../Editor/codeedit.h"
#include "../Editor/baseeditor.h"
#include "../PicoBlazeGrid/picoblazegrid.h"
#include "../TabBar/tabbar.h"
#include "../ExtAppOutput/extappoutput.h"
#include "../CompileInfo/compileinfo.h"
#include "../HelpDockWidget/helpdockwidget.h"



/**
 * @brief Constructor. Inits docking and tabbar.
 * @param mainWindow Parent window.
 */
WDockManager::WDockManager(QWidget *parent, QWidget *centralWidget)
    : QObject(parent)
{
    //qDebug() << "WDockManager: WDockManager()";
    //wMainWindow = mainWindow;
    m_layout = new QVBoxLayout(centralWidget);
    m_centralWidget = centralWidget;
    //wTab = new TabBar(centralWidget);
    wTab = NULL;
    splitter = new QSplitter(centralWidget);
    splitter->show();
    //wTab->setTabsClosable(true);
    //wTab->setMovable(true);
    wRight = NULL;
    wLeft = NULL;
    wBottom = NULL;
    bookmarkList = NULL;
    breakpointList = NULL;
    activeCodeEdit = NULL;
    welcomeScr = NULL;
    centralBase = NULL;
    this->dockWidgets = false;
    wDockBotPrevHeight = 0;
    wDockRightPrevWidth = 0;
    bottomVisible = true;
    rightVisible = true;

    //layout->addWidget(wTab);
    m_layout->addWidget(splitter);
    centralWidget->setLayout(m_layout);
    //qDebug() << "WDockManager: return WDockManager()";
}



//OBSOLETE!
/**
 * @brief Handles change of active CodeEdit (another CodeEdit recieves focus)
 * @param editor New active editor
 */
void WDockManager::changeActiveCodeEdit(CodeEdit */*editor*/)
{}
    //qDebug() << "WDockManager: changeActiveCodeEdit()";
    //if (this->activeCodeEdit != editor)
    //{
        //qDebug() << "wdockmanager - change active Code Editor";
        //this->activeCodeEdit = editor;
        /*if (breakpointList != NULL && editor->getParentProject() != NULL)
        {
            connect(this->activeCodeEdit,
                      SIGNAL(breakpointListAdd(int)),
                      this,
                      SLOT(breakpointListAddSlot(int))
                     );
            connect(this->activeCodeEdit,
                    SIGNAL(breakpointListRemove(int)),
                    this,
                    SLOT(breakpointListRemoveSlot(int))
                   );
        }
        if (bookmarkList != NULL)
        {
            bookmarkList->disconnect();
            bookmarkList->reload(editor->getBookmarkList());
            connect(this->activeCodeEdit, SIGNAL(bookmarkListAdd(int)), bookmarkList, SLOT(bookmarkListAddSlot(int)));
            connect(this->activeCodeEdit, SIGNAL(bookmarkListRemove(int)), bookmarkList, SLOT(bookmarkListRemoveSlot(int)));
        }*/
        //    updateAnalysersSlot(this->activeCodeEdit);
        //wTab->setCurrentIndex(codeEditList.indexOf(this->activeCodeEdit->getParentCodeEdit()));
    //}
    //qDebug() << "WDockManager: return changeActiveCodeEdit()";
//}


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
        //CodeEdit *editor = openCentralWidgets.at(index)->getCodeEdit();
        activeCodeEdit->loadCodeEdit(openCentralWidgets.at(m_currTabBarIndex).second->at(index)->getCodeEdit());
        /*if (breakpointList != NULL)
        {
            connect(this->activeCodeEdit,
                    SIGNAL(breakpointListAdd(QString, int)),
                    this,
                    SLOT(breakpointListAddSlot(QString, int))
                   );
            connect(this->activeCodeEdit,
                    SIGNAL(breakpointListRemove(QString, int)),
                    this,
                    SLOT(breakpointListRemoveSlot(QString, int))
                   );
        }
        if (bookmarkList != NULL)
        {
            connect(this->activeCodeEdit, SIGNAL(bookmarkListAdd(int)), this, SLOT(bookmarkListAddSlot(int)));
            connect(this->activeCodeEdit, SIGNAL(bookmarkListRemove(int)), this, SLOT(bookmarkListRemoveSlot(int)));
        }*/
        //    updateAnalysersSlot(this->activeCodeEdit);
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
    if (true == changed)
    {
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
    }
    else
    {
        for (int i = 0; i < wTab->count(); i++)
        {
            if (wTab->tabText(i) == "*"+name && wTab->tabToolTip(i) == path)
            {
                //qDebug() << "wdockmanager: change tab status slot";
                wTab->tabChanged(i, changed);
                wTab->update();
                break;
            }
        }
    }
    //qDebug() << "WDockManager: return changeTabStatusSlot()";
}


void WDockManager::closeTab(int index, bool openUntitled)
{
    //qDebug() << "WDockManager: closeTab()";
    emit saveCodeEdit(openCentralWidgets.at(m_currTabBarIndex).second->at(wTab->currentIndex())->getCodeEdit(), true);
    emit tabClosed(wTab->tabText(index));
    openCentralWidgets.at(m_currTabBarIndex).second->removeAt(index);
    wTab->removeTab(index);
    //wTab->tabRemoved(index);
    if (wTab->currentIndex() < 0 && true == openUntitled)
    {
        if ("untracked" != wTab->id)
        {
            qDebug() << "WDockManager: closeTab() - request untitled";
            emit requestUntitled(false);
        }
        else
        {
            qDebug() << "WDockManager: closeTab() - untracked request untitled";
            emit requestUntitled(true);
        }
        //this->removeTabBar(wTab->id);
        //this->hideDockWidgetArea(1);
        //this->hideDockWidgetArea(2);
    }
    //qDebug() << "WDockManager: return closeTab()";
}



void WDockManager::setTabChanged()
{
    //qDebug() << "WDockManager: setTabChanged()";
    openCentralWidgets.at(m_currTabBarIndex).second->at(wTab->currentIndex())->getCodeEdit()->setChanged();
    //codeEditList.at(wTab->currentIndex())->setChanged();
    activeCodeEdit->setChanged();
    //qDebug() << "WDockManager: return setTabChanged()";
}


void WDockManager::setTabSaved()
{
    //qDebug() << "WDockManager: setTabSaved()";
    //qDebug() << "wdockmanager: set tab saved";
    //codeEditList.at(wTab->currentIndex())->setSaved();
    openCentralWidgets.at(m_currTabBarIndex).second->at(wTab->currentIndex())->getCodeEdit()->setSaved();
    activeCodeEdit->setSaved();
    //qDebug() << "WDockManager: return setTabSaved()";
}


WTextEdit* WDockManager::getCentralTextEdit()
{
    return activeCodeEdit->getTextEdit();
}



WTextEdit* WDockManager::getTabTextEdit(int index)
{
    return openCentralWidgets.at(m_currTabBarIndex).second->at(index)->getCodeEdit()->getTextEdit();
}



CodeEdit* WDockManager::getCentralWidget()
{
    return activeCodeEdit;
}



CodeEdit* WDockManager::getTabWidget(int index)
{
    return openCentralWidgets.at(m_currTabBarIndex).second->at(index)->getCodeEdit();
}



int WDockManager::getTabCount()
{
    if (NULL == wTab)
    {
        return 0;
    }
    return wTab->count();
}


/**
 * @brief
 */
QString WDockManager::getCentralName()
{
    if (NULL == wTab)
    {
        return QString();
    }
    return openCentralWidgets.at(m_currTabBarIndex).second->at(wTab->currentIndex())->getCodeEdit()->getName();
}


/**
 * @brief
 */
QString WDockManager::getCentralPath()
{
    if (NULL == wTab)
    {
        return QString();
    }
    return openCentralWidgets.at(m_currTabBarIndex).second->at(wTab->currentIndex())->getCodeEdit()->getPath();
}


/**
 * @brief
 * @param wName
 */
void WDockManager::setCentralName(QString wName)
{
    //qDebug() << "WDockManager: setCentralName()";
    openCentralWidgets.at(m_currTabBarIndex).second->at(wTab->currentIndex())->getCodeEdit()->setName(wName);
    activeCodeEdit->setName(wName);
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
    openCentralWidgets.at(m_currTabBarIndex).second->at(wTab->currentIndex())->getCodeEdit()->setPath(wPath);
    activeCodeEdit->setPath(wPath);
    wTab->setTabToolTip(wTab->currentIndex(), wPath);
    //qDebug() << "WDockManager: return setCentralPath()";
}



bool WDockManager::addUntrackedCentralWidget(QString wName, QString wPath)
{
    //qDebug() << "WDockManager: addUntrackedCentralWidget()";
    bool found = false;
    int tab;
    for (tab = 0; wTab != NULL && tab < wTab->count(); tab++)
    {
        if (wTab->tabText(tab) == wName && wTab->tabToolTip(tab) == wPath)
        {
            found = true;
            break;
        }
    }
    if (found != true)
    {
        CodeEdit *newEditor = new CodeEdit((QWidget *)(this->parent()), true, wName, wPath, NULL);
        //this->codeEditList.append(newEditor);
        connect(newEditor,
               SIGNAL(changedTabStatus(QString, QString, bool)),
               this,
               SLOT(changeTabStatusSlot(QString, QString, bool))
              );
        BaseEditor *newBaseEditor;
        if (centralBase == NULL)
        {
            deleteCentralWelcome();
            //qDebug() << "WDockManager: Create centralBase";
            newBaseEditor = new BaseEditor(NULL, this, newEditor, false);
            //qDebug() << "WDockManager: Created BaseEditor1";
            centralBase = new BaseEditor(splitter, this, newEditor, false);
            //qDebug() << "WDockManager: Created BaseEditor2";
            activeCodeEdit = centralBase->getCodeEdit();
            activeCodeEdit->connectAct();
            connect(activeCodeEdit,
                    SIGNAL(changedTabStatus(QString, QString, bool)),
                    this,
                    SLOT(changeTabStatusSlot(QString, QString, bool))
                   );
            connect(activeCodeEdit,
                    SIGNAL(breakpointEmit(QString, int)),
                    this,
                    SLOT(breakpointEmitSlot(QString, int))
                   );
            connect(activeCodeEdit,
                    SIGNAL(breakpointsAddLines(QString, int, int)),
                    this,
                    SLOT(breakpointsAddLinesSlot(QString, int, int))
                   );
            connect(activeCodeEdit,
                    SIGNAL(breakpointsRemoveLines(QString, int, int)),
                    this,
                    SLOT(breakpointsRemoveLinesSlot(QString, int, int))
                   );
            connect(activeCodeEdit,
                    SIGNAL(bookmarkEmit(QString, int)),
                    this,
                    SLOT(bookmarkEmitSlot(QString, int))
                   );
            connect(activeCodeEdit,
                    SIGNAL(bookmarksAddLines(QString, int, int)),
                    this,
                    SLOT(bookmarksAddLinesSlot(QString, int, int))
                   );
            connect(activeCodeEdit,
                    SIGNAL(bookmarksRemoveLines(QString, int, int)),
                    this,
                    SLOT(bookmarksRemoveLinesSlot(QString, int, int))
                   );
            /*connect(activeCodeEdit,
                    SIGNAL(breakpointListRemove(QString, int)),
                    this,
                    SLOT(breakpointListRemoveSlot(QString, int))
                );*/
            //qDebug() << "WDockManager: activeCodeEdit assigned";
            splitter->addWidget(centralBase);
            //qDebug() << "WDockManager: Create splitter";
            if (this->dockWidgets == true)
            {
                //emit createDockWidgets();
                //wMainWindow->CreateDockWidgets();
            //}
            //else
            //{
                //activeCodeEdit->show();
                this->showDockWidgetArea(1);
                this->showDockWidgetArea(2);
            }
            emit centralCreated();
        }
        else
        {
            newBaseEditor = new BaseEditor(NULL, this, newEditor, false);
            activeCodeEdit->loadCodeEdit(newBaseEditor->getCodeEdit());
            //activeCodeEdit->show();
            //this->showDockWidgetArea(1);
            //this->showDockWidgetArea(2);
        }
        openCentralWidgets.at(m_currTabBarIndex).second->append(newBaseEditor);
        wTab->addTab(wName);
        wTab->tabAdded();
        wTab->setCurrentIndex(wTab->count()-1);
        wTab->setTabToolTip(wTab->currentIndex(), wPath);
        //testovaci nazev
        //wTab->setTabText(wTab->currentIndex(), "aa.asm");
        //add tab tooltip with path
        newBaseEditor->getCodeEdit()->connectAct();
        //newEditor->connectAct(true);
        //activeCodeEdit->connectAct();
        /*connect(newEditor,
                SIGNAL(changedTabStatus(QString, QString, bool)),
                this,
                SLOT(changeTabStatusSlot(QString, QString, bool))
               );
        connect(newEditor,
                SIGNAL(breakpointListAdd(QString, int)),
                this,
                SLOT(breakpointListAddSlot(QString, int))
               );
        connect(newEditor,
                SIGNAL(breakpointListRemove(QString, int)),
                this,
                SLOT(breakpointListRemoveSlot(QString, int))
               );*/
        //connect(activeCodeEdit, SIGNAL(changedTabStatus(QString, QString, bool)), this, SLOT(changeTabStatusSlot(QString, QString, bool)));
        if (wPath == "untracked")
        {
            newEditor->setChanged();
        }
    }
    else
    {
        wTab->setCurrentIndex(tab);
        return false;
    }
    return true;
    //qDebug() << "WDockManager: return addUntrackedCentralWidget()";
}


bool WDockManager::addUntrackedCentralWidget(QString wName, QString wPath, QStringList text)
{
    //qDebug() << "WDockManager: addUntrackedCentralWidget(text)";
    bool found = false;
    int tab = 0;
    for (int tab; wTab != NULL && tab < wTab->count(); tab++)
    {
        if (wTab->tabText(tab) == wName && wTab->tabToolTip(tab) == wPath)
        {
            found = true;
            break;
        }
    }
    if (found != true)
    {
        CodeEdit *newEditor = new CodeEdit((QWidget *)(this->parent()), true, wName, wPath, NULL);
        connect(newEditor,
                SIGNAL(changedTabStatus(QString, QString, bool)),
                this,
                SLOT(changeTabStatusSlot(QString, QString, bool))
               );
        //newEditor->getTextEdit()->reloadHighlighter(SourceType::PICOBLAZEASM);
        for (int i = 0; i < text.size(); i++)
        {
            newEditor->getTextEdit()->appendPlainText(text.at(i));
        }
        //this->codeEditList.append(newEditor);
        BaseEditor *newBaseEditor;
        if (centralBase == NULL)
        {
            deleteCentralWelcome();
            //qDebug() << "WDockManager: Create centralBase";
            newBaseEditor = new BaseEditor(NULL, this, newEditor, false);
            //qDebug() << "WDockManager: Created BaseEditor1";
            centralBase = new BaseEditor(splitter, this, newEditor, false);
            //qDebug() << "WDockManager: Created BaseEditor2";
            activeCodeEdit = centralBase->getCodeEdit();
            activeCodeEdit->connectAct();
            connect(activeCodeEdit,
                    SIGNAL(changedTabStatus(QString, QString, bool)),
                    this,
                    SLOT(changeTabStatusSlot(QString, QString, bool))
                );
            connect(activeCodeEdit,
                    SIGNAL(breakpointEmit(QString, int)),
                    this,
                    SLOT(breakpointEmitSlot(QString, int))
                );
            connect(activeCodeEdit,
                    SIGNAL(breakpointsAddLines(QString, int, int)),
                    this,
                    SLOT(breakpointsAddLinesSlot(QString, int, int))
                );
            connect(activeCodeEdit,
                    SIGNAL(breakpointsRemoveLines(QString, int, int)),
                    this,
                    SLOT(breakpointsRemoveLinesSlot(QString, int, int))
                );
            connect(activeCodeEdit,
                    SIGNAL(bookmarkEmit(QString, int)),
                    this,
                    SLOT(bookmarkEmitSlot(QString, int))
                );
            connect(activeCodeEdit,
                    SIGNAL(bookmarksAddLines(QString, int, int)),
                    this,
                    SLOT(bookmarksAddLinesSlot(QString, int, int))
                );
            connect(activeCodeEdit,
                    SIGNAL(bookmarksRemoveLines(QString, int, int)),
                    this,
                    SLOT(bookmarksRemoveLinesSlot(QString, int, int))
                );
            /*connect(activeCodeEdit,
                    SIGNAL(breakpointListRemove(QString, int)),
                    this,
                    SLOT(breakpointListRemoveSlot(QString, int))
                );*/
            //qDebug() << "WDockManager: activeCodeEdit assigned";
            splitter->addWidget(centralBase);
            //qDebug() << "WDockManager: Create splitter";
            if (this->dockWidgets == true)
            {
            //    emit createDockWidgets();
                //wMainWindow->CreateDockWidgets();
            //}
            //else
            //{
                //activeCodeEdit->show();
                this->showDockWidgetArea(1);
                this->showDockWidgetArea(2);
            }
            emit centralCreated();
        }
        else
        {
            newBaseEditor = new BaseEditor(NULL, this, newEditor, false);
            activeCodeEdit->loadCodeEdit(newBaseEditor->getCodeEdit());
            //activeCodeEdit->show();
            //this->showDockWidgetArea(1);
            //this->showDockWidgetArea(2);
        }
        openCentralWidgets.at(m_currTabBarIndex).second->append(newBaseEditor);
        wTab->addTab(wName);
        wTab->tabAdded();
        wTab->setCurrentIndex(wTab->count()-1);
        wTab->setTabToolTip(wTab->currentIndex(), wPath);
        newBaseEditor->getCodeEdit()->connectAct();
        //newEditor->connectAct(true);
        //activeCodeEdit->connectAct();
        /*connect(newEditor,
                SIGNAL(breakpointListAdd(QString, int)),
                this,
                SLOT(breakpointListAddSlot(QString, int))
               );
        connect(newEditor,
                SIGNAL(breakpointListRemove(QString, int)),
                this,
                SLOT(breakpointListRemoveSlot(QString, int))
               );*/
        //testovaci nazev
        //wTab->setTabText(wTab->currentIndex(), "aa.asm");
        //add tab tooltip with path
        /*connect(newEditor,
                SIGNAL(changedTabStatus(QString, QString, bool)),
                this,
                SLOT(changeTabStatusSlot(QString, QString, bool))
               );
        connect(activeCodeEdit,
                SIGNAL(changedTabStatus(QString, QString, bool)),
                this,
                SLOT(changeTabStatusSlot(QString, QString, bool))
               );*/
        newEditor->setChanged();
    }
    else
    {
        wTab->setCurrentIndex(tab);
        return false;
    }
    return true;
    //qDebug() << "WDockManager: return addUntrackedCentralWidget(text)";
}




bool WDockManager::addCentralWidget(QString wName, QString wPath)
{
    //qDebug() << "WDockManager: addCentralWidget()";
    bool found = false;
    int tab;
    /*if (wTab == NULL)
    {
        qDebug() << "wTab == NULL!";
    }*/
    for (tab = 0; wTab != NULL && tab < wTab->count(); tab++)
    {
        if (wTab->tabText(tab) == wName && wTab->tabToolTip(tab) == wPath)
        {
            found = true;
            break;
        }
    }
    //qDebug() << "WDockManager: Found test";
    if (found != true)
    {
        CodeEdit *newEditor = new CodeEdit((QWidget *)(this->parent()), true, wName, wPath, NULL);
        //this->codeEditList.append(newEditor);
        BaseEditor *newBaseEditor;
        if (centralBase == NULL)
        {
            this->deleteCentralWelcome();
            //qDebug() << "WDockManager: Create centralBase";
            newBaseEditor = new BaseEditor(NULL, this, newEditor, false);
            //qDebug() << "WDockManager: base1 height" << newBaseEditor->height();
            //qDebug() << "WDockManager: Created BaseEditor1";
            centralBase = new BaseEditor(splitter, this, newEditor, false);
            //qDebug() << "WDockManager: base2 height" << centralBase->height();
            //qDebug() << "WDockManager: Created BaseEditor2";
            activeCodeEdit = centralBase->getCodeEdit();
            activeCodeEdit->connectAct();
            //activeCodeEdit->connectAct();
            connect(activeCodeEdit,
                    SIGNAL(changedTabStatus(QString, QString, bool)),
                    this,
                    SLOT(changeTabStatusSlot(QString, QString, bool))
                );
            connect(activeCodeEdit,
                    SIGNAL(breakpointEmit(QString, int)),
                    this,
                    SLOT(breakpointEmitSlot(QString, int))
                );
            connect(activeCodeEdit,
                    SIGNAL(breakpointsAddLines(QString, int, int)),
                    this,
                    SLOT(breakpointsAddLinesSlot(QString, int, int))
                );
            connect(activeCodeEdit,
                    SIGNAL(breakpointsRemoveLines(QString, int, int)),
                    this,
                    SLOT(breakpointsRemoveLinesSlot(QString, int, int))
                );
            connect(activeCodeEdit,
                    SIGNAL(bookmarkEmit(QString, int)),
                    this,
                    SLOT(bookmarkEmitSlot(QString, int))
                );
            connect(activeCodeEdit,
                    SIGNAL(bookmarksAddLines(QString, int, int)),
                    this,
                    SLOT(bookmarksAddLinesSlot(QString, int, int))
                );
            connect(activeCodeEdit,
                    SIGNAL(bookmarksRemoveLines(QString, int, int)),
                    this,
                    SLOT(bookmarksRemoveLinesSlot(QString, int, int))
                );
            /*connect(activeCodeEdit,
                    SIGNAL(breakpointListRemove(QString, int)),
                    this,
                    SLOT(breakpointListRemoveSlot(QString, int))
                );*/
            /*QPalette palette = activeCodeEdit->palette();
            palette.setColor(QPalette::Base, QColor::fromRgbF(1, 1, 0, 0.05));
            activeCodeEdit->setPalette(palette);*/
            //qDebug() << "WDockManager: activeCodeEdit assigned";
            splitter->addWidget(centralBase);
            //qDebug() << "WDockManager: splitter height" << splitter->height();
            //qDebug() << "WDockManager: Create splitter";
            if (wName != NULL && wPath != NULL)
            {
                if (this->dockWidgets == true)
                {
                //    emit createDockWidgets();
                    //wMainWindow->CreateDockWidgets();
                //}
                //else
                //{
                    //activeCodeEdit->show();
                    this->showDockWidgetArea(1);
                    this->showDockWidgetArea(2);
                }
            }
            emit centralCreated();
        }
        else
        {
            newBaseEditor = new BaseEditor(NULL, this, newEditor, false);
            //activeCodeEdit->loadCodeEdit(newBaseEditor->getCodeEdit());
            //activeCodeEdit->show();
            //this->showDockWidgetArea(1);
            //this->showDockWidgetArea(2);
        }
        newEditor->changeHeight();
        openCentralWidgets.at(m_currTabBarIndex).second->append(newBaseEditor);
        newBaseEditor->getCodeEdit()->getTextEdit()->setPositionToStart();
        wTab->addTab(wName);
        wTab->tabAdded();
        wTab->setCurrentIndex(wTab->count()-1);
        wTab->setTabToolTip(wTab->currentIndex(), wPath);
        wTab->show();
        newBaseEditor->getCodeEdit()->connectAct();
        //newEditor->connectAct(true);
        /*qDebug() << "WDockManager: wTab - tab added";
        if (true == wTab->isHidden())
        {
            qDebug() << "WDockManager: wTab hidden";
        }*/
        
        //testovaci nazev
        //wTab->setTabText(wTab->currentIndex(), "aa.asm");
        //add tab tooltip with path
        /*connect(newEditor,
                SIGNAL(breakpointListAdd(QString, int)),
                this,
                SLOT(breakpointListAddSlot(QString, int))
               );
        connect(newEditor,
                SIGNAL(breakpointListRemove(QString, int)),
                this,
                SLOT(breakpointListRemoveSlot(QString, int))
               );
        connect(newEditor,
                SIGNAL(changedTabStatus(QString, QString, bool)),
                this,
                SLOT(changeTabStatusSlot(QString, QString, bool))
               );
        connect(activeCodeEdit,
                SIGNAL(changedTabStatus(QString, QString, bool)),
                this,
                SLOT(changeTabStatusSlot(QString, QString, bool))
               );*/
        //connect(newEditor, SIGNAL(updateAnalysers(CodeEdit*)), this, SLOT(updateAnalysersSlot(CodeEdit*)));
    }
    else
    {
        wTab->setCurrentIndex(tab);
        return false;
        //activeCodeEdit->show();
        //this->showDockWidgetArea(1);
        //this->showDockWidgetArea(2);
    }
    return true;
    //qDebug() << "WDockManager: return addCentralWidget()";
}


void WDockManager::addDockWidget(WidgetCode code)
{
    //qDebug() << "WDockManager: addDockWidget()";
    WDock *newWDock;
    if (code == WSIMULATIONINFO)
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
    /*if (wDockBotPrevHeight < newWDock->getQDockWidget()->height())
    {
        wDockBotPrevHeight = newWDock->getQDockWidget()->height();
    }*/
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
    if (false == this->dockWidgets)
    {
        WDock *newWDock = new WDock(this, WSIMULATIONINFO, (QWidget *)(this->parent()), path, simControl);
        if (getDockWidgetArea(newWDock->getArea()) != NULL)
        {
            emit tabifyDockWidget(getDockWidgetArea(newWDock->getArea()), newWDock->getQDockWidget());
            //wMainWindow->tabifyDockWidget(getDockWidgetArea(newWDock->getArea()), newWDock->getQDockWidget());
        }
        /*if (wDockBotPrevHeight < newWDock->getQDockWidget()->height())
        {
            wDockBotPrevHeight = newWDock->getQDockWidget()->widget()->height();
        }*/
        openDockWidgets.append(newWDock);
        //connect(newWDock, SIGNAL(stopSimSig()), this, SLOT(stopSimSlot()));
    }
    else
    {
        PicoBlazeGrid *simWidget = new PicoBlazeGrid(this->getDockWidget(WSIMULATIONINFO),
                                                     simControl
                                                    );
        connect(this, SIGNAL(unhighlightSim()), simWidget, SLOT(unhighlight()));
        simWidget->setProjectPath(path);
        simWidget->fixHeight();
        connect(simWidget, SIGNAL(stopSimSig()), this, SLOT(stopSimSlot()));
        this->openSimWidgets.append(simWidget);
        simWidget->hide();
    }
    //qDebug() << "WDockManager: return addSimDockWidgetP2()";
}


QDockWidget* WDockManager::getDockWidget(WidgetCode code)
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
        if (true == (*i)->cmpArea(area))
        {
            switch (area)
            {
                case 1:
                {
                    /*if (wDockRightPrevWidth < (*i)->getQDockWidget()->width())
                    {
                        //qDebug() << "WDockManager: old height:" << wDockBotPrevHeight;
                        //qDebug() << "WDockManager: new height:" << (*i)->getQDockWidget()->height();
                        wDockRightPrevWidth = (*i)->getQDockWidget()->width();
                    }*/
                    (*i)->getQDockWidget()->setMaximumWidth(0);
                    (*i)->getQDockWidget()->setMinimumWidth(0);
                    break;
                }
                case 2:
                {
                    //if (wDockBotPrevHeight < (*i)->getQDockWidget()->height())
                    //{
                        //qDebug() << "WDockManager: old height:" << wDockBotPrevHeight;
                        //qDebug() << "WDockManager: new height:" << (*i)->getQDockWidget()->height();
                    //    wDockBotPrevHeight = (*i)->getQDockWidget()->height();
                    //}
                    (*i)->getQDockWidget()->setMaximumHeight(0);
                    (*i)->getQDockWidget()->setMinimumHeight(0);
                    break;
                }
            }
            //(*i)->getQDockWidget()->resize((*i)->getQDockWidget()->size().width(), 0);
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
        if (true == (*i)->cmpArea(area))
        {
            //vybira si automaticky nejmensi
            switch(area)
            {
                case 1:
                {
                    if ("Hide" != rightAreaTabs->tabText(rightAreaTabs->currentIndex()))
                    {
                        //(*i)->getQDockWidget()->setFixedWidth(wDockRightPrevWidth);
                        (*i)->getQDockWidget()->setMaximumWidth(500);
                        (*i)->getQDockWidget()->setMinimumWidth(300);
                        wDockRightPrevWidth = 0;
                    }
                    break;
                }
                case 2:
                {
                    if ("Hide" != bottomAreaTabs->tabText(bottomAreaTabs->currentIndex()))
                    {
                        //(*i)->getQDockWidget()->setFixedHeight(wDockBotPrevHeight);
                        (*i)->getQDockWidget()->setMaximumHeight(999);
                        (*i)->getQDockWidget()->setMinimumHeight(280);
                        //(*i)->getQDockWidget()->resize((*i)->getQDockWidget()->size().width(), wDockBotPrevHeight);
                        wDockBotPrevHeight = 0;
                    }
                    break;
                }
            }
        }
    }
    //qDebug() << "WDockManager: wDockBotPrevHeight" << wDockBotPrevHeight;
    //qDebug() << "WDockManager: return showDockWidgetArea()";
}


bool WDockManager::isEmpty()
{
    return openCentralWidgets.at(m_currTabBarIndex).second->empty();
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


/*void WDockManager::updateAnalysersSlot(CodeEdit *editor)
{
    //qDebug() << "WDockManager: updateAnalysersSlot()";
    if (this->getDockWidget(wAnalysVar) != NULL && this->getDockWidget(wAnalysFunc) != NULL)
    {
        Analys *analyser = new Analys(*(editor->getTextEdit()->document()));
        ((AnalyserWidget*)(this->getDockWidget(wAnalysVar)->widget()))->fill(analyser->getVar());
        ((AnalyserWidget*)(this->getDockWidget(wAnalysFunc)->widget()))->fill(analyser->getFunc());
    }
    //qDebug() << "WDockManager: return updateAnalysersSlot()";
}*/


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
    //codeEditList.swap(from, to);
    openCentralWidgets.at(m_currTabBarIndex).second->swap(from, to);
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
            if (this->wTab->tabText(i) == fileName)
            {
                //qDebug() << "WDockManager: CodeEdit found";
                if (this->wTab->currentIndex() != i)
                {
                    this->wTab->setCurrentIndex(i);
                    //this->changeCodeEditor(i);
                }
                return;
            }
        }
    }
    //qDebug() << "WDockManager: return setCentralByName()";
}


bool WDockManager::setCentralByPath(QString filePath)
{
    //qDebug() << "WDockManager: setCentralByName()";
    if (filePath != "")
    {
        for (int i = 0; i < this->getTabCount(); i++)
        {
            //qDebug() << "WDockManager: tabtooltip" << wTab->tabToolTip(i) << "filePath" << filePath;
            if (this->wTab->tabToolTip(i) == filePath)
            {
                //qDebug() << "WDockManager: CodeEdit found";
                if (this->wTab->currentIndex() != i)
                {
                    this->wTab->setCurrentIndex(i);
                    //this->changeCodeEditor(i);
                }
                return true;
            }
        }
    }
    return false;
    //qDebug() << "WDockManager: return setCentralByName()";
}


CodeEdit* WDockManager::getCentralByPath(QString filePath)
{
    //qDebug() << "WDockManager: setCentralByName()";
    if (filePath != "")
    {
        for (int i = 0; i < this->getTabCount(); i++)
        {
            //qDebug() << "WDockManager: tabtooltip" << wTab->tabToolTip(i) << "filePath" << filePath;
            if (this->wTab->tabToolTip(i) == filePath)
            {
                //qDebug() << "WDockManager: CodeEdit found";
                return openCentralWidgets.at(m_currTabBarIndex).second->at(this->wTab->currentIndex())->getCodeEdit();
            }
        }
    }
    //qDebug() << "WDockManager: return setCentralByName()";
    return NULL;
}


void WDockManager::setCentralByIndex(int index)
{
    this->wTab->setCurrentIndex(index);
}


void WDockManager::setEditorsReadOnly(bool readonly)
{
    //for (int i = 0; i < codeEditList.count(); i++)
    //{
        activeCodeEdit->getTextEdit()->queryReadOnly(readonly);
    //}
}


void WDockManager::addDockW(Qt::DockWidgetArea area, QDockWidget* dockWidget)
{
    emit addDockWidget(area, dockWidget);
}


void WDockManager::unhighlightSimWidget()
{
    emit unhighlightSim();
}


void WDockManager::highlightError(QString filename, int line)
{
    //qDebug() << "Line" << QDir(filename).absolutePath() << "orig" << filename;
    this->setCentralByPath(QDir(filename).absolutePath());
    this->getCentralTextEdit()->scrollToLine(line);
    this->getCentralTextEdit()->selectLine(line);
}


void WDockManager::handleShowHideBottom(int index)
{
    //qDebug() << "WDockManager: bottom changed to" << bottomAreaTabs->tabText(index);
    if (NULL == activeCodeEdit)
    {
        return;
    }
    if ("Hide" == bottomAreaTabs->tabText(index))
    {
        if (true == bottomVisible)
        {
            hideDockWidgetArea(2);
            bottomVisible = false;
            QApplication::processEvents();
            activeCodeEdit->changeHeight();
            //QTimer::singleShot(50, activeCodeEdit, SLOT(changeHeight()));
        }
    }
    else if (false == bottomVisible)
    {
        showDockWidgetArea(2);
        bottomVisible = true;
        QApplication::processEvents();
        activeCodeEdit->changeHeight();
        //QTimer::singleShot(50, activeCodeEdit, SLOT(changeHeight()));
    }
}


/*void WDockManager::handleShowHideLeft(int index)
{
    qDebug() << "WDockManager: left changed to" << leftAreaTabs->tabText(index);
    if ("Hide" == leftAreaTabs->tabText(index))
    {
        if (true == visible)
        {
            hideDockWidgetArea(2);
            visible = false;
        }
    }
    else if (false == visible)
    {
        showDockWidgetArea(2);
        visible = true;
    }
}*/


void WDockManager::handleShowHideRight(int index)
{
    //qDebug() << "WDockManager: right changed to" << rightAreaTabs->tabText(index);
    if (NULL == activeCodeEdit)
    {
        return;
    }
    if ("Hide" == rightAreaTabs->tabText(index))
    {
        if (true == rightVisible)
        {
            hideDockWidgetArea(1);
            rightVisible = false;
        }
    }
    else if (false == rightVisible)
    {
        showDockWidgetArea(1);
        rightVisible = true;
    }
}


void WDockManager::stopSimSlot()
{
    //qDebug() << "WDockManager: stopsimslot";
    emit stopSimSig();
}


void WDockManager::clockChangedSlot(double clock, int clockMult)
{
    emit clockChangedSig(clock, clockMult);
}

void WDockManager::changeSimWidget(int index)
{
    //qDebug() << "WDockManager: changeSimWidget index:" << index;
    //qDebug() << "WDockManager: openSimWidgets size:" <<openSimWidgets.size();
    if (this->getDockWidget(WSIMULATIONINFO) == NULL)
    {
        qDebug() << "Simulation Dock Widget is null, should never happen";
        return;
    }
    if (index >= openSimWidgets.size())
    {
        this->getDockWidget(WSIMULATIONINFO)->setWidget(this->openSimWidgets.at(openSimWidgets.size()-1));
    }
    else
    {
        this->getDockWidget(WSIMULATIONINFO)->setWidget(this->openSimWidgets.at(index));
    }
    //qDebug() << "WDockManager: changeSimWidget done";
}


void WDockManager::deleteActiveSimWidget()
{
    //qDebug() << "WDockManager: deleteActiveSimWidget";
    PicoBlazeGrid *tempGrid = (PicoBlazeGrid*)(this->getDockWidget(WSIMULATIONINFO)->widget());
    int index = openSimWidgets.indexOf(tempGrid);
    if (openSimWidgets.count() > 1)
    {
        this->openSimWidgets.removeAt(index);
        if (index == this->openSimWidgets.count())
        {
            this->getDockWidget(WSIMULATIONINFO)->setWidget(this->openSimWidgets.at(index -1 ));
        }
        else
        {
            this->getDockWidget(WSIMULATIONINFO)->setWidget(this->openSimWidgets.at(index));
        }
        delete tempGrid;
        tempGrid = NULL;
    }
    else
    {
        this->openSimWidgets.removeAt(index);
        while (this->openDockWidgets.count() > 0)
        {
            WDock *tmpDock = this->openDockWidgets.at(0);
            this->openDockWidgets.removeAt(0);
            delete tmpDock->getQDockWidget();
            delete tmpDock;
        }
        this->breakpointList = NULL;
        this->bookmarkList = NULL;
        this->bottomAreaTabs = NULL;
        this->rightAreaTabs = NULL;
        this->dockWidgets = false;
        //delete tempGrid;
        //tempGrid = NULL;
    }
    //qDebug() << "WDockManager: deleteActiveSimWidget done";
}


void WDockManager::closeFile(QString path, bool openUntitled)
{
    //qDebug() << "WDockManager: path " << path;
    for (int i = 0; i < this->wTab->count(); i++)
    {
        //qDebug() << "WDockManager: tooltip (" << i << ") " << this->wTab->tabToolTip(i);
        if (path == this->wTab->tabToolTip(i))
        {
            this->closeTab(i, openUntitled);
            break;
        }
    }
}


void WDockManager::breakpointEmitSlot(QString file, int line)
{
    emit breakpointEmit(file, line);
}


void WDockManager::breakpointsAddLinesSlot(QString file, int line, int linesAdded)
{
    //qDebug() << "WDockManager: breakpointsAddLines";
    emit breakpointsAddLines(file, line, linesAdded);
}


void WDockManager::breakpointsRemoveLinesSlot(QString file, int line, int linesRemoved)
{
    emit breakpointsRemoveLines(file, line, linesRemoved);
}


void WDockManager::bookmarkEmitSlot(QString file, int line)
{
    emit bookmarkEmit(file, line);
}


void WDockManager::bookmarksAddLinesSlot(QString file, int line, int linesAdded)
{
    //qDebug() << "WDockManager: bookmarksAddLines";
    emit bookmarksAddLines(file, line, linesAdded);
}


void WDockManager::bookmarksRemoveLinesSlot(QString file, int line, int linesRemoved)
{
    emit bookmarksRemoveLines(file, line, linesRemoved);
}


/*void WDockManager::breakpointListRemoveSlot(QString file, int line)
{
    emit breakpointListRemove(file, line);
}*/


void WDockManager::setCentralWelcome()
{
    //qDebug() << "WDockManager: setCentralWelcome()";
    /*if (openCentralWidgets.count() == 0)
    {
        delete centralBase;
        centralBase = NULL;
        activeCodeEdit = NULL;
        this->hideDockWidgetArea(1);
        this->hideDockWidgetArea(2);
        emit centralClosed();
    }*/
    //qDebug() << this->splitter->count();
    if (this->splitter->count() == 0)
    {
        this->welcomeScr = new WelcomeScr(this->splitter);
        this->splitter->addWidget(this->welcomeScr);
        connect(this->welcomeScr,
                SIGNAL(example()),
                this,
                SLOT(welcomeScrExample())
               );
        connect(this->welcomeScr,
                SIGNAL(openProject()),
                this,
                SLOT(welcomeScrOpenPrj())
               );
        connect(this->welcomeScr,
                SIGNAL(newProject()),
                this,
                SLOT(welcomeScrNewPrj())
               );
        connect(this->welcomeScr,
                SIGNAL(manual()),
                this,
                SLOT(welcomeScrManual())
               );
        connect(this->welcomeScr,
                SIGNAL(quickGuide()),
                this,
                SLOT(welcomeScrGuide())
               );
        connect(this->welcomeScr,
                SIGNAL(recentProject(QString)),
                this,
                SLOT(welcomeScrRecent(QString))
               );
    }
    //qDebug() << "WDockManager: return setCentralWelcome()";
}


void WDockManager::compilationFinishedSlot(bool success)
{
}


void WDockManager::welcomeScrExample()
{
   emit welcomeScrExampleSig();
}


void WDockManager::welcomeScrOpenPrj()
{
    emit welcomeScrOpenPrjSig();
}


void WDockManager::welcomeScrNewPrj()
{
    emit welcomeScrNewPrjSig();
}


void WDockManager::welcomeScrManual()
{
    emit welcomeScrManualSig();
}


void WDockManager::welcomeScrGuide()
{
    emit welcomeScrGuideSig();
}


void WDockManager::welcomeScrRecent(QString path)
{
    emit welcomeScrRecentSig(path);
}


void WDockManager::deleteCentralWelcome()
{
    //qDebug() << "WDockManager: deleteCentralWelcome();";
    if (this->welcomeScr != NULL)
    {
        this->welcomeScr->disconnect();
        this->welcomeScr->deleteLater();
        //delete this->welcomeScr;
        this->welcomeScr = NULL;
    }
    //qDebug() << "WDockManager: return deleteCentralWelcome();";
}


bool WDockManager::getTabStatusChanged(int index)
{
    return wTab->tabStatus(index);
}


void WDockManager::showProjectEditors(QString projectPath)
{
    for (int i = 0; i < openCentralWidgets.count(); i++)
    {
        //qDebug() << openCentralWidgets.at(i).first->id;
        if (projectPath == openCentralWidgets.at(i).first->id)
        {
            if (m_currTabBarIndex == i)
            {
                return;
            }
            qDebug() << "WDockManager: tabs checked";
            if (wTab != NULL)
            {
                wTab->hide();
            }
            qDebug() << "WDockManager: tabs checked 2";
            wTab = openCentralWidgets.at(i).first;
            wTab->show();
            m_currTabBarIndex = i;
            if (NULL != activeCodeEdit)
            {
                changeCodeEditor(wTab->currentIndex());
                //qDebug() << "WDockManager: checking tabs - return";
            }
            break;
        }
    }
}


void WDockManager::appendTabBar(QString projectPath)
{
    TabBar *tabBar = new TabBar(m_centralWidget);
    tabBar->id = projectPath;
    QList<BaseEditor*> *editList = new QList<BaseEditor*>();
    openCentralWidgets.append(qMakePair(tabBar, editList));
    m_layout->insertWidget(0, tabBar);
    if (NULL == wTab)
    {
        wTab = tabBar;
        wTab->show();
        m_currTabBarIndex = 0;
    }
    else
    {
        tabBar->hide();
    }
    connect(tabBar, SIGNAL(tabCloseRequested(int)),this, SLOT(closeTab(int)));
    connect(tabBar, SIGNAL(currentChanged(int)), this, SLOT(changeCodeEditor(int)));
    connect(tabBar, SIGNAL(tabMoved(int, int)), this, SLOT(moveEditorsSlot(int, int)));

}


void WDockManager::removeTabBar(QString projectPath)
{
    for (int i = 0; i < openCentralWidgets.count(); i++)
    {
        if (projectPath == openCentralWidgets.at(i).first->id)
        {
            TabBar *tab = openCentralWidgets.at(i).first;
            openCentralWidgets.removeAt(i);
            delete tab;
            wTab = NULL;
            break;
        }
    }
    if (openCentralWidgets.count() == 0)
    {
        delete centralBase;
        centralBase = NULL;
        activeCodeEdit = NULL;
        this->hideDockWidgetArea(1);
        this->hideDockWidgetArea(2);
        emit centralClosed();
    }
    /*if (openCentralWidgets.count() > 0)
    {
        wTab = openCentralWidgets.at(openCentralWidgets.count()-1).first;
        wTab->show();
        m_currTabBarIndex = openCentralWidgets.count()-1;
    }
    else
    {*/
    /*    wTab = NULL;
        if (NULL != activeCodeEdit)
        {
            
        }*/
    //}
}


void WDockManager::setBottomAreaToCompilerInfo()
{
    if (false == bottomVisible)
    {
        showDockWidgetArea(2);
    }
    for (int i = 0; i < bottomAreaTabs->count(); i++)
    {
        if ("Compiler Messages" == bottomAreaTabs->tabText(i))
        {
            bottomAreaTabs->setCurrentIndex(i);
            break;
        }
    }
}


void WDockManager::setBottomAreaToExtAppOutput()
{
    if (false == bottomVisible)
    {
        showDockWidgetArea(2);
    }
    for (int i = 0; i < bottomAreaTabs->count(); i++)
    {
        if ("External Applications" == bottomAreaTabs->tabText(i))
        {
            bottomAreaTabs->setCurrentIndex(i);
            break;
        }
    }
}


void WDockManager::setBottomAreaToSimulationInfo()
{
    if (false == bottomVisible)
    {
        showDockWidgetArea(2);
    }
    for (int i = 0; i < bottomAreaTabs->count(); i++)
    {
        if ("Simulator" == bottomAreaTabs->tabText(i))
        {
            bottomAreaTabs->setCurrentIndex(i);
            break;
        }
    }
}


void WDockManager::changeTab(bool next)
{
    if (true == next)
    {
        if (wTab->currentIndex()+1 < wTab->count())
        {
            wTab->setCurrentIndex(wTab->currentIndex()+1);
        }
    }
    else
    {
        if (wTab->currentIndex()-1 >= 0)
        {
            wTab->setCurrentIndex(wTab->currentIndex()-1);
        }
    }
}


void WDockManager::closeCurrentTab()
{
    if (wTab->count() > 0)
    {
        this->closeTab(wTab->currentIndex());
    }
}




/////
///// WDock
/////

WDock::WDock(WDockManager *parent, WidgetCode code, QWidget *parentWindow)
    : QObject(parentWindow)
{
    //qDebug() << "WDock: WDock()";
    switch (code)
    {
        case WBOOKMARKLIST:
        {
            wDockWidget = new QDockWidget("Bookmarks", parentWindow);
            wDockWidget->setAllowedAreas(Qt::RightDockWidgetArea);
            wDockWidget->setFeatures(QDockWidget::NoDockWidgetFeatures);
            parent->addDockW(Qt::RightDockWidgetArea, wDockWidget);
            //mainWindow->addDockWidget(Qt::RightDockWidgetArea, wDockWidget);
            parent->createBookmarkList(wDockWidget);
            area = 1;
            wDockWidget->setWidget(parent->getBookmarkList());
	        break;
        }
        case WBREAKPOINTLIST:
        {
            wDockWidget = new QDockWidget("Breakpoints", parentWindow);
            wDockWidget->setAllowedAreas(Qt::RightDockWidgetArea);
            wDockWidget->setFeatures(QDockWidget::NoDockWidgetFeatures);
            parent->addDockW(Qt::RightDockWidgetArea, wDockWidget);
            //mainWindow->addDockWidget(Qt::RightDockWidgetArea, wDockWidget);
            parent->createBreakpointList(wDockWidget);
            area = 1;
            wDockWidget->setWidget(parent->getBreakpointList());
	        break;
        }
        case WCOMPILEINFO:
        {
            wDockWidget = new QDockWidget("Compiler Messages", parentWindow);
            wDockWidget->setAllowedAreas(Qt::BottomDockWidgetArea);
            wDockWidget->setFeatures(QDockWidget::NoDockWidgetFeatures);
            parent->addDockW(Qt::BottomDockWidgetArea, wDockWidget);
            //mainWindow->addDockWidget(Qt::BottomDockWidgetArea, wDockWidget);
            //QPlainTextEdit *newWidget = new QPlainTextEdit(wDockWidget);
            CompileInfo *newWidget = new CompileInfo(wDockWidget);
            newWidget->setFont(QFont("UbuntuMono", 10));
            area = 2;
            newWidget->setReadOnly(true);
            wDockWidget->setWidget(newWidget);
            //parent->connect(wDockWidget, SIGNAL(visibilityChanged(bool)), parent, SLOT(showBottomArea(bool)));
            parent->connect(newWidget, SIGNAL(errorClicked(QString, int)), parent, SLOT(highlightError(QString, int)));
	        break;
        }
        case WSIMULATIONINFO:
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
        /*case wAnalysVar:
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
        }*/
        case WBOTTOMHIDE:
        {
            wDockWidget = new QDockWidget("Hide", parentWindow);
            wDockWidget->setAllowedAreas(Qt::BottomDockWidgetArea);
            wDockWidget->setFeatures(QDockWidget::NoDockWidgetFeatures);
            parent->addDockW(Qt::BottomDockWidgetArea, wDockWidget);
            //ShowHideWidget *newDock = new ShowHideWidget(wDockWidget);
            QWidget *newDock = new QWidget(wDockWidget);
            area = 2;
            wDockWidget->setWidget(newDock);
            //QPushButton *pshTitle = new QPushButton("Hide", parentWindow);
            //wDockWidget->setTitleBarWidget(pshTitle);
            //parent->connect(wDockWidget, SIGNAL(visibilityChanged(bool)), parent, SLOT(hideBottomArea(bool)));
            //parent->connect(pshTitle, SIGNAL(clicked()), parent, SLOT(showBottomArea()));
            break;
        }
        case WRIGHTHIDE:
        {
            wDockWidget = new QDockWidget("Hide", parentWindow);
            wDockWidget->setAllowedAreas(Qt::RightDockWidgetArea);
            wDockWidget->setFeatures(QDockWidget::NoDockWidgetFeatures);
            parent->addDockW(Qt::RightDockWidgetArea, wDockWidget);
            //ShowHideWidget *newDock = new ShowHideWidget(wDockWidget);
            QWidget *newDock = new QWidget(wDockWidget);
            area = 1;
            wDockWidget->setWidget(newDock);
            //QPushButton *pshTitle = new QPushButton("Hide", parentWindow);
            //wDockWidget->setTitleBarWidget(pshTitle);
            //parent->connect(wDockWidget, SIGNAL(visibilityChanged(bool)), parent, SLOT(hideBottomArea(bool)));
            //parent->connect(pshTitle, SIGNAL(clicked()), parent, SLOT(showBottomArea()));
            break;
        }
        case WASMMACROANALYSER:
        {
            wDockWidget = new QDockWidget("Macros", parentWindow);
            wDockWidget->setAllowedAreas(Qt::RightDockWidgetArea);
            wDockWidget->setFeatures(QDockWidget::NoDockWidgetFeatures);
            parent->addDockW(Qt::RightDockWidgetArea, wDockWidget);
            AsmMacroAnalyser *newDock = new AsmMacroAnalyser(wDockWidget);
            area = 1;
            wDockWidget->setWidget(newDock); 
            break;
        }
        case WEXTAPPOUTPUT:
        {
            wDockWidget = new QDockWidget("External Applications", parentWindow);
            wDockWidget->setAllowedAreas(Qt::BottomDockWidgetArea);
            wDockWidget->setFeatures(QDockWidget::NoDockWidgetFeatures);
            parent->addDockW(Qt::BottomDockWidgetArea, wDockWidget);
            ExtAppOutput *newDock = new ExtAppOutput(wDockWidget);
            area = 2;
            wDockWidget->setWidget(newDock);
            break;
        }
        case WHELPDOCKWIDGET:
        {
            wDockWidget = new QDockWidget("Help", parentWindow);
            wDockWidget->setAllowedAreas(Qt::RightDockWidgetArea);
            wDockWidget->setFeatures(QDockWidget::NoDockWidgetFeatures);
            parent->addDockW(Qt::RightDockWidgetArea, wDockWidget);
            HelpDockWidget *newDock = new HelpDockWidget(wDockWidget);
            area = 1;
            wDockWidget->setWidget(newDock); 
            break;
        }
        default:
        {
            qDebug() << "WDockManager: unknown widget code";
        }
    }
    this->code=code;
    //qDebug() << "WDock: return WDock()";
}



WDock::WDock(WDockManager *parent, WidgetCode code, QWidget *parentWindow, QString path, MCUSimControl* simControl)
    : QObject(parentWindow)
{
    switch (code)
    {
        case WSIMULATIONINFO:
        {
            wDockWidget = new QDockWidget("Simulator", parentWindow);
            wDockWidget->setAllowedAreas(Qt::BottomDockWidgetArea);
            parent->addDockW(Qt::BottomDockWidgetArea, wDockWidget);
            wDockWidget->setFeatures(QDockWidget::NoDockWidgetFeatures);
            //mainWindow->addDockWidget(Qt::BottomDockWidgetArea, wDockWidget);
            PicoBlazeGrid *newWidget = new PicoBlazeGrid(wDockWidget, simControl);
            connect(parent, SIGNAL(unhighlightSim()), newWidget, SLOT(unhighlight()));
            newWidget->setProjectPath(path);
            area = 2;
            wDockWidget->setWidget(newWidget);
            wDockWidget->show();
            newWidget->fixHeight();
            connect(newWidget, SIGNAL(stopSimSig()), parent, SLOT(stopSimSlot()));
            connect(newWidget, SIGNAL(clockChanged(double, int)), parent, SLOT(clockChangedSlot(double, int)));
            parent->openSimWidgets.append(newWidget);
            //qDebug() << "WSimulationInfo: height fixed";
            //parent->connect(wDockWidget, SIGNAL(visibilityChanged(bool)), parent, SLOT(showBottomArea(bool)));
            break;
        }
        default:
        {
            qDebug() << "WDockManager: unknown widget code";
        }
    }
    this->code = code;
}



WDock::~WDock()
{
    /*if (wDockWidget != NULL)
    {
        delete wDockWidget;
    }*/
}



QDockWidget* WDock::getQDockWidget()
{
    return wDockWidget;
}



bool WDock::cmpCode(WidgetCode code)
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
