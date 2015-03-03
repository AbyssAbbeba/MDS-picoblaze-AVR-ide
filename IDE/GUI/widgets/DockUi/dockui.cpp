#include <QtGui>
#include "dockui.h"
#include "../AsmMacroAnalyser/asmmacroanalyser.h"
#include "../BreakpointList/breakpointlist.h"
#include "../BookmarkList/bookmarklist.h"
#include "../PicoBlazeGrid/picoblazegrid.h"
#include "../CompileInfo/compileinfo.h"
#include "../HelpDockWidget/helpdockwidget.h"

#ifdef MDS_FEATURE_EXTERNAL_APPS
    #include "../ExtAppOutput/extappoutput.h"
#endif //MDS_FEATURE_EXTERNAL_APPS

#ifdef MDS_FEATURE_SIM_CALLWATCHER
    #include "../CallWatcher/callwatcher.h"
#endif //MDS_FEATURE_SIM_CALLWATCHER

#ifdef MDS_FEATURE_SIM_REGWATCHER
    #include "../RegWatcher/regwatcher.h"
#endif //MDS_FEATURE_SIM_REGWATCHER

#include "../../../simulators/SimControl/MCUSimControl.h"



DockUi::DockUi(QObject *parent)
    : QObject(parent)
{
    #ifdef MDS_FEATURE_SIM_REGWATCHER
        m_regWatcher = NULL;
    #endif
    m_bookmarkList = NULL;
    m_breakpointList = NULL;
    m_compileInfo = NULL;
    m_simulationInfo = NULL;
    m_asmMacroAnalyser = NULL;
    #ifdef MDS_FEATURE_EXTERNAL_APPS
        m_extAppOutput = NULL;
    #endif
    m_helpDockWidget = NULL;
    #ifdef MDS_FEATURE_SIM_CALLWATCHER
        m_callWatcher = NULL;
    #endif

    m_bottomHide = NULL;
    m_rightHide = NULL;

    m_bottomTabs = NULL;
    m_rightTabs = NULL;
}


DockUi::~DockUi()
{
    delete m_bottomTabs;
    delete m_rightTabs;
}


void DockUi::createWidgets(MCUSimControl *simControl)
{
    m_bottomTabs = new QTabWidget(0);
    m_rightTabs = new QTabWidget(0);

    m_rightTabs->setMaximumWidth(600);
    m_rightTabs->setMinimumWidth(320);

    m_bottomTabs->setMaximumHeight(600);
    m_bottomTabs->setMinimumHeight(300);

    #ifdef MDS_FEATURE_SIM_REGWATCHER
        m_regWatcher = new RegWatcher(m_rightTabs, simControl);
    #endif
    m_bookmarkList = new BookmarkList(m_rightTabs);
    m_breakpointList = new BreakpointList(m_rightTabs);
    m_compileInfo = new CompileInfo(m_bottomTabs);
    m_simulationInfo = new PicoBlazeGrid(m_bottomTabs, simControl);
    m_asmMacroAnalyser = new AsmMacroAnalyser(m_rightTabs);
    #ifdef MDS_FEATURE_EXTERNAL_APPS
        m_extAppOutput = new ExtAppOutput(m_bottomTabs);
    #endif
    m_helpDockWidget = new HelpDockWidget(m_rightTabs);
    #ifdef MDS_FEATURE_SIM_CALLWATCHER
        m_callWatcher = new CallWatcher(m_rightTabs, simControl);
    #endif

    m_bottomHide = new QWidget(m_bottomTabs);
    m_rightHide = new QWidget(m_rightTabs);

    m_bottomTabs->addTab(m_compileInfo, "Compiler Messages");
    m_bottomTabs->addTab(m_simulationInfo, "Simulator");
    #ifdef MDS_FEATURE_EXTERNAL_APPS
        m_bottomTabs->addTab(m_extAppOutput, "External Applications");
    #endif
    m_bottomTabs->addTab(m_bottomHide, "Hide");

    {
        QPixmap pixmap(":resources/icons/messages.png");
        m_bottomTabs->setTabIcon(0, QIcon(pixmap));
        m_bottomTabs->setTabToolTip(0,"Compiler Messages");
    }
    {
        QPixmap pixmap(":resources/icons/processor.png");
        m_bottomTabs->setTabIcon(1, QIcon(pixmap));
        m_bottomTabs->setTabToolTip(1,"Simulator");
    }
    #ifdef MDS_FEATURE_EXTERNAL_APPS
        {
            QPixmap pixmap(":resources/icons/application_xp_terminal.png");
            m_bottomTabs->setTabIcon(2, QIcon(pixmap));
            m_bottomTabs->setTabToolTip(2,"External Applications");
        }
    #endif
    {
        QPixmap pixmap(":resources/icons/bullet_arrow_down.png");
        m_bottomTabs->setTabIcon(3, QIcon(pixmap));
        m_bottomTabs->setTabToolTip(3,"Hide");
    }
    
    m_rightTabs->addTab(m_breakpointList, "");
    m_rightTabs->addTab(m_bookmarkList, "");
    m_rightTabs->addTab(m_asmMacroAnalyser, "");
    #ifdef MDS_FEATURE_SIM_REGWATCHER
        m_rightTabs->addTab(m_regWatcher, "");
    #endif
    #ifdef MDS_FEATURE_SIM_CALLWATCHER
        m_rightTabs->addTab(m_callWatcher, "");
    #endif
    m_rightTabs->addTab(m_helpDockWidget, "");
    m_rightTabs->addTab(m_rightHide, "");
    m_rightTabs->setTabPosition(QTabWidget::East);

    {
        QPixmap pixmap(":resources/icons/breakpoint.png");
        QMatrix rm;
        rm.rotate(-90);
        pixmap = pixmap.transformed(rm);
        m_rightTabs->setTabIcon(0, QIcon(pixmap));
        m_rightTabs->setTabToolTip(0,"Breakpoints");
    }
    {
        QPixmap pixmap(":resources/icons/bullet_star.png");
        QMatrix rm;
        rm.rotate(-90);
        pixmap = pixmap.transformed(rm);
        m_rightTabs->setTabIcon(1, QIcon(pixmap));
        m_rightTabs->setTabToolTip(1,"Bookmarks");
    }
    {
        QPixmap pixmap(":resources/icons/brick.png");
        QMatrix rm;
        rm.rotate(-90);
        pixmap = pixmap.transformed(rm);
        m_rightTabs->setTabIcon(2, QIcon(pixmap));
        m_rightTabs->setTabToolTip(2,"Macros");
    }
    {
        QPixmap pixmap(":resources/icons/reg_watch.png");
        QMatrix rm;
        rm.rotate(-90);
        pixmap = pixmap.transformed(rm);
        m_rightTabs->setTabIcon(3, QIcon(pixmap));
        m_rightTabs->setTabToolTip(3,"Reg Watcher");
    }
    {
        QPixmap pixmap(":resources/icons/door_in.png");
        QMatrix rm;
        rm.rotate(-90);
        pixmap = pixmap.transformed(rm);
        m_rightTabs->setTabIcon(4, QIcon(pixmap));
        m_rightTabs->setTabToolTip(4,"Call Watcher");
    }
    {
        QPixmap pixmap(":resources/icons/help.png");
        QMatrix rm;
        rm.rotate(-90);
        pixmap = pixmap.transformed(rm);
        m_rightTabs->setTabIcon(5, QIcon(pixmap));
        m_rightTabs->setTabToolTip(5,"Help");
    }
    {
        QPixmap pixmap(":resources/icons/bullet_arrow_right.png");
        QMatrix rm;
        rm.rotate(-90);
        pixmap = pixmap.transformed(rm);
        m_rightTabs->setTabIcon(6, QIcon(pixmap));
        m_rightTabs->setTabToolTip(6,"Hide");
    }

    connect(m_bottomTabs,
            SIGNAL(currentChanged(int)),
            this,
            SLOT(handleBottomHide())
           );

    connect(m_rightTabs,
            SIGNAL(currentChanged(int)),
            this,
            SLOT(handleRightHide())
           );

}



void DockUi::setBottomAreaToExtAppOutput()
{
    #ifdef MDS_FEATURE_EXTERNAL_APPS
        m_bottomTabs->setCurrentIndex(2);
    #endif
}


void DockUi::setBottomAreaToSimulationInfo()
{
    m_bottomTabs->setCurrentIndex(1);
}



void DockUi::setBottomAreaToCompilerInfo()
{
    m_bottomTabs->setCurrentIndex(0);
}


void DockUi::handleRightHide()
{
    if (m_rightTabs->currentWidget() == m_rightHide)
    {
        for (int i = 0; i < m_rightTabs->count(); i++)
        {
            m_rightTabs->widget(i)->setFixedWidth(0);
        }
        QTabBar *tabBar = m_rightTabs->findChild<QTabBar *>(QLatin1String("qt_tabwidget_tabbar"));
        m_rightTabs->setFixedWidth(tabBar->width());
    }
    else
    {
        for (int i = 0; i < m_rightTabs->count(); i++)
        {
            m_rightTabs->widget(i)->setMinimumWidth(320);
            m_rightTabs->widget(i)->setMaximumWidth(600);
        }
        m_rightTabs->setMaximumWidth(600);
        m_rightTabs->setMinimumWidth(320);
    }
}


void DockUi::handleBottomHide()
{
    if (m_bottomTabs->currentWidget() == m_bottomHide)
    {
        for (int i = 0; i < m_bottomTabs->count(); i++)
        {
            m_bottomTabs->widget(i)->setFixedHeight(0);
        }
        QTabBar *tabBar = m_bottomTabs->findChild<QTabBar *>(QLatin1String("qt_tabwidget_tabbar"));
        m_bottomTabs->setFixedHeight(tabBar->height());
    }
    else
    {
        for (int i = 0; i < m_bottomTabs->count(); i++)
        {
            m_bottomTabs->widget(i)->setMinimumHeight(300);
            m_bottomTabs->widget(i)->setMaximumHeight(600);
        }
        m_bottomTabs->setMaximumHeight(600);
        m_bottomTabs->setMinimumHeight(300);
    }
}