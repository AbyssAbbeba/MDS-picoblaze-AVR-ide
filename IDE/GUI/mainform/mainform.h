/**
 * @brief Header for MainForm class
 * C++ Interface: ...
 *
 * ...
 *
 * Copyright: See COPYING file that comes with this distribution.
 *
 * @author Erik Chalupa <xchalu10@stud.fit.vutbr.cz>, (C) 2012
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
#include "../widgets/Editor/codeedit.h"
#include "../widgets/DockManager/wdockmanager.h"
#include "../widgets/CompileInfo/compileinfo.h"
#include "../widgets/Tools/Display/displaytool.h"
#include "../widgets/Tools/Convertor/convertortool.h"
#include "../project/project.h"
#include "../../compiler/core/CompilerThread.h"
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

        void saveFile(CodeEdit *editor);
        WDockManager* getWDockManager();
        ProjectMan* getProjectMan();
        //bool dockWidgets;
        void openProject(QString path);

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

        //simulation status - start/stop
        bool simulationStatus;      
};



#endif
