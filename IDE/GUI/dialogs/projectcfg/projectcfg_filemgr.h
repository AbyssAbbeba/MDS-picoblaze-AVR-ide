/**
 * @brief Header for file manager class
 * C++ Interface: ...
 *
 * ...
 *
 * Copyright: See COPYING file that comes with this distribution.
 *
 * @author Erik Chalupa <xchalu10@stud.fit.vutbr.cz>, (C) 2012
 * @ingroup GUI
 * @file filemgr.h
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


    private slots:
        void deleteFile();
        void newFile();
        void addFile();
        void setMainFile();

    signals:
        void reloadTree();

    private:
        Project *project;
        Ui_ProjectCfg_FileMgr ui;

        bool reloadFiles;
};



#endif
