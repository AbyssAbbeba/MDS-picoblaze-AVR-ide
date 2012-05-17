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


class Project;


/**
 * @brief
 * @ingroup GUI
 * @class ProjectMan
 */
class ProjectMan
{
    public:
        ProjectMan(MainForm *mainWindow);
        void addFile(QFile *file, QString path, QString name);
        void addProject(QString name, QString path, QFile *file);
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
 * @brief
 * @ingroup GUI
 * @class Project
 */
class Project : public QObject
{
    Q_OBJECT
    public:
        //konstruktor pro otevirani projektu
        Project(QFile *file, QMainWindow * mainWindow, ProjectMan *parent);
        //konstruktor pro prazdny projekt
        Project(QString name, QString path, QMainWindow * mainWindow, QFile *file, ProjectMan *parent);
        ~Project();

        void addFile(QFile *file, QString path, QString name);

        QDockWidget *prjDockWidget;
        QTreeWidget *prjTreeWidget;

        QString prjName;
        QString prjPath;
        int fileCount;


        QString mainFileName;
        QString mainFilePath;
        QList<QString> fileNames;
        QList<QString> filePaths;

        int errorFlag;

    private slots:
        void setActive();
        void openItem();
        void setMainFile();
        void removeFile();

    private:
        ProjectMan *parentManager;
       
};

#endif
