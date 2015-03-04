#ifndef DOCKUI_H
#define DOCKUI_H

#include <QObject>
#include "../../mds.h"


#ifdef MDS_FEATURE_SIM_CALLWATCHER
    class CallWatcher;
#endif
#ifdef MDS_FEATURE_SIM_REGWATCHER
    class RegWatcher;
#endif
class QTabWidget;
class BookmarkList;
class BreakpointList;
class CompileInfo;
class PicoBlazeGrid;
class AsmMacroAnalyser;
#ifdef MDS_FEATURE_EXTERNAL_APPS
    class ExtAppOutput;
#endif
class HelpDockWidget;
class QWidget;
class Project;
class MCUSimControl;


class DockUi : public QObject
{
    Q_OBJECT
    public:
        DockUi(QObject *parent);
        ~DockUi();
        void createWidgets(MCUSimControl *simControl);
        void setBottomAreaToExtAppOutput();
        void setBottomAreaToSimulationInfo();
        void setBottomAreaToCompilerInfo();

        #ifdef MDS_FEATURE_SIM_REGWATCHER
            RegWatcher *m_regWatcher;
        #endif
        BookmarkList *m_bookmarkList;
        BreakpointList *m_breakpointList;
        CompileInfo *m_compileInfo;
        PicoBlazeGrid *m_simulationInfo;
        AsmMacroAnalyser *m_asmMacroAnalyser;
        #ifdef MDS_FEATURE_EXTERNAL_APPS
            ExtAppOutput *m_extAppOutput;
        #endif
        HelpDockWidget *m_helpDockWidget;
        #ifdef MDS_FEATURE_SIM_CALLWATCHER
            CallWatcher *m_callWatcher;
        #endif
        
        QWidget *m_bottomHide;
        QWidget *m_rightHide;
        
        QTabWidget *m_bottomTabs;
        QTabWidget *m_rightTabs;

    private slots:
        void handleRightHide();
        void handleBottomHide();

    private:
        bool m_bottomHidden;
        bool m_rightHidden;
};




#endif