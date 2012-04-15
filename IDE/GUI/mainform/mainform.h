/**
 * @brief
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
#include "../widgets/codeedit.h"
#include "../widgets/wdockmanager.h"
#include "project.h"


class ProjectMan;
class WDockManager;
class CodeEdit;

//hlavni okno programu
/**
 * @brief
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

    private slots:
        void newFile();
        void openFile();
        void addFile();
        void saveFile();
        void saveFileAs();
        void saveAll();
        void newProject();
        void openProject();
        void saveProject();
        void compileProject();

    private:
        void CreateMenu();
        void CreateActions();
        void CreateToolbar();
        void CreateDockWidgets();
        void CreateWelcome();

        ProjectMan *projectMan;

        WDockManager *wDockManager;


        QMenu *fileMenu;
    	QMenu *editMenu;
    	QMenu *interfaceMenu;
    	QMenu *projectMenu;
    	QMenu *toolsMenu;
    	QMenu *helpMenu;


        QToolBar *fileToolBar;
        QToolBar *projectToolBar;

        QAction *newAct;
        QAction *openAct;
        QAction *addAct;
        QAction *saveAct;
        QAction *saveAsAct;
        QAction *saveAllAct;
        QAction *newProjAct;
        QAction *openProjAct;
        QAction *saveProjAct;
        QAction *exitAct;

        QAction *undoAct;
        QAction *redoAct;
        QAction *copyAct;
        QAction *cutAct;
        QAction *pasteAct;        

        QAction *interfaceConfigAct;

        QAction *projectCompileAct;
        QAction *projectConfigAct;

        QAction *aboutAction;
        QAction *aboutQT;        
        QAction *helpAction;        
};



#endif
