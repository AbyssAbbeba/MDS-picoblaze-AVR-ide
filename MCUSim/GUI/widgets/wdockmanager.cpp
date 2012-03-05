#include <QtGui>
#include "wdockmanager.h"

WDockManager::WDockManager(QMainWindow *mainWindow)
{
    wMainWindow = mainWindow;
    wTab = new QTabWidget(wMainWindow);
    wTab->setTabsClosable(true);
    wTab->setMovable(true);
    //connect(this, currentChanger(int),...)
    //connect(this, tabCloseRequest(int),...)
    wMainWindow->setCentralWidget(wTab);
}


CodeEdit* WDockManager::getCentralWidget()
{
    return (CodeEdit*)wTab->currentWidget();
}


void WDockManager::addCentralWidget(QString name)
{
    
    CodeEdit *newEditor = new CodeEdit(wMainWindow);
    wTab->addTab(newEditor, name);
    wTab->setCurrentIndex(wTab->count()-1);
    openCentralWidgets.append(newEditor);
	   
}

void WDockManager::addDockWidget(int code)
{
    WDock *newWDock = new WDock(code, wMainWindow);
    openDockWidgets.append(newWDock);
}


WDock::WDock(int code, QMainWindow *mainWindow)
{
    switch (code)
    {
        case wListCode:
        {
            wDockWidget = new QDockWidget("List", mainWindow);
            wDockWidget->setAllowedAreas(Qt::LeftDockWidgetArea);
            mainWindow->addDockWidget(Qt::LeftDockWidgetArea, wDockWidget);
            QListWidget *newWidget = new QListWidget(wDockWidget);
            wDockWidget->setWidget(newWidget);
	    break;
        }
        case wListCode2:
        {
            wDockWidget = new QDockWidget("List2", mainWindow);
            wDockWidget->setAllowedAreas(Qt::RightDockWidgetArea);
            mainWindow->addDockWidget(Qt::RightDockWidgetArea, wDockWidget);
            QListWidget *newWidget = new QListWidget(wDockWidget);
            wDockWidget->setWidget(newWidget);
	    break;
        }
	case wListCode3:
        {
            wDockWidget = new QDockWidget("List3", mainWindow);
            wDockWidget->setAllowedAreas(Qt::BottomDockWidgetArea);
            mainWindow->addDockWidget(Qt::BottomDockWidgetArea, wDockWidget);
            QListWidget *newWidget = new QListWidget(wDockWidget);
            wDockWidget->setWidget(newWidget);
	    break;
        }
    }
}

WDock::~WDock()
{
    delete wDockWidget;
}
