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
 * @file projectcfg_compiler.h
 */


#ifndef PROJECTCFG_COMPILER_H
#define PROJECTCFG_COMPILER_H


#include <QWidget>
//#include <QDialogButtonBox>
#include <QCheckBox>
#include <QLabel>
#include <QLineEdit>
#include <QList>
#include "../project/project.h"
#include "ui_projectcfg_compiler.h"


class Project;


/**
 * @brief Project Dialog Config compiler tab
 * @ingroup GUI
 * @class ProjectCfg_Compiler
 */
class ProjectCfg_Compiler : public QWidget
{
    Q_OBJECT
    public:
        ProjectCfg_Compiler(QWidget *parentWidget, Project *currProject);
        void save();
        QList<bool> getOpt();
        QList<int> getDepths();

    signals:
        void setMainFile(QString mainFile);

    public slots:
        void setFiles(QList<QString> files, QString mainFile);

    private:
        void load();
      
        Project *project;
        Ui_ProjectCfg_Compiler ui;

};

#endif