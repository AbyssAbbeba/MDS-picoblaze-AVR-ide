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
#include "../errordialog/errordlg.h"
#include "../enums/enums.h"


//class MainForm;
class CodeEdit;
class ProjectTree;

//simulation handle
class McuMemoryView;
class RegDisplayList;
class MCUSimControl;
class McuSimCfgMgr;

class Project;


/*typedef enum LangType {
    LANG_ASM = 0, LANG_C
} LangType;*/


/**
 * @brief Manager of Projects.
 * @ingroup GUI
 * @class ProjectMan
 */
class ProjectMan : public QObject
{
    Q_OBJECT
    public:
        ProjectMan(QWidget *parent);
        void addFile(QString path, QString name);
        void addUntrackedFile(QString path, QString name);
        void addProject(QString name, QString path, QString architecture, LangType langType, QFile *file);
        void addUntrackedProject();
        void openProject(QFile *file);
        void setActive(Project *activePrj);
        bool isActiveProject(Project *project);
        Project* getActive();
        void createActiveMakefile();

    signals:
        void connectProject(Project *project);
        void addDockWidget(Qt::DockWidgetArea area, QDockWidget* dockWidget);
        void tabifyDockWidget(QWidget *first, QWidget *second);

    private:
        int projectCount;
        QList<Project*> openProjects;
        Project *activeProject;
        Project *untrackedProject;
    
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
        Project(QFile *file, ProjectMan *parent);
        Project(ProjectMan *parent);
        //konstruktor pro prazdny projekt
        Project(QString name, QString path, QString arch, LangType langType, QFile *file, ProjectMan *parent);
        ~Project();

        void addFile(QString path, QString name);
        void setupSim();
        bool start();
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
        //
        QList<bool> compileOpt;

        int errorFlag;

        //simulation handle
        McuMemoryView *m_mcuMemoryView;
        RegDisplayList *m_regList;
        MCUSimControl *m_simControlUnit;

    private slots:
        void setActive();
        void openItem();
        void openUntrackedItem();
        void emitFileCount();
        void startCfgDlgCore();

    signals:
        void highlightLine(QString file, int line, QColor *color, QColor *origColor);
        void addUntrackedFile(QString name, QString path);
        //void addDockWidget(Qt::DockWidgetArea area, QDockWidget* dockWidget);
        void openFilePath(QString path);
        void setEditorReadOnly(bool readOnly);
        //void setCentralChanged();
        void fileCountSignal(int fileCount);
        void startConfig(Project *project);
        
    private:
        ProjectMan *parentManager;
        int prevLine;
        int prevLine2;
        int prevLine3;
        QColor *currLineColor;
        QColor *prevLineColor;
        QColor *prevLine2Color;
        QColor *origPrevLineCol;
        QColor *origPrevLine2Col;
        QColor *origCurrLineCol;
        
       
};

#endif
