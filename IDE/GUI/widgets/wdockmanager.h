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


#include <QDockWidget>
#include <QMainWindow>
#include <QList>
#include <QTabWidget>
#include "codeedit.h"
#include  "../mainform/mainform.h"

class WDock;
class MainForm;
class CodeEdit;


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
        void removeDockWidget(int code);
        void addCentralWidget(QString wName, QString wPath);
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
        QDockWidget* getQDockWidget();
    private:
        QDockWidget *wDockWidget;
        int code;
};


#endif
