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


#include <QDockWidget>
#include <QMainWindow>
#include <QList>
#include <QSplitter>
#include "../Editor/codeedit.h"
#include "../Editor/baseeditor.h"
#include "../../mainform/mainform.h"
#include "../SimulationInfo/wsimulationinfo.h"
#include "../BreakpointList/breakpointlist.h"
#include "../BookmarkList/bookmarklist.h"
#include "../TabBar/tabbar.h"
#include "../AnalyserWidget/analyserwidget.h"
#include "../Analyser/analys.h"

class WDock;
class MainForm;
class CodeEdit;
class BaseEditor;
class BookmarkList;
class BreakpointList;


/**
 * @brief
 * @ingroup GUI
 * @class WDockManager
 */
class WDockManager : public QObject
{
    Q_OBJECT
    public:
        WDockManager(MainForm *mainWindow);
        void addDockWidget(int code);
        QDockWidget* getDockWidget(int code);
        QDockWidget* getDockWidgetArea(int area);
        void hideDockWidgetArea(int area);
        void showDockWidgetArea(int area);
        void removeDockWidget(int code);
        void addCentralWidget(QString wName, QString wPath);
        QTextEdit* getCentralTextEdit();
        QTextEdit* getTabTextEdit(int index);
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

    private slots:
        void closeTab(int index);
        void changeTabName(CodeEdit* editor, QString name);
        void changeCodeEditor(int index);
        void changeActiveCodeEdit(CodeEdit* editor);
        void updateAnalysersSlot(CodeEdit *editor);

    signals:
        void saveCodeEdit(CodeEdit *editor);

    private:
        MainForm *wMainWindow;
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
        
        //widgets
        BookmarkList *bookmarkList;
        BreakpointList *breakpointList;
};



/**
 * @brief
 * @ingroup GUI
 * @class WDock
 */
class WDock
{
    public:
        WDock(WDockManager *parent, int code, MainForm *mainWindow);
        ~WDock();
        bool cmpCode(int code);
        bool cmpArea(int area);
        int getArea();
        QDockWidget* getQDockWidget();
    private:
        QDockWidget *wDockWidget;
        int code;
        int area;
};


#endif
