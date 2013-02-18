/**
 * @brief Header for ProjectMan class and Project class
 * C++ Interface: ...
 *
 * ...
 *
 * Copyright: See COPYING file that comes with this distribution.
 *
 * @author Erik Chalupa <xchalu10@stud.fit.vutbr.cz>, (C) 2012
 * @ingroup GUI
 * @file project.h
 */



#ifndef PROJECT_H
#define PROJECT_H

#define ERR_OK 500
#define ERR_ASSIGN 501
#define ERR_CONTENT 502

#include <QString>
#include <QList>
#include <QTreeWidget>
#include <QMenu>
#include <QAction>
#include <QtXml>
#include "mainform.h"
#include "../dialogs/errordlg.h"


class MainForm;
class CodeEdit;
class ProjectTree;

//simulation handle
class McuMemoryView;
class RegDisplayList;
class MCUSimControl;
class McuSimCfgMgr;

class Project;


typedef enum LangType {
    LANG_ASM = 0, LANG_C
} LangType;


/**
 * @brief Manager of Projects.
 * @ingroup GUI
 * @class ProjectMan
 */
class ProjectMan
{
    public:
        ProjectMan(MainForm *mainWindow);
        void addFile(QString path, QString name);
        void addProject(QString name, QString path, QString architecture, LangType langType, QFile *file);
        void openProject(QFile *file);
        void setActive(Project *activePrj);
        bool isActiveProject(Project *project);
        Project* getActive();
        void createActiveMakefile();

        MainForm *mainWindow;


    private:
        int projectCount;
        QList<Project*> openProjects;
        Project *activeProject;
    
};



/**
 * @brief Project class. Manages files and simulation.
 * @ingroup GUI
 * @class Project
 */
class Project : public QObject
{
    Q_OBJECT
    
    public:
        //konstruktor pro otevirani projektu
        Project(QFile *file, MainForm* mainWindow, ProjectMan *parent);
        //konstruktor pro prazdny projekt
        Project(QString name, QString path, QString arch, LangType langType, MainForm* mainWindow, QFile *file, ProjectMan *parent);
        ~Project();

        void addFile(QString path, QString name);
        void setupSim();
        void start();
        void stop();
        void reset();
        void step();
        void run();
        MCUSimControl* getSimControl();
        void setMainFile(QString path, QString name);
        void removeFile(QString path, QString name);

        QDockWidget *prjDockWidget;
        ProjectTree *prjTreeWidget;

        QString prjName;
        QString prjPath;
        int fileCount;

        LangType langType;
        QString architecture;
        QString mainFileName;
        QString mainFilePath;
        QList<QString> fileNames;
        QList<QString> filePaths;

        int errorFlag;

        //simulation handle
        McuMemoryView *m_mcuMemoryView;
        RegDisplayList *m_regList;
        MCUSimControl *m_simControlUnit;

    private slots:
        void setActive();
        void openItem();

    private:
        ProjectMan *parentManager;
        MainForm *parentWindow;
        
       
};

#endif
