#include <QtGui>
#include "wdockmanager.h"

WDockManager::WDockManager(MainForm *mainWindow)
{
    wMainWindow = mainWindow;
    wTab = new QTabWidget(wMainWindow);
    wTab->setTabsClosable(true);
    wTab->setMovable(true);
    wRight = NULL;
    wLeft = NULL;
    wBottom = NULL;
    connect(wTab, SIGNAL(tabCloseRequested(int)),this, SLOT(closeTab(int)));
    wMainWindow->setCentralWidget(wTab);
}



void WDockManager::closeTab(int index)
{
    wMainWindow->saveFile((CodeEdit*)wTab->widget(index));
    wTab->removeTab(index);
}



void WDockManager::setTabChanged()
{
    ((CodeEdit*)(wTab->currentWidget()))->setChanged();;
}


void WDockManager::setTabSaved()
{
    ((CodeEdit*)wTab->currentWidget())->setSaved();
}



CodeEdit* WDockManager::getCentralWidget()
{
    return (CodeEdit*)wTab->currentWidget();
}



CodeEdit* WDockManager::getTabWidget(int index)
{
    return (CodeEdit*)wTab->widget(index);
}



int WDockManager::getTabCount()
{
    return wTab->count();
}



QString WDockManager::getCentralName()
{
    return ((CodeEdit*)wTab->currentWidget())->getName();
}

QString WDockManager::getCentralPath()
{
    return ((CodeEdit*)wTab->currentWidget())->getPath();
}


void WDockManager::setCentralName(QString wName)
{
    ((CodeEdit*)wTab->currentWidget())->setName(wName);
    wTab->setTabText(wTab->currentIndex(), wName);
}


void WDockManager::setCentralPath(QString wPath)
{
    ((CodeEdit*)wTab->currentWidget())->setPath(wPath);
}



void WDockManager::addCentralWidget(QString wName, QString wPath)
{
    CodeEdit *newEditor = new CodeEdit(wTab, wName, wPath);
    wTab->addTab(newEditor, wName);
    wTab->setCurrentIndex(wTab->count()-1);
    //add tab tooltip with path
    openCentralWidgets.append(newEditor);
}

void WDockManager::addDockWidget(int code)
{
    WDock *newWDock = new WDock(code, wMainWindow);
    if (getDockWidgetArea(newWDock->getArea())!=NULL)
        wMainWindow->tabifyDockWidget(getDockWidgetArea(newWDock->getArea()), newWDock->getQDockWidget());
    openDockWidgets.append(newWDock);
}


QDockWidget* WDockManager::getDockWidget(int code)
{
    QList<WDock*>::iterator i;
    for (i = openDockWidgets.begin(); i != openDockWidgets.end(); i++)
        if ((*i)->cmpCode(code) == true)
            return (*i)->getQDockWidget();
     return NULL;
}


QDockWidget* WDockManager::getDockWidgetArea(int area)
{
    QList<WDock*>::iterator i;
    for (i = openDockWidgets.begin(); i != openDockWidgets.end(); i++)
        if ((*i)->cmpArea(area) == true)
            return (*i)->getQDockWidget();
     return NULL;
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
            area = 0;
            wDockWidget->setWidget(newWidget);
	    break;
        }
        case wListCode2:
        {
            wDockWidget = new QDockWidget("List2", mainWindow);
            wDockWidget->setAllowedAreas(Qt::RightDockWidgetArea);
            mainWindow->addDockWidget(Qt::RightDockWidgetArea, wDockWidget);
            QListWidget *newWidget = new QListWidget(wDockWidget);
            area = 1;
            wDockWidget->setWidget(newWidget);
	    break;
        }
	case wCompileInfo:
        {
            wDockWidget = new QDockWidget("Compiler Info", mainWindow);
            wDockWidget->setAllowedAreas(Qt::BottomDockWidgetArea);
            mainWindow->addDockWidget(Qt::BottomDockWidgetArea, wDockWidget);
            QPlainTextEdit *newWidget = new QPlainTextEdit(wDockWidget);
            newWidget->setFont(QFont("Andale Mono", 10));
            area = 2;
            newWidget->setReadOnly(true);
            wDockWidget->setWidget(newWidget);
	    break;
        }
        case wHexEdit:
        {
            wDockWidget = new QDockWidget("Simulation Info", mainWindow);
            wDockWidget->setAllowedAreas(Qt::BottomDockWidgetArea);
            mainWindow->addDockWidget(Qt::BottomDockWidgetArea, wDockWidget);
            //v budoucnu nahradit 20 na constCounterSize v konstruktoru
            //i 16 za promennou podle nastaveni
            HexEdit *newWidget = new HexEdit(wDockWidget, false, 20, 16);
            area = 2;
            wDockWidget->setWidget(newWidget);
            wDockWidget->setMaximumWidth(450);
	    break;
        }
    }
    this->code=code;
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
