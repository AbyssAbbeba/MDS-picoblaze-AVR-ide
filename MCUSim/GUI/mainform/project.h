#ifndef PROJECT_H
#define PROJECT_H

#define ERR_OK 500
#define ERR_ASSIGN 501
#define ERR_CONTENT 502

#include <QString>
#include <QList>
#include <QVector>
#include <QMainWindow>
#include <QTreeWidget>
#include <QtXml>


class Project;

class ProjectMan
{
    public:
        ProjectMan(QMainWindow * mainWindow);
        void addFile(QFile *file, QString path, QString name);
        void addProject(QString name, QString path, QFile *file);
        void openProject(QFile *file);
        void setActive(Project *activePrj);
        bool isActiveProject(Project *project);
        Project* getActive();

    private:
        int projectCount;
        QList<Project*> openProjects;
        Project *activeProject;
        QMainWindow *mainWindow;
    
};


class Project : public QObject
{
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

        QList<QString> fileNames;
        QList<QString> filePaths;

        int errorFlag;

    private slots:
        void setActive();

    private:
        ProjectMan *parentManager;
       
};

#endif
