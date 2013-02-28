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
#include "../widgets/Tools/Display/displaytool.h"
#include "../widgets/Tools/Convertor/convertortool.h"
#include "project.h"


class ProjectMan;
class WDockManager;
class CodeEdit;


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

        void openFilePath(QString path);
        void saveFile(CodeEdit *editor);
        WDockManager* getWDockManager();
        void CreateDockWidgets();
        ProjectMan* getProjectMan();
        bool dockWidgets;
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
        void toolConvertor();
        void toolDisplay();
        void exampleOpen();
        void showPlugins();

    private:
        //init functions
        void CreateMenu();
        void CreateActions();
        void CreateToolbar();
        void CreateWelcome();
        //project manager
        ProjectMan *projectMan;
        //dock widgets manager
        WDockManager *wDockManager;

        //menus in main menu
        QMenu *fileMenu;
        QMenu *editMenu;
        QMenu *interfaceMenu;
        QMenu *projectMenu;
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
