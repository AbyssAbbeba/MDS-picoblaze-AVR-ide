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


//class QMenu;
//class QAction;
class ProjectMan;

//hlavni okno programu
class MainForm : public QMainWindow
{
    Q_OBJECT
    public:
        MainForm();

        void openFilePath(QString path);

    private slots:
        void newFile();
        void openFile();
        void addFile();
        void saveFile();
        void saveFile(CodeEdit *editor);
        void saveFileAs();
        //void saveFileAs(CodeEdit *editor);
        void saveAll();
        void newProject();
        void openProject();
        void saveProject();

    private:
        void CreateMenu();
        void CreateActions();
        void CreateToolbar();
        void CreateDockWidgets();

        ProjectMan *projectMan;

        WDockManager *wDockManager;


        QMenu *fileMenu;
    	QMenu *editMenu;
    	QMenu *viewMenu;
    	QMenu *projectMenu;
    	QMenu *toolsMenu;
    	QMenu *helpMenu;


        QToolBar *toolBar;

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

        QAction *projectConfigAct;

        QAction *aboutAction;
        QAction *aboutQT;        
        QAction *helpAction;        
};



#endif
