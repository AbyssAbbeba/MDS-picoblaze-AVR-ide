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

#include <QMainWindow>
#include <QToolBar>
#include <QDockWidget>
#include <QListWidget>
#include <QTreeWidget>
#include <QIcon>
#include <QPixmap>
#include "../widgets/Editor/codeedit.h"
#include "../widgets/DockManager/wdockmanager.h"
#include "../widgets/CompileInfo/compileinfo.h"
#include "../widgets/Tools/Display/displaytool.h"
#include "../widgets/Tools/Convertor/convertortool.h"
#include "../project/project.h"
#include "../../compiler/CompilerThread.h"
#include "../../compiler/core/CompilerOptions.h"


class ProjectMan;
class WDockManager;
class CodeEdit;
class Project;


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

        void saveFile(CodeEdit *editor);
        WDockManager* getWDockManager();
        ProjectMan* getProjectMan();
        //bool dockWidgets;
        bool openProject(QString path);

    private slots:
        //slots connected to menu actions
        void newFile();
        void openFile();
        void addFile();
        void newAddFile();
        //void removeProjFile();
        void saveFile();
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
        //void showPlugins();
        //void writeToWCompileInfo(QString text);
        void createDockWidgets();
        void simProjectData();
        void tabifyDockWidgetSlot(QDockWidget *widget1, QDockWidget *widget2);
        void addDockWidgetSlot(Qt::DockWidgetArea area, QDockWidget *widget);
        void connectProjectSlot(Project *project);
        void highlightLine(QString file, int line, QColor *color);
        void setCentralByName(QString file);
        void scrollCentralToLine(int line);
        void setEditorReadOnly(bool readOnly);
        void addUntrackedFile(QString name, QString path);
        void openFilePath(QString path);
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
        void activeProjectChanged(int index);
        void enableSimActs();
        void disableSimActs();

    signals:
        void unhighlightSim();

    private:
        //init functions
        void createMenu();
        void createActions();
        void createShortcuts();
        void createToolbar();
        void createWelcome();
        //project manager
        ProjectMan *projectMan;
        //dock widgets manager
        WDockManager *wDockManager;

        //menus in main menu
        QMenu *fileMenu;
        QMenu *editMenu;
        QMenu *interfaceMenu;
        QMenu *projectMenu;
        QMenu *simulationMenu;
        QMenu *toolsMenu;
        QMenu *helpMenu;

        //toolbars
        QToolBar *fileToolBar;
        QToolBar *projectToolBar;
        QToolBar *simulationToolBar;

        //actions
        QAction *newAct;
        QAction *openAct;
        QAction *addAct;
        QAction *saveAct;
        QAction *saveAsAct;
        QAction *saveAllAct;
        QAction *exitAct;

        QAction *undoAct;
        QAction *redoAct;
        QAction *copyAct;
        QAction *cutAct;
        QAction *pasteAct;        

        QAction *interfaceConfigAct;
        QAction *pluginAct;

        QAction *newProjAct;
        QAction *newAddAct;
        //QAction *removeFileAct;
        QAction *openProjAct;
        QAction *saveProjAct;
        QAction *projectCompileAct;
        QAction *projectConfigAct;

        QAction *simulationFlowAct;
        QAction *simulationStepAct;
        QAction *simulationRunAct;
        QAction *simulationAnimateAct;
        QAction *simulationResetAct;
        QAction *simulationUnhighlightAct;

        QAction *toolDisassemblerAct;
        QAction *toolTranslatorAct;
        QAction *toolFileConvertAct;
        QAction *toolConvertorAct;
        QAction *toolDisplayAct;

        QAction *aboutAct;
        QAction *aboutQTAct;        
        QAction *helpActionAct;
        QAction *example1Act;

        QPixmap *pm_projNewAdd;
        QPixmap *pm_projNew;
        QPixmap *pm_projOpen;
        QPixmap *pm_projSave;
        QPixmap *pm_projComp;
        QPixmap *pm_simFlowStart;
        QPixmap *pm_simFlowStop;
        QPixmap *pm_simRun;
        QPixmap *pm_simAnimate;
        QPixmap *pm_simStep;
        QPixmap *pm_simReset;
        QPixmap *pm_simUnhighlight;
        QPixmap *pm_toolDis;
        QPixmap *pm_cross;

        QIcon *icon_projNewAdd;
        QIcon *icon_projNew;
        QIcon *icon_projOpen;
        QIcon *icon_projSave;
        QIcon *icon_projComp;
        QIcon *icon_simFlow;
        QIcon *icon_simRun;
        QIcon *icon_simAnimate;
        QIcon *icon_simStep;
        QIcon *icon_simReset;
        QIcon *icon_simUnhighlight;
        QIcon *icon_toolDis;
        

        // simulation status - start/stop
        bool simulationStatus;
        
        // sim run status
        bool simulationRunStatus;
        // sim animate status
        bool simulationAnimateStatus;

        bool projectTabConnected;
};



#endif
