#ifndef WDOCKMANAGER_H
#define WDOCKMANAGER_H

#define wListCode 1000
#define wListCode2 1001
#define wListCode3 1002
#define wCodeEditCode 1003

#include <QDockWidget>
#include <QMainWindow>
#include <QList>
#include <QTabWidget>
#include "codeedit.h"

class WDock;

class WDockManager
{
    //Q_OBJECT
    public:
        WDockManager(QMainWindow *mainWindow);
        void addDockWidget(int code);
        void removeDockWidget(int code);
        void addCentralWidget(QString name);
        CodeEdit* getCentralWidget();
    private:
        //slo by i zavolat funkci parent() misto promenne
        QMainWindow *wMainWindow;
        QList<WDock*> openDockWidgets;
        QList<CodeEdit*> openCentralWidgets;
        QTabWidget *wTab;
        
};


class WDock
{
    public:
        WDock(int code, QMainWindow *mainWindow);
        ~WDock();
    private:
        QDockWidget *wDockWidget;
};


#endif
