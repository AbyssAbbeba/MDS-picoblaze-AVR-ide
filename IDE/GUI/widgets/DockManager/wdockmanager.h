/**
 * @brief
 * C++ Interface: ...
 *
 * ...
 *
 * Copyright: See COPYING file that comes with this distribution.
 *
 * @author Erik Chalupa <xchalu10@stud.fit.vutbr.cz>, (C) 2012
 * @ingroup GUI
 * @file wdockmanager.h
 */




#ifndef WDOCKMANAGER_H
#define WDOCKMANAGER_H

#define wBookmarkList 1000
#define wBreakpointList 1001
#define wCompileInfo 1002
#define wCodeEditCode 1003
#define wSimulationInfo 1004
#define wAnalysVar 1005
#define wAnalysFunc 1006
#define wBottomHide 1007


#include <QDockWidget>
//#include <QMainWindow>
#include <QList>
#include <QSplitter>
#include <QTabBar>
#include "../Editor/codeedit.h"
#include "../Editor/baseeditor.h"
#include "../Editor/wtextedit.h"
//#include "../../mainform/mainform.h"
//#include "../SimulationInfo/wsimulationinfo.h"
#include "../PicoBlazeGrid/picoblazegrid.h"
#include "../BreakpointList/breakpointlist.h"
#include "../BookmarkList/bookmarklist.h"
#include "../TabBar/tabbar.h"
#include "../AnalyserWidget/analyserwidget.h"
#include "../Analyser/analys.h"
//#include "../ShowHideWidget/showhidewidget.h"

class WDock;
class CodeEdit;
class BaseEditor;
class BookmarkList;
class BreakpointList;
class WTextEdit;


/**
 * @brief
 * @ingroup GUI
 * @class WDockManager
 */
class WDockManager : public QObject
{
    Q_OBJECT
    public:
        WDockManager(QWidget *parent, QWidget *centralWidget);
        void addDockWidget(int code);
        void addSimDockWidgetP1();
        void addSimDockWidgetP2(QString path, MCUSimControl* simControl);
        QDockWidget* getDockWidget(int code);
        QDockWidget* getDockWidgetArea(int area);
        void hideDockWidgetArea(int area);
        void showDockWidgetArea(int area);
        void removeDockWidget(int code);
        void addUntrackedCentralWidget(QString wName, QString wPath);
        void addUntrackedCentralWidget(QString wName, QString wPath, QStringList text);
        void addCentralWidget(QString wName, QString wPath);
        WTextEdit* getCentralTextEdit();
        WTextEdit* getTabTextEdit(int index);
        CodeEdit* getCentralWidget();
        CodeEdit* getTabWidget(int index);
        QString getCentralName();
        QString getCentralPath();
        void setCentralName(QString wName);
        void setCentralPath(QString wPath);
        void setTabChanged();
        void setTabSaved();
        int getTabCount();
        bool isEmpty();
        BookmarkList* getBookmarkList();
        BreakpointList* getBreakpointList();
        void createBookmarkList(QDockWidget *wDockWidget);
        void createBreakpointList(QDockWidget *wDockWidget);
        void setCentralByName(QString fileName);
        void setCentralByPath(QString filePath);
        void setEditorsReadOnly(bool readonly);
        void changeSimWidget(int index);
        
        bool dockWidgets;
        QTabBar *bottomAreaTabs;
        QList<PicoBlazeGrid*> openSimWidgets;

    public slots:
        void changeLine(QListWidgetItem *item);
        void addDockW(Qt::DockWidgetArea area, QDockWidget* dockWidget);
        void unhighlightSimWidget();
        void highlightError(QString filename, int line);
        void handleShowHideBottom(int index);
        

    private slots:
        void closeTab(int index);
        void changeTabStatusSlot(QString name, QString path, bool changed);
        void changeCodeEditor(int index);
        void changeActiveCodeEdit(CodeEdit* editor);
        void updateAnalysersSlot(CodeEdit *editor);
        void moveEditorsSlot(int from, int to);
        void stopSimSlot();
        //void hideBottomArea(bool show);
        //void showBottomArea(bool show);
        //void dockWidgetsCreated();

    signals:
        void saveCodeEdit(CodeEdit *editor);
        void createDockWidgets();
        void tabifyDockWidget(QDockWidget *widget1, QDockWidget* widget2);
        void addDockWidget(Qt::DockWidgetArea area, QDockWidget* dockWidget);
        void getSimProjectData();
        void unhighlightSim();
        void stopSimSig();

    private:
        //MainForm *wMainWindow;
        QList<WDock*> openDockWidgets;
        WDock *wLeft;
        WDock *wBottom;
        WDock *wRight;
        QList<BaseEditor*> openCentralWidgets;
        BaseEditor *centralBase;
        TabBar *wTab;
        QSplitter *splitter;
        CodeEdit *activeCodeEdit;
        QList<CodeEdit*> codeEditList;
        QString tmpPrjPath;
        MCUSimControl* tmpSimControl;
        
        //widgets
        BookmarkList *bookmarkList;
        BreakpointList *breakpointList;

        //wdockwidgets previous height
        int wDockBotPrevHeight;
        bool visible;
};



/**
 * @brief
 * @ingroup GUI
 * @class WDock
 */
class WDock : public QObject
{
    Q_OBJECT
    public:
        WDock(WDockManager *parent, int code, QWidget *parentWindow);
        WDock(WDockManager *parent, int code, QWidget *parentWindow, QString path, MCUSimControl* simControl);
        //~WDock();
        bool cmpCode(int code);
        bool cmpArea(int area);
        int getArea();
        QDockWidget* getQDockWidget();

    signals:
        void stopSimSig();

    private slots:
        void stopSimSlot();
        
    private:
        QDockWidget *wDockWidget;
        int code;
        int area;
};


#endif
