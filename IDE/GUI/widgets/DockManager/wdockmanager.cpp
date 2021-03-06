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
#include "../../mds.h"
#include "wdockmanager.h"
#include "../Editor/wtextedit.h"
#include "../WelcomeScr/welcomescr.h"
#include "../Editor/codeedit.h"
#include "../Editor/baseeditor.h"
#include "../TabBar/tabbar.h"
#include "../HelpWidget/helpbrowser.h"





/**
 * @brief Constructor. Inits docking and tabbar.
 * @param mainWindow Parent window.
 */
WDockManager::WDockManager(QWidget *parent, QWidget *centralWidget)
    : QObject(parent)
{
    //qDebug() << "WDockManager: WDockManager()";
    //wMainWindow = mainWindow;
    m_parent = parent;
    m_layout = new QVBoxLayout(centralWidget);
    m_centralWidget = centralWidget;
    //wTab = new TabBar(centralWidget);
    wTab = NULL;
    splitter = new QSplitter(centralWidget);
    splitter->show();
    activeHelpBrowser = new HelpBrowser(splitter);
    activeHelpBrowser->hide();
    //wTab->setTabsClosable(true);
    //wTab->setMovable(true);
//     wRight = NULL;
//     wLeft = NULL;
//     wBottom = NULL;

//     m_tabBottom = NULL;

//         m_tabRight = new QTabWidget(parent);
//         QDockWidget *wDockWidget = new QDockWidget("", parent);
//         wDockWidget->setAllowedAreas(Qt::BottomDockWidgetArea);
//         wDockWidget->setFeatures(QDockWidget::NoDockWidgetFeatures);
//         wDockWidget->setWidget(m_tabRight);
//         emit addDockWidget(Qt::BottomDockWidgetArea, wDockWidget);
    
    //tabLeft = NULL;
//     m_tabRight = NULL;
    
//     bookmarkList = NULL;
//     breakpointList = NULL;
    activeCodeEdit = NULL;
    welcomeScr = NULL;
    centralBase = NULL;
//     this->dockWidgets = false;
//     wDockBotPrevHeight = 0;
//     wDockRightPrevWidth = 0;
//     bottomVisible = true;
//     rightVisible = true;

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
    if ("Help Browser" == wTab->tabToolTip(index))
    {
        if (activeCodeEdit != NULL)
        {
            activeCodeEdit->hide();
        }
        activeHelpBrowser->show();
        activeHelpBrowser->setSource(m_helpBrowserUrl);
        emit tabChangedToDisabled(true);
    }
    else if (activeCodeEdit != NULL && index >= 0)
    {
        if (true == activeHelpBrowser->isVisible())
        {
            activeHelpBrowser->hide();
        }
        activeCodeEdit->show();
        //qDebug() << "wdockmanager - change Code Editor";
        //qDebug() << "index: " << index;
        //qDebug() << "size: " << openCentralWidgets.count();
        //CodeEdit *editor = openCentralWidgets.at(index)->getCodeEdit();
        activeCodeEdit->loadCodeEdit(openCentralWidgets.at(m_currTabBarIndex).second->at(index)->getCodeEdit());
        QTimer::singleShot(50, activeCodeEdit, SLOT(changeHeight()));
        emit tabChangedToDisabled(false);
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
    if ("Help Browser" == wTab->tabText(index))
    {
        activeHelpBrowser->hide();
    }
    else
    {
        if (openCentralWidgets.at(m_currTabBarIndex).second->at(wTab->currentIndex()) != NULL)
        {
            emit saveCodeEdit(openCentralWidgets.at(m_currTabBarIndex).second->at(wTab->currentIndex())->getCodeEdit(), true);
        }
    }
    emit tabClosed(wTab->tabToolTip(index));
    openCentralWidgets.at(m_currTabBarIndex).second->removeAt(index);
    wTab->removeTab(index);
    //wTab->tabRemoved(index);
    if (true == openUntitled)
    {
        if (wTab->currentIndex() < 0)
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
        else if (wTab->count() == 1 && wTab->tabToolTip(0) == "Help Browser")
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
        }
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
    if (NULL != openCentralWidgets.at(m_currTabBarIndex).second->at(index))
    {
        return openCentralWidgets.at(m_currTabBarIndex).second->at(index)->getCodeEdit()->getTextEdit();
    }
    else
    {
        return NULL;
    }
}



CodeEdit* WDockManager::getCentralWidget()
{
    return activeCodeEdit;
}


QString WDockManager::getTabToolTip(int index)
{
    if (wTab != NULL && index < wTab->count())
    {
        return wTab->tabToolTip(index);
    }
    return QString();
}



CodeEdit* WDockManager::getTabWidget(int index)
{
    if (NULL != openCentralWidgets.at(m_currTabBarIndex).second->at(index))
    {
        return openCentralWidgets.at(m_currTabBarIndex).second->at(index)->getCodeEdit();
    }
    else
    {
        return NULL;
    }
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
    if (NULL == openCentralWidgets.at(m_currTabBarIndex).second->at(wTab->currentIndex()))
    {
        return QString();
    }
    else
    {
        return openCentralWidgets.at(m_currTabBarIndex).second->at(wTab->currentIndex())->getCodeEdit()->getName();
    }
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
    if (NULL == openCentralWidgets.at(m_currTabBarIndex).second->at(wTab->currentIndex()))
    {
        return QString();
    }
    else
    {
        return openCentralWidgets.at(m_currTabBarIndex).second->at(wTab->currentIndex())->getCodeEdit()->getPath();
    }
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
//             if (this->dockWidgets == true)
//             {
                //emit createDockWidgets();
                //wMainWindow->CreateDockWidgets();
            //}
            //else
            //{
                //activeCodeEdit->show();
//                 this->showDockWidgetArea(1);
//                 this->showDockWidgetArea(2);
//             }
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
    for (; wTab != NULL && tab < wTab->count(); tab++)
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
//             if (this->dockWidgets == true)
//             {
            //    emit createDockWidgets();
                //wMainWindow->CreateDockWidgets();
            //}
            //else
            //{
                //activeCodeEdit->show();
//                 this->showDockWidgetArea(1);
//                 this->showDockWidgetArea(2);
//             }
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
//                 if (this->dockWidgets == true)
//                 {
//                 //    emit createDockWidgets();
//                     //wMainWindow->CreateDockWidgets();
//                 //}
//                 //else
//                 //{
//                     //activeCodeEdit->show();
//                     this->showDockWidgetArea(1);
//                     this->showDockWidgetArea(2);
//                 }
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


// void WDockManager::addDockWidget(WidgetCode code, MCUSimControl* simControl)
// {
//     qDebug() << "WDockManager: addDockWidget()";
//     if (NULL == m_tabBottom)
//     {
//         m_tabBottom = new QTabWidget(m_parent);
//         QDockWidget *wDockWidget = new QDockWidget("", m_parent);
//         wDockWidget->setAllowedAreas(Qt::BottomDockWidgetArea);
//         wDockWidget->setFeatures(QDockWidget::NoDockWidgetFeatures);
//         wDockWidget->setWidget(m_tabBottom);
//         emit addDockWidget(Qt::BottomDockWidgetArea, wDockWidget);
//     }
//     if (NULL == m_tabRight)
//     {
//         m_tabRight = new QTabWidget(m_parent);
//         QDockWidget *wDockWidget = new QDockWidget("", m_parent);
//         wDockWidget->setAllowedAreas(Qt::BottomDockWidgetArea);
//         wDockWidget->setFeatures(QDockWidget::NoDockWidgetFeatures);
//         wDockWidget->setWidget(m_tabRight);
//         emit addDockWidget(Qt::RightDockWidgetArea, wDockWidget);
//     }
//     WDock *newWDock;
//     switch (code)
//     {
//         case WSIMULATIONINFO:
//         {
//             this->addSimDockWidget(simControl);
//             break;
//         }
//         case WCALLWATCHER:
//         {
//             this->addCallWatcher(simControl);
//             break;
//         }
//         case WREGWATCHER:
//         {
//             this->addRegWatcher(simControl);
//             break;
//         }
//         case WEXTAPPOUTPUT:
//         {
//         }
//         case WCOMPILEINFO:
//         {
//         }
//         case WBOTTOMHIDE:
//         {
//             newWDock = new WDock(this, code, m_tabBottom);
//             openDockWidgets.append(newWDock);
//             break;
//         }
//         case WBOOKMARKLIST:
//         {
//         }
//         case WBREAKPOINTLIST:
//         {
//         }
//         case WASMMACROANALYSER:
//         {
//         }
//         case WHELPDOCKWIDGET:
//         {
//         }
//         case WRIGHTHIDE:
//         {
//             newWDock = new WDock(this, code, m_tabRight);
//             openDockWidgets.append(newWDock);
//             break;
//         }
//     }
//     
//     /*if (getDockWidgetArea(newWDock->getArea()) != NULL)
//     {
//         emit tabifyDockWidget(getDockWidgetArea(newWDock->getArea()), newWDock->getQDockWidget());
//         //wMainWindow->tabifyDockWidget(getDockWidgetArea(newWDock->getArea()), newWDock->getQDockWidget());
//     }*/
//     /*if (wDockBotPrevHeight < newWDock->getQDockWidget()->height())
//     {
//         wDockBotPrevHeight = newWDock->getQDockWidget()->height();
//     }*/
//     //qDebug() << "WDockManager: return addDockWidget()";
// }


// void WDockManager::addSimDockWidget(MCUSimControl* simControl)
// {
//     //qDebug() << "WDockManager: addSimDockWidgetP1()";
//     //qDebug() << "WDockManager: return addSimDockWidgetP1()";
//     if (false == this->dockWidgets)
//     {
//         WDock *newWDock = new WDock(this, WSIMULATIONINFO, m_tabBottom, simControl);
//         /*if (getDockWidgetArea(newWDock->getArea()) != NULL)
//         {
//             emit tabifyDockWidget(getDockWidgetArea(newWDock->getArea()), newWDock->getQDockWidget());
//             //wMainWindow->tabifyDockWidget(getDockWidgetArea(newWDock->getArea()), newWDock->getQDockWidget());
//         }*/
//         /*if (wDockBotPrevHeight < newWDock->getQDockWidget()->height())
//         {
//             wDockBotPrevHeight = newWDock->getQDockWidget()->widget()->height();
//         }*/
//         openDockWidgets.append(newWDock);
//         //connect(newWDock, SIGNAL(stopSimSig()), this, SLOT(stopSimSlot()));
//     }
//     else
//     {
//         PicoBlazeGrid *simWidget = new PicoBlazeGrid(m_tabBottom, simControl);
//         connect(this, SIGNAL(unhighlightSim()), simWidget, SLOT(unhighlight()));
//         //simWidget->setProjectPath(path);
//         simWidget->fixHeight();
//         connect(simWidget, SIGNAL(stopSimSig()), this, SLOT(stopSimSlot()));
//         this->openSimWidgets.append(simWidget);
//         simWidget->hide();
//     }
//     emit getSimProjectData();
// }


// void WDockManager::addRegWatcher(MCUSimControl *simControl)
// {
//     #ifdef MDS_FEATURE_SIM_REGWATCHER
//         if (false == this->dockWidgets)
//         {
//             WDock *newWDock = new WDock(this, WREGWATCHER, m_tabRight, simControl);
//             /*if (getDockWidgetArea(newWDock->getArea()) != NULL)
//             {
//                 emit tabifyDockWidget(getDockWidgetArea(newWDock->getArea()), newWDock->getQDockWidget());
//                 //wMainWindow->tabifyDockWidget(getDockWidgetArea(newWDock->getArea()), newWDock->getQDockWidget());
//             }*/
//             /*if (wDockBotPrevHeight < newWDock->getQDockWidget()->height())
//             {
//                 wDockBotPrevHeight = newWDock->getQDockWidget()->widget()->height();
//             }*/
//             openDockWidgets.append(newWDock);
//             //connect(newWDock, SIGNAL(stopSimSig()), this, SLOT(stopSimSlot()));
//         }
//         else
//         {
//             RegWatcher *regWatcher = new RegWatcher(m_tabRight, simControl);
//             //connect(this, SIGNAL(unhighlightSim()), simWidget, SLOT(unhighlight()));
//             //simWidget->fixHeight();
//             //connect(simWidget, SIGNAL(stopSimSig()), this, SLOT(stopSimSlot()));
//             this->openRegWatchers.append(regWatcher);
//             regWatcher->hide();
//         }
//     #endif
// }


// void WDockManager::addCallWatcher(MCUSimControl *simControl)
// {
//     #ifdef MDS_FEATURE_SIM_CALLWATCHER
//         //qDebug() << "add call watcher";
//         if (false == this->dockWidgets)
//         {
//             WDock *newWDock = new WDock(this, WCALLWATCHER, m_tabRight, simControl);
//             /*if (getDockWidgetArea(newWDock->getArea()) != NULL)
//             {
//                 qDebug() << "add call watcher 2";
//                 emit tabifyDockWidget(getDockWidgetArea(newWDock->getArea()), newWDock->getQDockWidget());
//                 //wMainWindow->tabifyDockWidget(getDockWidgetArea(newWDock->getArea()), newWDock->getQDockWidget());
//             }*/
//             /*if (wDockBotPrevHeight < newWDock->getQDockWidget()->height())
//             {
//                 wDockBotPrevHeight = newWDock->getQDockWidget()->widget()->height();
//             }*/
//             openDockWidgets.append(newWDock);
//             //connect(newWDock, SIGNAL(stopSimSig()), this, SLOT(stopSimSlot()));
//         }
//         else
//         {
//             CallWatcher *callWatcher = new CallWatcher(m_tabRight, simControl);
//             //connect(this, SIGNAL(unhighlightSim()), simWidget, SLOT(unhighlight()));
//             //simWidget->fixHeight();
//             //connect(simWidget, SIGNAL(stopSimSig()), this, SLOT(stopSimSlot()));
//             this->openCallWatchers.append(callWatcher);
//             callWatcher->hide();
//         }
//     #endif
// }


/*void WDockManager::addSimDockWidgetP2(QString path, MCUSimControl* simControl)
{
    //qDebug() << "WDockManager: addSimDockWidgetP2()";

    //qDebug() << "WDockManager: return addSimDockWidgetP2()";
}*/


// QDockWidget* WDockManager::getDockWidget(WidgetCode code)
// {
//     qDebug() << "WDockManager: getDockWidget()";
//     QList<WDock*>::iterator i;
//     for (i = openDockWidgets.begin(); i != openDockWidgets.end(); i++)
//     {
//         if ((*i)->cmpCode(code) == true)
//         {
//                 qDebug() << "WDockManager: return getDockWidget()";
//             return (*i)->getQDockWidget();
//         }
//     }
//     qDebug() << "WDockManager: return getDockWidget()";
//     return NULL;
// }


//QDockWidget* WDockManager::getDockWidgetArea(int area)
//{
    //qDebug() << "WDockManager: getDockWidgetArea()";
//    QList<WDock*>::iterator i;
//    for (i = openDockWidgets.begin(); i != openDockWidgets.end(); i++)
//    {
//        if ((*i)->cmpArea(area) == true)
//        {
            //qDebug() << "WDockManager: return getDockWidgetArea()";
//            return (*i)->getQDockWidget();
//        }
//    }
    //qDebug() << "WDockManager: return getDockWidgetArea()";
//    return NULL;
//}


// void WDockManager::hideDockWidgetArea(int area)
// {
//     //qDebug() << "WDockManager: hideDockWidgetArea()";
//     QList<WDock*>::iterator i;
//     for (i = openDockWidgets.begin(); i != openDockWidgets.end(); i++)
//     {
//         if (true == (*i)->cmpArea(area))
//         {
//             switch (area)
//             {
//                 case 1:
//                 {
//                     /*if (wDockRightPrevWidth < (*i)->getQDockWidget()->width())
//                     {
//                         //qDebug() << "WDockManager: old height:" << wDockBotPrevHeight;
//                         //qDebug() << "WDockManager: new height:" << (*i)->getQDockWidget()->height();
//                         wDockRightPrevWidth = (*i)->getQDockWidget()->width();
//                     }*/
//                     (*i)->getQDockWidget()->setMaximumWidth(0);
//                     (*i)->getQDockWidget()->setMinimumWidth(0);
//                     break;
//                 }
//                 case 2:
//                 {
//                     //if (wDockBotPrevHeight < (*i)->getQDockWidget()->height())
//                     //{
//                         //qDebug() << "WDockManager: old height:" << wDockBotPrevHeight;
//                         //qDebug() << "WDockManager: new height:" << (*i)->getQDockWidget()->height();
//                     //    wDockBotPrevHeight = (*i)->getQDockWidget()->height();
//                     //}
//                     (*i)->getQDockWidget()->setMaximumHeight(0);
//                     (*i)->getQDockWidget()->setMinimumHeight(0);
//                     break;
//                 }
//             }
//             //(*i)->getQDockWidget()->resize((*i)->getQDockWidget()->size().width(), 0);
//         }
//     }
//     //qDebug() << "WDockManager: return hideDockWidgetArea()";
// }

// void WDockManager::showDockWidgetArea(int area)
// {
//     //qDebug() << "WDockManager: showDockWidgetArea()";
// /*    QList<WDock*>::iterator i;
//     for (i = openDockWidgets.begin(); i != openDockWidgets.end(); i++)
//     {
//         if (true == (*i)->cmpArea(area))
//         {
//             //vybira si automaticky nejmensi
//             switch(area)
//             {
//                 case 1:
//                 {
//                     if ("Hide" != rightAreaTabs->tabWhatsThis(rightAreaTabs->currentIndex()))
//                     {
//                         //(*i)->getQDockWidget()->setFixedWidth(wDockRightPrevWidth);
//                         (*i)->getQDockWidget()->setMaximumWidth(500);
//                         (*i)->getQDockWidget()->setMinimumWidth(300);
//                         wDockRightPrevWidth = 0;
//                     }
//                     break;
//                 }
//                 case 2:
//                 {
//                     if ("Hide" != bottomAreaTabs->tabText(bottomAreaTabs->currentIndex()))
//                     {
//                         //(*i)->getQDockWidget()->setFixedHeight(wDockBotPrevHeight);
//                         (*i)->getQDockWidget()->setMaximumHeight(999);
//                         (*i)->getQDockWidget()->setMinimumHeight(280);
//                         //(*i)->getQDockWidget()->resize((*i)->getQDockWidget()->size().width(), wDockBotPrevHeight);
//                         wDockBotPrevHeight = 0;
//                     }
//                     break;
//                 }
//             }
//         }
//     }*/
//     //qDebug() << "WDockManager: wDockBotPrevHeight" << wDockBotPrevHeight;
//     //qDebug() << "WDockManager: return showDockWidgetArea()";
// }


bool WDockManager::isEmpty()
{
    return openCentralWidgets.at(m_currTabBarIndex).second->empty();
}


// BookmarkList* WDockManager::getBookmarkList()
// {
//     return bookmarkList;
// }

// BreakpointList* WDockManager::getBreakpointList()
// {
//     return breakpointList;
// }


// void WDockManager::createBreakpointList(QTabWidget *parentTabWidget)
// {
//     //qDebug() << "WDockManager: createBreakpointList()";
//     breakpointList = new BreakpointList(parentTabWidget);
//     parentTabWidget->addTab(breakpointList, "");
//     parentTabWidget->setTabIcon(parentTabWidget->count()-1, QIcon(":resources/icons/processor.png"));
//     parentTabWidget->setTabToolTip(parentTabWidget->count()-1, "Breakpoints");
//     //qDebug() << "WDockManager: return createBreakpointList()";
// }

// void WDockManager::createBookmarkList(QTabWidget *parentTabWidget)
// {
//     //qDebug() << "WDockManager: createBookmarkList()";
//     bookmarkList = new BookmarkList(parentTabWidget);
//     parentTabWidget->addTab(bookmarkList, "");
//     parentTabWidget->setTabIcon(parentTabWidget->count()-1, QIcon(":resources/icons/processor.png"));
//     parentTabWidget->setTabToolTip(parentTabWidget->count()-1, "Bookmarks");
//     //qDebug() << "WDockManager: return createBookmarkList()";
// }


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


//obsolete
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
    //qDebug() << "WDockManager: setCentralByPath()";
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
    //qDebug() << "WDockManager: return setCentralByPath()";
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


// void WDockManager::addDockW(Qt::DockWidgetArea area, QDockWidget* dockWidget)
// {
//     emit addDockWidget(area, dockWidget);
// }


// void WDockManager::unhighlightSimWidget()
// {
//     //qDebug() << "WDockManager: unhighlight";
//     emit unhighlightSim();
// }


void WDockManager::highlightError(QString filename, int line)
{
    qDebug() << "Line in file" << QDir(filename).absolutePath() << "orig" << filename;
    if (false == this->setCentralByPath(QDir(filename).absolutePath()))
    {
        return;
    }
    this->getCentralTextEdit()->scrollToLine(line);
    this->getCentralTextEdit()->selectLine(line);
}


// void WDockManager::handleShowHideBottom(int index)
// {
//     //qDebug() << "WDockManager: bottom changed to" << bottomAreaTabs->tabText(index);
//     if (NULL == activeCodeEdit)
//     {
//         return;
//     }
//     if ("Hide" == bottomAreaTabs->tabText(index))
//     {
//         if (true == bottomVisible)
//         {
//             hideDockWidgetArea(2);
//             bottomVisible = false;
//             QApplication::processEvents();
//             activeCodeEdit->changeHeight();
//             //QTimer::singleShot(50, activeCodeEdit, SLOT(changeHeight()));
//         }
//     }
//     else
//     {
//         if (false == bottomVisible)
//         {
//             showDockWidgetArea(2);
//             bottomVisible = true;
//             QApplication::processEvents();
//             activeCodeEdit->changeHeight();
//             //QTimer::singleShot(50, activeCodeEdit, SLOT(changeHeight()));
//         }
//         /*else
//         {
//             if (bottomAreaTabs->tabText(index) == m_prevBotTabText)
//             {
//                 qDebug() << "WDockManager: same clicked, hide bottom";
//             }
//         }
//         m_prevBotTabText = bottomAreaTabs->tabText(index);*/
//     }
//}


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


// void WDockManager::handleShowHideRight(int index)
// {
    //qDebug() << "WDockManager: right changed to" << rightAreaTabs->tabText(index);
//     if (NULL == activeCodeEdit)
//     {
//         return;
//     }
//     if ("Hide" == rightAreaTabs->tabWhatsThis(index))
//     {
//         if (true == rightVisible)
//         {
//             hideDockWidgetArea(1);
//             rightVisible = false;
//         }
//     }
//     else if (false == rightVisible)
//     {
//         showDockWidgetArea(1);
//         rightVisible = true;
//    }
// }


// void WDockManager::stopSimSlot()
// {
//     //qDebug() << "WDockManager: stopsimslot";
//     emit stopSimSig();
// }


// void WDockManager::clockChangedSlot(double clock, int clockMult)
// {
//     emit clockChangedSig(clock, clockMult);
// }

// void WDockManager::changeSimWidget(int index)
// {
//     //qDebug() << "WDockManager: changeSimWidget index:" << index;
//     //qDebug() << "WDockManager: openSimWidgets size:" <<openSimWidgets.size();
//     if (this->getDockWidget(WSIMULATIONINFO) == NULL)
//     {
//         qDebug() << "Simulation Dock Widget is null, should never happen";
//         return;
//     }
//     if (index >= openSimWidgets.size())
//     {
//         this->getDockWidget(WSIMULATIONINFO)->setWidget(this->openSimWidgets.at(openSimWidgets.size()-1));
//     }
//     else
//     {
//         this->getDockWidget(WSIMULATIONINFO)->setWidget(this->openSimWidgets.at(index));
//     }
//     #ifdef MDS_FEATURE_SIM_CALLWATCHER
//         if (this->getDockWidget(WCALLWATCHER) == NULL)
//         {
//             qDebug() << "Call Watcher Dock Widget is null, should never happen";
//             return;
//         }
//         if (index >= openCallWatchers.size())
//         {
//             this->getDockWidget(WCALLWATCHER)->setWidget(this->openCallWatchers.at(openCallWatchers.size()-1));
//         }
//         else
//         {
//             this->getDockWidget(WCALLWATCHER)->setWidget(this->openCallWatchers.at(index));
//         }
//     #endif
//     #ifdef MDS_FEATURE_SIM_REGWATCHER
//         if (this->getDockWidget(WREGWATCHER) == NULL)
//         {
//             qDebug() << "Reg Watcher Dock Widget is null, should never happen";
//             return;
//         }
//         if (index >= openRegWatchers.size())
//         {
//             this->getDockWidget(WREGWATCHER)->setWidget(this->openRegWatchers.at(openRegWatchers.size()-1));
//         }
//         else
//         {
//             this->getDockWidget(WREGWATCHER)->setWidget(this->openRegWatchers.at(index));
//         }
//     #endif
//     //qDebug() << "WDockManager: changeSimWidget done";
// }


// void WDockManager::deleteActiveSimWidget()
// {
//     //qDebug() << "WDockManager: deleteActiveSimWidget";
//     bool removeDocks = false;
//     PicoBlazeGrid *tempGrid = (PicoBlazeGrid*)(this->getDockWidget(WSIMULATIONINFO)->widget());
//     int index = openSimWidgets.indexOf(tempGrid);
//     if (openSimWidgets.count() > 1)
//     {
//         this->openSimWidgets.removeAt(index);
//         if (index == this->openSimWidgets.count())
//         {
// //            this->getDockWidget(WSIMULATIONINFO)->setWidget(this->openSimWidgets.at(index -1));
//         }
//         else
//         {
// //            this->getDockWidget(WSIMULATIONINFO)->setWidget(this->openSimWidgets.at(index));
//         }
//         delete tempGrid;
//         tempGrid = NULL;
//     }
//     else
//     {
//         this->openSimWidgets.removeAt(index);
//         removeDocks = true;
//         //delete tempGrid;
//         //tempGrid = NULL;
//     }
// 
//     #ifdef MDS_FEATURE_SIM_CALLWATCHER
//     {
//         CallWatcher *tempWatcher = (CallWatcher*)(this->getDockWidget(WCALLWATCHER)->widget());
//         int index2 = openCallWatchers.indexOf(tempWatcher);
//         if (openCallWatchers.count() > 1)
//         {
//             this->openCallWatchers.removeAt(index2);
//             if (index2 == this->openCallWatchers.count())
//             {
// //                this->getDockWidget(WCALLWATCHER)->setWidget(this->openCallWatchers.at(index2 -1));
//             }
//             else
//             {
// //                this->getDockWidget(WCALLWATCHER)->setWidget(this->openCallWatchers.at(index2));
//             }
//             delete tempWatcher;
//             tempWatcher = NULL;
//         }
//         else
//         {
//             this->openCallWatchers.removeAt(index2);
//             //delete tempWatcher;
//             //tempGrid = NULL;
//         }
//     }
//     #endif
//     #ifdef MDS_FEATURE_SIM_REGWATCHER
//     {
//         RegWatcher *tempWatcher = (RegWatcher*)(this->getDockWidget(WREGWATCHER)->widget());
//         int index2 = openRegWatchers.indexOf(tempWatcher);
//         if (openRegWatchers.count() > 1)
//         {
//             this->openRegWatchers.removeAt(index2);
//             if (index2 == this->openRegWatchers.count())
//             {
// //                this->getDockWidget(WREGWATCHER)->setWidget(this->openRegWatchers.at(index2 -1));
//             }
//             else
//             {
// //                this->getDockWidget(WREGWATCHER)->setWidget(this->openRegWatchers.at(index2));
//             }
//             delete tempWatcher;
//             tempWatcher = NULL;
//         }
//         else
//         {
//             this->openRegWatchers.removeAt(index2);
//             //delete tempWatcher;
//             //tempGrid = NULL;
//         }
//     }
//     #endif
// 
//     if (true == removeDocks)
//     {
//         /*while (this->openDockWidgets.count() > 0)
//         {
//             WDock *tmpDock = this->openDockWidgets.at(0);
//             this->openDockWidgets.removeAt(0);
//             delete tmpDock->getQDockWidget();
//             delete tmpDock;
//         }
//         this->breakpointList = NULL;
//         this->bookmarkList = NULL;
//         this->bottomAreaTabs = NULL;
//         this->rightAreaTabs = NULL;
//         this->dockWidgets = false;*/
//     }
//     //qDebug() << "WDockManager: deleteActiveSimWidget done";
// }


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
    if (this->splitter->count() == 1)
    {
        this->activeHelpBrowser->hide();
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


// void WDockManager::compilationFinishedSlot(bool success)
// {
// }


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
        if (projectPath == openCentralWidgets.at(i).first->id)
        {
            if (m_currTabBarIndex == i)
            {
                return;
            }
            //qDebug() << "WDockManager: tabs checked";
            if (wTab != NULL)
            {
                wTab->hide();
            }
            //qDebug() << "WDockManager: tabs checked 2";
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
            m_currTabBarIndex = -1;
            break;
        }
    }
    if (openCentralWidgets.count() == 0)
    {
        delete centralBase;
        centralBase = NULL;
        activeCodeEdit = NULL;
//         this->hideDockWidgetArea(1);
//         this->hideDockWidgetArea(2);
        emit centralClosed();
    }
    /*if (NULL == wTab && openCentralWidgets.count() != 0)
    {
        m_currTabBarIndex = 0;
        wTab = openCentralWidgets.at(0).first;
    }*/
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


// void WDockManager::setBottomAreaToCompilerInfo()
// {
//     if (false == bottomVisible)
//     {
//         showDockWidgetArea(2);
//     }
//     for (int i = 0; i < bottomAreaTabs->count(); i++)
//     {
//         if ("Compiler Messages" == bottomAreaTabs->tabText(i))
//         {
//             bottomAreaTabs->setCurrentIndex(i);
//             break;
//         }
//     }
// }


// void WDockManager::setBottomAreaToExtAppOutput()
// {
//     if (false == bottomVisible)
//     {
//         showDockWidgetArea(2);
//     }
//     for (int i = 0; i < bottomAreaTabs->count(); i++)
//     {
//         if ("External Applications" == bottomAreaTabs->tabText(i))
//         {
//             bottomAreaTabs->setCurrentIndex(i);
//             break;
//         }
//     }
// }


// void WDockManager::setBottomAreaToSimulationInfo()
// {
//     if (false == bottomVisible)
//     {
//         showDockWidgetArea(2);
//     }
//     for (int i = 0; i < bottomAreaTabs->count(); i++)
//     {
//         if ("Simulator" == bottomAreaTabs->tabText(i))
//         {
//             bottomAreaTabs->setCurrentIndex(i);
//             break;
//         }
//     }
// }


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


void WDockManager::setHelpBrowserPath(const QUrl &url)
{
    m_helpBrowserUrl = url;
    activeHelpBrowser->setSource(url);
    if (NULL != activeCodeEdit)
    {
        activeCodeEdit->hide();
    }
    activeHelpBrowser->show();
    bool found = false;
    int i;
    for (i = 0; i < wTab->count(); i++)
    {
        if ("Help Browser" == wTab->tabText(i))
        {
            found = true;
            break;
        }
    }
    if (false == found)
    {
        wTab->addTab("Help Browser");
        wTab->tabAdded();
        wTab->setTabToolTip(wTab->count()-1, "Help Browser");
        wTab->setCurrentIndex(wTab->count()-1);
        openCentralWidgets.at(m_currTabBarIndex).second->append(NULL);
    }
    else
    {
        wTab->setCurrentIndex(i);
    }
}




/////
///// WDock
/////

// WDock::WDock(WDockManager *parent, WidgetCode code, QTabWidget *parentTabWidget)
//     : QObject(parentTabWidget)
// {
//     //qDebug() << "WDock: WDock()";
//     switch (code)
//     {
//         case WBOOKMARKLIST:
//         {
//             /*wDockWidget = new QDockWidget("Bookmarks", parentTabWidget);
//             wDockWidget->setAllowedAreas(Qt::RightDockWidgetArea);
//             wDockWidget->setFeatures(QDockWidget::NoDockWidgetFeatures);
//             QLabel *lbl = new QLabel("Bookmarks", wDockWidget);
// 
//             wDockWidget->setTitleBarWidget(lbl);
//             parent->addDockW(Qt::RightDockWidgetArea, wDockWidget);*/
//             //mainWindow->addDockWidget(Qt::RightDockWidgetArea, wDockWidget);
//             parent->createBookmarkList(parentTabWidget);
//             area = 1;
//             //wDockWidget->setWidget(parent->getBookmarkList());
// 	        break;
//         }
//         case WBREAKPOINTLIST:
//         {
//             /*wDockWidget = new QDockWidget("Breakpoints", parentTabWidget);
//             //wDockWidget->setToolTip("Breakpoints");
//             wDockWidget->setAllowedAreas(Qt::RightDockWidgetArea);
//             wDockWidget->setFeatures(QDockWidget::NoDockWidgetFeatures);
//             QLabel *lbl = new QLabel("Breakpoints", wDockWidget);
// 
//             wDockWidget->setTitleBarWidget(lbl);
//             parent->addDockW(Qt::RightDockWidgetArea, wDockWidget);*/
//             //mainWindow->addDockWidget(Qt::RightDockWidgetArea, wDockWidget);
//             parent->createBreakpointList(parentTabWidget);
//             area = 1;
//             //wDockWidget->setWidget(parent->getBreakpointList());
// 	        break;
//         }
//         case WCOMPILEINFO:
//         {
//             /*wDockWidget = new QDockWidget("Compiler Messages", parentTabWidget);
//             wDockWidget->setAllowedAreas(Qt::BottomDockWidgetArea);
//             wDockWidget->setFeatures(QDockWidget::NoDockWidgetFeatures);
//             parent->addDockW(Qt::BottomDockWidgetArea, wDockWidget);*/
//             //mainWindow->addDockWidget(Qt::BottomDockWidgetArea, wDockWidget);
//             //QPlainTextEdit *newWidget = new QPlainTextEdit(wDockWidget);
//             CompileInfo *newWidget = new CompileInfo(parentTabWidget);
//             newWidget->setFont(QFont("UbuntuMono", 10));
//             area = 2;
//             newWidget->setReadOnly(true);
//             //wDockWidget->setWidget(newWidget);
//             //parent->connect(wDockWidget, SIGNAL(visibilityChanged(bool)), parent, SLOT(showBottomArea(bool)));
//             parent->connect(newWidget, SIGNAL(errorClicked(QString, int)), parent, SLOT(highlightError(QString, int)));
// 	        break;
//         }
//         case WSIMULATIONINFO:
//         {
//             qDebug() << "WDock: invalid usage of constructor";
//             /*wDockWidget = new QDockWidget("Simulation Info", parentWindow);
//             wDockWidget->setAllowedAreas(Qt::BottomDockWidgetArea);
//             parent->addDockW(Qt::BottomDockWidgetArea, wDockWidget);
//             //mainWindow->addDockWidget(Qt::BottomDockWidgetArea, wDockWidget);
//             PicoBlazeGrid *newWidget = new PicoBlazeGrid(wDockWidget, mainWindow->getProjectMan()->getActive()->getSimControl());
//             newWidget->setProjectPath(mainWindow->getProjectMan()->getActive()->prjPath);
//             area = 2;
//             wDockWidget->setWidget(newWidget);
//             newWidget->fixHeight();
//             qDebug() << "WSimulationInfo: height fixed";*/
// 	        break;
//         }
//         /*case wAnalysVar:
//         {
//             wDockWidget = new QDockWidget("Variables", parentWindow);
//             wDockWidget->setAllowedAreas(Qt::RightDockWidgetArea);
//             parent->addDockW(Qt::RightDockWidgetArea, wDockWidget);
//             //mainWindow->addDockWidget(Qt::RightDockWidgetArea, wDockWidget);
//             AnalyserWidget *newDock = new AnalyserWidget(wDockWidget);
//             area = 1;
//             wDockWidget->setWidget(newDock);
//             newDock->connect(newDock, SIGNAL(itemDoubleClicked(QListWidgetItem *)), parent, SLOT(changeLine(QListWidgetItem *)));
//             break;
//         }
//         case wAnalysFunc:
//         {
//             wDockWidget = new QDockWidget("Functions", parentWindow);
//             wDockWidget->setAllowedAreas(Qt::RightDockWidgetArea);
//             parent->addDockW(Qt::RightDockWidgetArea, wDockWidget);
//             //mainWindow->addDockWidget(Qt::RightDockWidgetArea, wDockWidget);
//             AnalyserWidget *newDock = new AnalyserWidget(wDockWidget);
//             area = 1;
//             wDockWidget->setWidget(newDock);
//             break;
//         }*/
//         case WBOTTOMHIDE:
//         {
//             /*wDockWidget = new QDockWidget("Hide", parentTabWidget);
//             wDockWidget->setAllowedAreas(Qt::BottomDockWidgetArea);
//             wDockWidget->setFeatures(QDockWidget::NoDockWidgetFeatures);
//             parent->addDockW(Qt::BottomDockWidgetArea, wDockWidget);
//             //ShowHideWidget *newDock = new ShowHideWidget(wDockWidget);*/
//             QWidget *newDock = new QWidget(parentTabWidget);
//             area = 2;
//             //wDockWidget->setWidget(newDock);
//             //QPushButton *pshTitle = new QPushButton("Hide", parentWindow);
//             //wDockWidget->setTitleBarWidget(pshTitle);
//             //parent->connect(wDockWidget, SIGNAL(visibilityChanged(bool)), parent, SLOT(hideBottomArea(bool)));
//             //parent->connect(pshTitle, SIGNAL(clicked()), parent, SLOT(showBottomArea()));
//             break;
//         }
//         case WRIGHTHIDE:
//         {
//             /*wDockWidget = new QDockWidget("Hide", parentTabWidget);
//             wDockWidget->setAllowedAreas(Qt::RightDockWidgetArea);
//             wDockWidget->setFeatures(QDockWidget::NoDockWidgetFeatures);
//             parent->addDockW(Qt::RightDockWidgetArea, wDockWidget);*/
//             //ShowHideWidget *newDock = new ShowHideWidget(wDockWidget);
//             QWidget *newDock = new QWidget(parentTabWidget);
//             area = 1;
//             //wDockWidget->setWidget(newDock);
//             //QPushButton *pshTitle = new QPushButton("Hide", parentWindow);
//             //wDockWidget->setTitleBarWidget(pshTitle);
//             //parent->connect(wDockWidget, SIGNAL(visibilityChanged(bool)), parent, SLOT(hideBottomArea(bool)));
//             //parent->connect(pshTitle, SIGNAL(clicked()), parent, SLOT(showBottomArea()));
//             break;
//         }
//         case WASMMACROANALYSER:
//         {
//             /*wDockWidget = new QDockWidget("Macros", parentTabWidget);
//             wDockWidget->setAllowedAreas(Qt::RightDockWidgetArea);
//             wDockWidget->setFeatures(QDockWidget::NoDockWidgetFeatures);
//             QLabel *lbl = new QLabel("Macros", wDockWidget);
// 
//             wDockWidget->setTitleBarWidget(lbl);
//             parent->addDockW(Qt::RightDockWidgetArea, wDockWidget);*/
//             AsmMacroAnalyser *newDock = new AsmMacroAnalyser(parentTabWidget);
//             area = 1;
//             //wDockWidget->setWidget(newDock);
//             break;
//         }
//         #ifdef MDS_FEATURE_EXTERNAL_APPS
//         case WEXTAPPOUTPUT:
//         {
//             /*wDockWidget = new QDockWidget("External Applications", parentTabWidget);
//             wDockWidget->setAllowedAreas(Qt::BottomDockWidgetArea);
//             wDockWidget->setFeatures(QDockWidget::NoDockWidgetFeatures);
//             parent->addDockW(Qt::BottomDockWidgetArea, wDockWidget);*/
//             ExtAppOutput *newDock = new ExtAppOutput(parentTabWidget);
//             area = 2;
//             //wDockWidget->setWidget(newDock);
//             break;
//         }
//         #endif // MDS_FEATURE_EXTERNAL_APPS
//         case WHELPDOCKWIDGET:
//         {
//             /*wDockWidget = new QDockWidget("Help", parentTabWidget);
//             wDockWidget->setAllowedAreas(Qt::RightDockWidgetArea);
//             wDockWidget->setFeatures(QDockWidget::NoDockWidgetFeatures);
//             QLabel *lbl = new QLabel("Help", wDockWidget);
// 
//             wDockWidget->setTitleBarWidget(lbl);
//             parent->addDockW(Qt::RightDockWidgetArea, wDockWidget);*/
//             HelpDockWidget *newDock = new HelpDockWidget(parentTabWidget);
//             area = 1;
//             //wDockWidget->setWidget(newDock);
//             connect(newDock, SIGNAL(showHelpContent(const QUrl &)), parent, SLOT(setHelpBrowserPath(const QUrl &)));
//             break;
//         }
//         case WCALLWATCHER:
//         {
//             qDebug() << "WDockManager: invalid use of wcallwatcher constructor, should never happen";
//             break;
//         }
//         case WREGWATCHER:
//         {
//             qDebug() << "WDockManager: invalid use of wregwatcher constructor, should never happen";
//             break;
//         }
//         default:
//         {
//             qDebug() << "WDockManager: unknown widget code";
//         }
//     }
//     this->code=code;
//     //qDebug() << "WDock: return WDock()";
// }
// 
// 
// 
// WDock::WDock(WDockManager *parent, WidgetCode code, QTabWidget *parentTabWidget, MCUSimControl* simControl)
//     : QObject(parent)
// {
//     switch (code)
//     {
//         case WSIMULATIONINFO:
//         {
//             //wDockWidget = new QDockWidget("Simulator", parentWindow);
//             //wDockWidget->setAllowedAreas(Qt::BottomDockWidgetArea);
//             //parent->addDockW(Qt::BottomDockWidgetArea, wDockWidget);
//             //wDockWidget->setFeatures(QDockWidget::NoDockWidgetFeatures);
//             //mainWindow->addDockWidget(Qt::BottomDockWidgetArea, wDockWidget);
//             //PicoBlazeGrid *newWidget = new PicoBlazeGrid(wDockWidget, simControl);
//             PicoBlazeGrid *newWidget = new PicoBlazeGrid(parentTabWidget, simControl);
//             connect(parent, SIGNAL(unhighlightSim()), newWidget, SLOT(unhighlight()));
//             //newWidget->setProjectPath(path);
//             area = 2;
//             //wDockWidget->setWidget(newWidget);
//             parentTabWidget->addTab(newWidget, "");
//             parentTabWidget->setTabIcon(parentTabWidget->count()-1, QIcon(":resources/icons/processor.png"));
//             parentTabWidget->setTabToolTip(parentTabWidget->count()-1, "Simulator");
//             //wDockWidget->show();
//             newWidget->fixHeight();
//             connect(newWidget, SIGNAL(stopSimSig()), parent, SLOT(stopSimSlot()));
//             connect(newWidget, SIGNAL(clockChanged(double, int)), parent, SLOT(clockChangedSlot(double, int)));
//             parent->openSimWidgets.append(newWidget);
//             //qDebug() << "WSimulationInfo: height fixed";
//             //parent->connect(wDockWidget, SIGNAL(visibilityChanged(bool)), parent, SLOT(showBottomArea(bool)));
//             break;
//         }
//         case WCALLWATCHER:
//         {
//             #ifdef MDS_FEATURE_SIM_CALLWATCHER
//                 /*wDockWidget = new QDockWidget("Call Watcher", parentTabWidget);
//                 wDockWidget->setAllowedAreas(Qt::RightDockWidgetArea);
//                 wDockWidget->setFeatures(QDockWidget::NoDockWidgetFeatures);
//                 QLabel *lbl = new QLabel("Call Watcher", wDockWidget);
// 
//                 wDockWidget->setTitleBarWidget(lbl);
//                 parent->addDockW(Qt::RightDockWidgetArea, wDockWidget);*/
//                 CallWatcher *newDock = new CallWatcher(parentTabWidget, simControl);
//                 parentTabWidget->addTab(newDock, "");
//                 parentTabWidget->setTabIcon(parentTabWidget->count()-1, QIcon(":resources/icons/processor.png"));
//                 parentTabWidget->setTabToolTip(parentTabWidget->count()-1, "Call Watcher");
//                 area = 1;
//             #endif
//             break;
//         }
//         case WREGWATCHER:
//         {
//             #ifdef MDS_FEATURE_SIM_REGWATCHER
//                /*wDockWidget = new QDockWidget("Reg Watcher", parentTabWidget);
//                 wDockWidget->setAllowedAreas(Qt::RightDockWidgetArea);
//                 wDockWidget->setFeatures(QDockWidget::NoDockWidgetFeatures);
//                 QLabel *lbl = new QLabel("Reg Watcher", wDockWidget);
// 
//                 wDockWidget->setTitleBarWidget(lbl);
//                 parent->addDockW(Qt::RightDockWidgetArea, wDockWidget);*/
//                 RegWatcher *newDock = new RegWatcher(parentTabWidget, simControl);
//                 parentTabWidget->addTab(newDock, "");
//                 parentTabWidget->setTabIcon(parentTabWidget->count()-1, QIcon(":resources/icons/processor.png"));
//                 parentTabWidget->setTabToolTip(parentTabWidget->count()-1, "Memory Watcher");
//                 area = 1;
//                 //wDockWidget->setWidget(newDock);
//             #endif
//             break;
//         }
//         default:
//         {
//             qDebug() << "WDockManager: unknown widget code";
//         }
//     }
//     this->code = code;
// }
// 
// 
// 
// WDock::~WDock()
// {
//     /*if (wDockWidget != NULL)
//     {
//         delete wDockWidget;
//     }*/
// }
// 
// 
// 
// QDockWidget* WDock::getQDockWidget()
// {
//     return wDockWidget;
// }
// 
// 
// 
// bool WDock::cmpCode(WidgetCode code)
// {
//     return (this->code==code);
// }
// 
// 
// bool WDock::cmpArea(int area)
// {
//     return (this->area==area);
// }
// 
// 
// int WDock::getArea()
// {
//     return this->area;
// }
