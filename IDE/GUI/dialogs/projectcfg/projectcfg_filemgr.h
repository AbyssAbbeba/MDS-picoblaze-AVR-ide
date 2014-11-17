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
 * @file projectcfg_filemgr.h
 */


#ifndef FILEMGR_H
#define FILEMGR_H


#include <QWidget>
#include <QStringList>
#include "ui_projectcfg_filemgr.h"

class Project;


/**
 * @brief
 * @ingroup GUI
 * @class FileMgr
 */
class ProjectCfg_FileMgr : public QWidget
{
    Q_OBJECT
    public:
        ProjectCfg_FileMgr(QWidget *parentWidget, Project *currProject);
        QStringList getPaths();

    public slots:
        void setMainFileByName(QString mainFile);
        void requestFiles();

    private slots:
        void deleteFile();
        void newFile();
        void addFile();
        void setMainFile();

    signals:
        void reloadTree();
        void setFiles(QList<QString> files, QString mainFile);

    private:
        Project *project;
        Ui_ProjectCfg_FileMgr ui;

        bool reloadFiles;
        QString mainFile;
};



#endif
