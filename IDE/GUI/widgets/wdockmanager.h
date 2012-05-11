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

#define wListCode 1000
#define wListCode2 1001
#define wCompileInfo 1002
#define wCodeEditCode 1003
#define wSimulationInfo 1004
//HexEdit would be included in Simulation Status widget


#include <QDockWidget>
#include <QMainWindow>
#include <QList>
#include <QTabWidget>
#include "codeedit.h"
#include  "../mainform/mainform.h"
#include "wsimulationinfo.h"

class WDock;
class MainForm;
class CodeEdit;
class HexEdit;


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

    private slots:
        void closeTab(int index);

    private:
        MainForm *wMainWindow;
        QList<WDock*> openDockWidgets;
        WDock* wLeft;
        WDock* wBottom;
        WDock* wRight;
        QList<CodeEdit*> openCentralWidgets;
        QTabWidget *wTab;
        
};



/**
 * @brief
 * @ingroup GUI
 * @class WDock
 */
class WDock
{
    public:
        WDock(int code, QMainWindow *mainWindow);
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
