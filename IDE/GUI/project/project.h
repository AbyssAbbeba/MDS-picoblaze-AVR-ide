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

//#define ERR_OK 500
//#define ERR_ASSIGN 501
//#define ERR_CONTENT 502

#include <QString>
#include <QList>
#include <QTreeWidget>
#include <QMenu>
#include <QAction>
#include <QtXml>
#include <QStringList>
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
        QString addUntrackedFile(QString path, QString name);
        void addProject(QString name, QString path, QString architecture, LangType langType, QFile *file);
        void addUntrackedProject();
        void openProject(QFile *file);
        void setActive(Project *activePrj);
        void setActiveByIndex(int index);
        bool isActiveProject(Project *project);
        Project* getActive();
        void createActiveMakefile();

    signals:
        void connectProject(Project *project);
        void addDockWidget(Qt::DockWidgetArea area, QDockWidget* dockWidget);
        void tabifyDockWidget(QDockWidget *first, QDockWidget *second);
        void projectOpened();

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
        //~Project();

        void setupSim();
        bool start();
        void stop();
        void reset();
        void step();
        void run();
        void animate();
        MCUSimControl* getSimControl();
        void setCompileOpt(QList<bool> opt);
        void setCompileIncPaths(QList<QString> paths);
        void setUseMainFile(bool enabled);
        void setIntVector(int value);
        void setHWBuild(int value);
        void setScratchpad(int value);
        void setProgMem(int value);
        void setName(QString name);
        void setFamily(QString family);

        QDockWidget *prjDockWidget;
        ProjectTree *prjTreeWidget;

        QString prjName;
        QString prjPath;
        int fileCount;

        LangType langType;
        QString family;
        int scratchpadSize;
        int progMemSize;
        int intVector;
        int hwBuild;
        QString mainFileName;
        QString mainFilePath;
        bool useMainFile;
        QList<QString> fileNames;
        QList<QString> filePaths;
        //Symbol, Macro, Debug, Code, List, Hex, Bin, SRec
        QList<bool> compileOpt;
        QStringList compileIncPaths;

        //simulation handle
        McuMemoryView *m_mcuMemoryView;
        RegDisplayList *m_regList;
        MCUSimControl *m_simControlUnit;

    public slots:
        void setMainFile(QString path, QString name);
        void removeFile(QString path, QString name);
        QString addFile(QString path, QString name);
        
    private slots:
        void setActive();
        void openItem();
        void openUntrackedItem();
        void emitFileCount();
        void startCfgDlgCore();
        void handleUpdateRequest(int mask);

    signals:
        void highlightLine(QString file, int line, QColor *color);
        void setCentralByName(QString file);
        void scrollToLine(int line);
        void addUntrackedFile(QString name, QString path);
        //void addDockWidget(Qt::DockWidgetArea area, QDockWidget* dockWidget);
        void openFilePath(QString path);
        void setEditorReadOnly(bool readOnly);
        //void setCentralChanged();
        void fileCountSignal(int fileCount);
        void startConfig(Project *project);
        
    private:
        ProjectMan *parentManager;
        std::vector<std::pair<const std::string *, unsigned int>> breakPoints;
        std::vector<std::pair<const std::string *, unsigned int>> currLine;
        int prevLine;
        int prevLine2;
        int prevLine3;
        QColor currLineColor;
        QColor prevLineColor;
        QColor prevLine2Color;
        QString currFile;
        QString prevFile;
        QString prevFile2;
        QString prevFile3;
        
        QTreeWidgetItem *treeProjName;
        QTreeWidgetItem *treeProjSource;
        QTreeWidgetItem *treeProjInclude;
        QTreeWidgetItem *treeProjCompiled;
        QTreeWidgetItem *treeProjOther;
        
       
};

#endif
