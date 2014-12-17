/**
 * @brief
 * C++ Interface: ...
 *
 * ...
 *
 * (C) copyright 2013, 2014 Moravia Microsystems, s.r.o.
 *
 * @author Erik Chalupa <erik.chalupa@moravia-microsystems.com>
 * @ingroup GUI
 * @file wdockmanager.h
 */


#ifndef WDOCKMANAGER_H
#define WDOCKMANAGER_H

// #define wBookmarkList 1000
// #define wBreakpointList 1001
// #define wCompileInfo 1002
// #define wCodeEditCode 1003
// #define wSimulationInfo 1004
// #define wAnalysVar 1005
// #define wAnalysFunc 1006
// #define wBottomHide 1007
// #define wRightHide  1008
// #define wAsmMacroAnalyser 1009
// #define wOutputWidget 1010


//#include <QMainWindow>
#include <QList>
#include <QObject>
#include <QPair>
//#include "../Editor/wtextedit.h"
//#include "../../mainform/mainform.h"
//#include "../SimulationInfo/wsimulationinfo.h"
//#include "../ShowHideWidget/showhidewidget.h"
//#include "../WelcomeScr/welcomescr.h"
#include "../../enums/enums.h"
//#include "../../project/project.h"

class WDock;
class CodeEdit;
class BaseEditor;
class BookmarkList;
class BreakpointList;
class WTextEdit;
class Project;
class WelcomeScr;
class PicoBlazeGrid;
class TabBar;
class QTabBar;
class MCUSimControl;
class QSplitter;
class QListWidgetItem;
class QDockWidget;
class QVBoxLayout;


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
        void addDockWidget(WidgetCode code);
        void addSimDockWidgetP1();
        void addSimDockWidgetP2(QString path, MCUSimControl* simControl);
        QDockWidget* getDockWidget(WidgetCode code);
        QDockWidget* getDockWidgetArea(int area);
        void hideDockWidgetArea(int area);
        void showDockWidgetArea(int area);
        void removeDockWidget(WidgetCode code);
        bool addUntrackedCentralWidget(QString wName, QString wPath);
        bool addUntrackedCentralWidget(QString wName, QString wPath, QStringList text);
        bool addCentralWidget(QString wName, QString wPath);
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
        bool getTabStatusChanged(int index);
        int getTabCount();
        void setCentralByIndex(int index);
        bool isEmpty();
        BookmarkList* getBookmarkList();
        BreakpointList* getBreakpointList();
        void createBookmarkList(QDockWidget *wDockWidget);
        void createBreakpointList(QDockWidget *wDockWidget);
        void setCentralByName(QString fileName);
        bool setCentralByPath(QString filePath);
        CodeEdit* getCentralByPath(QString filePath);
        void setEditorsReadOnly(bool readonly);
        void changeSimWidget(int index);
        void deleteActiveSimWidget();
        void closeFile(QString path);
        void setCentralWelcome(); //welcome screen
        void deleteCentralWelcome();
        void setBottomAreaToCompilerInfo();
        void setBottomAreaToSimulationInfo();
        void showProjectEditors(QString projectPath);
        void appendTabBar(QString projectPath);
        void removeTabBar(QString projectPath);
        
        bool dockWidgets;
        QTabBar *bottomAreaTabs;
        //QTabBar *leftAreaTabs;
        QTabBar *rightAreaTabs;
        QList<PicoBlazeGrid*> openSimWidgets;

    public slots:
        void changeLine(QListWidgetItem *item);
        void addDockW(Qt::DockWidgetArea area, QDockWidget* dockWidget);
        void unhighlightSimWidget();
        void highlightError(QString filename, int line);
        void handleShowHideBottom(int index);
        //void handleShowHideLeft(int index);
        void handleShowHideRight(int index);
        void compilationFinishedSlot(bool success);
        void changeTab(bool next);
        void closeCurrentTab();
        

    private slots:
        void changeTabStatusSlot(QString name, QString path, bool changed);
        void changeCodeEditor(int index);
        void changeActiveCodeEdit(CodeEdit* editor);
        //void updateAnalysersSlot(CodeEdit *editor);
        void moveEditorsSlot(int from, int to);
        void stopSimSlot();
        void clockChangedSlot(double clock, int clockMult);
        void breakpointEmitSlot(QString file, int line);
        void breakpointsAddLinesSlot(QString file, int line, int linesAdded);
        void breakpointsRemoveLinesSlot(QString file, int line, int linesRemoved);
        void bookmarkEmitSlot(QString file, int line);
        void bookmarksAddLinesSlot(QString file, int line, int linesAdded);
        void bookmarksRemoveLinesSlot(QString file, int line, int linesRemoved);
        void welcomeScrExample();
        void welcomeScrOpenPrj();
        void welcomeScrNewPrj();
        void welcomeScrManual();
        void welcomeScrGuide();
        void welcomeScrRecent(QString path);
        void closeTab(int index);
        //void breakpointListRemoveSlot(QString file, int line);
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
        void clockChangedSig(double clock, int clockMult);
        void centralClosed();
        void centralCreated();
        void breakpointEmit(QString file, int line);
        void bookmarkEmit(QString file, int line);
        void breakpointsAddLines(QString file, int line, int linesAdded);
        void breakpointsRemoveLines(QString file, int line, int linesRemoved);
        void bookmarksAddLines(QString file, int line, int linesAdded);
        void bookmarksRemoveLines(QString file, int line, int linesRemoved);
        //void breakpointListRemove(QString file, int line);
        void compilationFinishedSig(bool success);
        void welcomeScrExampleSig();
        void welcomeScrOpenPrjSig();
        void welcomeScrNewPrjSig();
        void welcomeScrManualSig();
        void welcomeScrGuideSig();
        void welcomeScrRecentSig(QString path);
        void tabClosed(QString path);

    private:
        //MainForm *wMainWindow;
        QList<WDock*> openDockWidgets;
        WDock *wLeft;
        WDock *wBottom;
        WDock *wRight;
        QList<QPair<TabBar*, QList<BaseEditor*>*>> openCentralWidgets;
        BaseEditor *centralBase;
        TabBar *wTab;
        //QList<TabBar*> m_tabBarList;
        QSplitter *splitter;
        CodeEdit *activeCodeEdit;
        //QList<CodeEdit*> codeEditList;
        QString tmpPrjPath;
        int m_currTabBarIndex;

        QVBoxLayout *m_layout;
        QWidget *m_centralWidget;
        
        //widgets
        BookmarkList *bookmarkList;
        BreakpointList *breakpointList;

        //welcome screen
        WelcomeScr *welcomeScr;
       

        //wdockwidgets previous height
        int wDockBotPrevHeight;
        int wDockRightPrevWidth;
        bool bottomVisible;
        bool rightVisible;
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
        WDock(WDockManager *parent, WidgetCode code, QWidget *parentWindow);
        WDock(WDockManager *parent, WidgetCode code, QWidget *parentWindow, QString path, MCUSimControl* simControl);
        ~WDock();
        bool cmpCode(WidgetCode code);
        bool cmpArea(int area);
        int getArea();
        QDockWidget* getQDockWidget();

    /*signals:
        void stopSimSig();

    private slots:
        void stopSimSlot();*/
        
    private:
        QDockWidget *wDockWidget;
        WidgetCode code;
        int area;
};


#endif
