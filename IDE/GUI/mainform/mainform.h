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
 * @file mainform.h
 */


#ifndef MAINFORM_H
#define MAINFORM_H

// Feature set configuration.
#include "../../mds.h"

#include <QMainWindow>
#include <QFileSystemWatcher>
#include <QProcess>
#include "../../compiler/core/CompilerOptions.h"
#include "../../compiler/CompilerThread.h"
#include "../compatibilitymode/compatibilitymode.h"
//#include <QToolBar>
//#include <QDockWidget>
//#include <QListWidget>
//#include <QTreeWidget>
//#include <QIcon>
//#include <QPixmap>


class ProjectMan;
class WDockManager;
class CodeEdit;
class Project;
class QToolBar;
class QAction;
class QDockWidget;
class QListWidget;
class QTreeWidget;
class QIcon;
class QPixmap;
class SaveDialog;
class QSignalMapper;

/*
#ifdef MDS_FEATURE_FILECONVERTER
#endif
#ifdef MDS_FEATURE_DISASSEMBLER
#endif
#ifdef MDS_FEATURE_TRANSLATOR
#endif
#ifdef MDS_FEATURE_LICENCE_CERTIFICATE
#endif
#ifdef MDS_FEATURE_CONVERTER_TOOL
#endif
#ifdef MDS_FEATURE_LOOP_GENERATOR
#endif
#ifdef MDS_FEATURE_8_SEGMENT_EDITOR
#endif
#ifdef MDS_FEATURE_SIM_LED_PANEL
#endif
#ifdef MDS_FEATURE_SIM_7_SEGMENT
#endif
*/

/**
 * @brief MainForm uses QMainWindow and adds functions for file streams, tools/simulation executing and other basic stuff
 * @ingroup GUI
 * @class MainForm
 */
class MainForm : public QMainWindow
{
    Q_OBJECT
    public:
        MainForm();
        ~MainForm();

        WDockManager* getWDockManager();
        ProjectMan* getProjectMan();
        //bool dockWidgets;
        //void emitSessionRestorationSignal();
        void createWelcome();

        int startHeight;

    public slots:
        void openFilePath(QString path, QString parentProjectName = "");
        void sessionRestorationSlot();
        bool openProject(QString path);

    private slots:
        //slots connected to menu actions
        void newFile();
        void openFile();
        void addFile();
        void newAddFile();
        //void removeProjFile();
        void saveFile();
        void saveFile(CodeEdit *editor, bool ask = false);
        void saveFileAs();
        void saveAll();
        void newProject();
        void openProject();
        void saveProject();
        void compileProject();
        void simulationStep();
        void simulationRunHandle();
        void simulationAnimateHandle();
        void simulationFlowHandle();
        void simulationReset();
        void toolDisassemble();
        void toolTranslate();
        void toolFileConvert();
        void toolConvertor();
        void toolDisplay();
        void exampleOpen();
        void undoSlot();
        void redoSlot();
        void cutSlot();
        void copySlot();
        void pasteSlot();
        void selectAllSlot();
        void deselectSlot();
        void findSlot();
        void findNextSlot();
        void findPrevSlot();
        void replaceSlot();
        void jmpToLineSlot();
        void commentSlot();
        void deleteCommentSlot();
        void jmpToBookmarkNextSlot();
        void jmpToBookmarkPrevSlot();
        //void showPlugins();
        //void writeToWCompileInfo(QString text);
        void createDockWidgets();
        void simProjectData();
        void tabifyDockWidgetSlot(QDockWidget *widget1, QDockWidget *widget2);
        void addDockWidgetSlot(Qt::DockWidgetArea area, QDockWidget *widget);
        void connectProjectSlot(Project *project);
        void highlightLine(QString file, int line, QColor *color);
        void simHighlightLines(std::vector<std::pair<const std::string *, unsigned int>> curr,
                               std::vector<std::pair<const std::string *, unsigned int>> prev,
                               std::vector<std::pair<const std::string *, unsigned int>> prev2,
                               QList<QColor*> colors);
        void setCentralByName(QString file);
        void scrollCentralToLine(int line);
        void setEditorReadOnly(bool readOnly);
        void addUntrackedFile(QString name, QString path);
        void reloadCompileInfo(const std::string &text, CompilerBase::MessageType type);
        void compilationFinished(bool success);
        void startProjectConfig(Project *project);
        void help();
        void disassembleOutput(std::vector<std::string> text);
        void translatorOutput(std::vector<std::string> & text);
        void translatorOutput(const std::vector<std::pair<unsigned int, std::string>> & text);
        void unhighlight();
        void projectConfig();
        void interfaceConfig();
        void projectOpened();
        void stopSimSlot();
        void clockChangedSlot(double clock, int clockMult);
        void activeProjectChanged(int index);
        void enableSimActs();
        void disableSimActs();
        void changeProjectFamily(QString family);
        void closeProject();
        void manageBreakpointEmit(QString file, int line);
        void breakpointsAddLines(QString file, int line, int linesAdded);
        void breakpointsRemoveLines(QString file, int line, int linesRemoved);
        void manageBookmarkEmit(QString file, int line);
        void bookmarksAddLines(QString file, int line, int linesAdded);
        void bookmarksRemoveLines(QString file, int line, int linesRemoved);
        //void manageBreakpointRemove(QString file, int line);
        void pauseSimulation();
        void scrollToFileLine(QString file, int line);
        void requestMacrosCodeEdits();
        void breakpointActHandle();
        void disableBreakpointsHandle();
        void manageLicense();
        void loopGen();
        void about();
        void refreshProjectTree();
        //void refreshProjectTree(int index);
        void showWebSite(QAction *action);
        void simLeds();
        void sim7Seg();
        void simSwitch();
        void simPortLogger();
        void userGuide();
        void openRecentFileSlot(QAction *action);
        void openRecentProjectSlot(QAction *action);
        void welcomeDialog();
        void saveOnClose(QString path);
        void shortcutCloseTab();
        void shortcutChangeTabLeft();
        void shortcutChangeTabRight();
        void reloadTabIcons();
        void fileClosed(QString path);
        void fileChanged(QString path);
        void reloadFile(QString path);
        void reloadCurrentFile();
        void setCentralUntitled(bool untracked);
        void reloadExternalApps();
        void startExtApp1();
        void startExtApp2();
        void startExtApp3();
        void startExtApp(int processNumber);
        void errorExtApp(int processNumber);
        void finishedExtApp(int processNumber);
        void stderrExtApp(int processNumber);
        void stdoutExtApp(int processNumber);

    signals:
        void unhighlightSim();
        void mtblCompiled(QString path);
        void provideMacroCodeEdits(QList<CodeEdit*> editList);
        //void sessionRestorationSignal();

    private:
        //init functions
        void createMenu();
        void createActions();
        void createShortcuts();
        void createToolbar();
        QString translateBeforeCompilation(QString path);
        
        //project manager
        ProjectMan *m_projectMan;
        //dock widgets manager
        WDockManager *m_wDockManager;

        //menus in main menu
        QMenu *fileMenu;
        QMenu *editMenu;
        QMenu *interfaceMenu;
        QMenu *projectMenu;
        QMenu *simulationMenu;
        QMenu *toolsMenu;
        QMenu *simToolsMenu;
        QMenu *helpMenu;

        QMenu *recentFilesMenu;
        QMenu *recentProjectsMenu;

        //toolbars
        QToolBar *fileToolBar;
        QToolBar *projectToolBar;
        QToolBar *simulationToolBar;
        QToolBar *m_toolToolBar;
        QToolBar *m_simtoolToolBar;
        QToolBar *m_externalAppsToolBar;
        QToolBar *m_helpToolBar;

        //actions
        //file menu
        QAction *newAct;
        QAction *openAct;
        //QAction *addAct;
        QAction *saveAct;
        QAction *saveAsAct;
        QAction *saveAllAct;
        QAction *reloadAct;
        QAction *closeFileAct;
        //QAction *closeAllAct;
        QAction *exitAct;

        //edit menu
        QAction *undoAct;
        QAction *redoAct;
        QAction *copyAct;
        QAction *cutAct;
        QAction *pasteAct;
        QAction *selectAllAct;
        QAction *deselectAct;
        QAction *findAct;
        QAction *findNextAct;
        QAction *findPreviousAct;
        QAction *replaceAct;
        QAction *jmpToLineAct;
        QAction *commentAct;
        QAction *deleteCommentAct;
        QAction *jmpToBookmarkNextAct;
        QAction *jmpToBookmarkPrevAct;

        QAction *interfaceConfigAct;
        QAction *pluginAct;

        QAction *newProjAct;
        QAction *newAddAct;
        //QAction *removeFileAct;
        QAction *openProjAct;
        QAction *saveProjAct;
        QAction *saveProjConfigAct;
        QAction *closeProjectAct;
        QAction *projectCompileAct;
        QAction *projectConfigAct;

        QAction *simulationFlowAct;
        QAction *simulationStepAct;
        QAction *simulationRunAct;
        QAction *simulationAnimateAct;
        QAction *simulationResetAct;
        QAction *simulationUnhighlightAct;
        QAction *simulationBreakpointAct;
        QAction *simulationDisableBreakpointsAct;

        QAction *toolDisassemblerAct;
        QAction *toolTranslatorAct;
        QAction *toolFileConvertAct;
        QAction *toolConvertorAct;
        QAction *toolDisplayAct;
        QAction *toolLoopGenAct;

        QAction *toolSimLedsAct;
        QAction *toolSim7SegAct;
        QAction *toolSimSwitchAct;
        QAction *toolSimLoggerAct;

        QAction *extAppAct[3];

        QAction *licenseAct;
        QAction *aboutAct;
        QAction *aboutQTAct;
        QAction *welcomeAct;
        QAction *helpAct;
        QAction *example1Act;

        QPixmap *pm_simFlowStart;
        QPixmap *pm_simFlowStop;
        QPixmap *pm_simRun;
        QPixmap *pm_simAnimate;
        QPixmap *pm_cross;

        QIcon *icon_simFlow;
        QIcon *icon_simRun;
        QIcon *icon_simAnimate;


        // simulation status - start/stop
        bool simulationStatus;

        // sim run status
        bool simulationRunStatus;
        // sim animate status
        bool simulationAnimateStatus;

        bool projectTabConnected;

        bool simulationRequest;

        bool simulationBreakpointsEnabled;

        bool simulationBreakpointsReload;

        QTabBar *projectTabs;

        CompilerOptions *options;

        QFileSystemWatcher m_fileWatcher;

        SaveDialog *m_reloadDlg;

        bool reloadDlgChange;

        CompatibilityMode m_compatibilityMode;

        QString m_lastDir;

        QProcess *m_procExtApps[3];

        QSignalMapper *m_finishedSignalMapper;
        QSignalMapper *m_errorSignalMapper;
        QSignalMapper *m_stderrSignalMapper;
        QSignalMapper *m_stdoutSignalMapper;

    protected:
        void closeEvent(QCloseEvent *event);
};



#endif
