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
    
    m_bottomHidden = false;
    m_rightHidden = false;
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
    {
        QPixmap pixmap(":resources/icons/messages.png");
        m_bottomTabs->setTabIcon(m_bottomTabs->count()-1, QIcon(pixmap));
        m_bottomTabs->setTabToolTip(m_bottomTabs->count()-1,"Compiler Messages");
    }
    m_bottomTabs->addTab(m_simulationInfo, "Simulator");
    {
        QPixmap pixmap(":resources/icons/processor.png");
        m_bottomTabs->setTabIcon(m_bottomTabs->count()-1, QIcon(pixmap));
        m_bottomTabs->setTabToolTip(m_bottomTabs->count()-1,"Simulator");
    }
    #ifdef MDS_FEATURE_EXTERNAL_APPS
        m_bottomTabs->addTab(m_extAppOutput, "External Applications");
        {
            QPixmap pixmap(":resources/icons/application_xp_terminal.png");
            m_bottomTabs->setTabIcon(m_bottomTabs->count()-1, QIcon(pixmap));
            m_bottomTabs->setTabToolTip(m_bottomTabs->count()-1,"External Applications");
        }
    #endif
    m_bottomTabs->addTab(m_bottomHide, "Hide");
    {
        QPixmap pixmap(":resources/icons/bullet_arrow_down.png");
        m_bottomTabs->setTabIcon(m_bottomTabs->count()-1, QIcon(pixmap));
        m_bottomTabs->setTabToolTip(m_bottomTabs->count()-1,"Hide");
    }


    
    m_rightTabs->addTab(m_breakpointList, "");
    {
        QPixmap pixmap(":resources/icons/breakpoint.png");
        QMatrix rm;
        rm.rotate(-90);
        pixmap = pixmap.transformed(rm);
        m_rightTabs->setTabIcon(m_rightTabs->count()-1, QIcon(pixmap));
        m_rightTabs->setTabToolTip(m_rightTabs->count()-1,"Breakpoints");
    }
    m_rightTabs->addTab(m_bookmarkList, "");
    {
        QPixmap pixmap(":resources/icons/bullet_star.png");
        QMatrix rm;
        rm.rotate(-90);
        pixmap = pixmap.transformed(rm);
        m_rightTabs->setTabIcon(m_rightTabs->count()-1, QIcon(pixmap));
        m_rightTabs->setTabToolTip(m_rightTabs->count()-1,"Bookmarks");
    }
    m_rightTabs->addTab(m_asmMacroAnalyser, "");
    {
        QPixmap pixmap(":resources/icons/brick.png");
        QMatrix rm;
        rm.rotate(-90);
        pixmap = pixmap.transformed(rm);
        m_rightTabs->setTabIcon(m_rightTabs->count()-1, QIcon(pixmap));
        m_rightTabs->setTabToolTip(m_rightTabs->count()-1,"Macros");
    }
    #ifdef MDS_FEATURE_SIM_REGWATCHER
        m_rightTabs->addTab(m_regWatcher, "");
        {
            QPixmap pixmap(":resources/icons/reg_watch.png");
            QMatrix rm;
            rm.rotate(-90);
            pixmap = pixmap.transformed(rm);
            m_rightTabs->setTabIcon(m_rightTabs->count()-1, QIcon(pixmap));
            m_rightTabs->setTabToolTip(m_rightTabs->count()-1,"Reg Watcher");
        }
    #endif
    #ifdef MDS_FEATURE_SIM_CALLWATCHER
        m_rightTabs->addTab(m_callWatcher, "");
        {
            QPixmap pixmap(":resources/icons/door_in.png");
            QMatrix rm;
            rm.rotate(-90);
            pixmap = pixmap.transformed(rm);
            m_rightTabs->setTabIcon(m_rightTabs->count()-1, QIcon(pixmap));
            m_rightTabs->setTabToolTip(m_rightTabs->count()-1,"Call Watcher");
        }
    #endif
    m_rightTabs->addTab(m_helpDockWidget, "");
    {
        QPixmap pixmap(":resources/icons/help.png");
        QMatrix rm;
        rm.rotate(-90);
        pixmap = pixmap.transformed(rm);
        m_rightTabs->setTabIcon(m_rightTabs->count()-1, QIcon(pixmap));
        m_rightTabs->setTabToolTip(m_rightTabs->count()-1,"Help");
    }
    m_rightTabs->addTab(m_rightHide, "");
    {
        QPixmap pixmap(":resources/icons/bullet_arrow_right.png");
        QMatrix rm;
        rm.rotate(-90);
        pixmap = pixmap.transformed(rm);
        m_rightTabs->setTabIcon(m_rightTabs->count()-1, QIcon(pixmap));
        m_rightTabs->setTabToolTip(m_rightTabs->count()-1,"Hide");
    }
    m_rightTabs->setTabPosition(QTabWidget::East);

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
    connect(m_asmMacroAnalyser,
            SIGNAL(macroClicked(QString, int)),
            this,
            SIGNAL(scrollToLine(QString, int))
           );
    connect(m_breakpointList,
            SIGNAL(breakpointClicked(QString, int)),
            this,
            SIGNAL(scrollToLine(QString, int))
           );
    connect(m_bookmarkList,
            SIGNAL(bookmarkClicked(QString, int)),
            this,
            SIGNAL(scrollToLine(QString, int))
           );
    connect(m_helpDockWidget,
            SIGNAL(showHelpContent(const QUrl&)),
            this,
            SIGNAL(showHelpContent(const QUrl&))
           );
    connect(m_compileInfo,
            SIGNAL(errorClicked(QString, int)),
            this,
            SIGNAL(scrollToLine(QString, int))
           );
    connect(m_simulationInfo,
            SIGNAL(stopSimSig()),
            this,
            SIGNAL(stopSimSig())
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
        m_rightHidden = true;
    }
    else if (true == m_rightHidden)
    {
        for (int i = 0; i < m_rightTabs->count(); i++)
        {
            m_rightTabs->widget(i)->setMinimumWidth(320);
            m_rightTabs->widget(i)->setMaximumWidth(600);
        }
        m_rightTabs->setMaximumWidth(600);
        m_rightTabs->setMinimumWidth(320);
        m_rightHidden = false;
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
        m_bottomHidden = true;
    }
    else if (true == m_bottomHidden)
    {
        for (int i = 0; i < m_bottomTabs->count(); i++)
        {
            m_bottomTabs->widget(i)->setMinimumHeight(300);
            m_bottomTabs->widget(i)->setMaximumHeight(600);
        }
        m_bottomTabs->setMaximumHeight(600);
        m_bottomTabs->setMinimumHeight(300);
        m_bottomHidden = false;
    }
}