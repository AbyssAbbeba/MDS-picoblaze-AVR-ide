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
 * @file codeedit.h
 */



#ifndef CODEEDIT_H
#define CODEEDIT_H


#include <QPlainTextEdit>
#include <QTabWidget>
#include "../mainform/project.h"


class Project;

//editor kodu s pouzitim QSyntaxHighlight a pocitanim radku
/**
 * @brief
 * @ingroup GUI
 * @class CodeEdit
 */
class CodeEdit : public QPlainTextEdit
{
    Q_OBJECT   
    public:
        CodeEdit(QTabWidget *parent, QString wName, QString wPath);
        CodeEdit(QTabWidget *parent, Project* parentPrj, QString wName, QString wPath);
        QString getName();
        QString getPath();
        void setName(QString wName);
        void setPath(QString wPath);
        void setSaved();
        bool isChanged();
        void connectAct();
        bool isChild(Project* project);
        void setParentProject(Project* project);

    public slots:
        void setChanged();

    private:
        QString name;
        QString path;
        bool changed;
        Project* parentProject;
        QTabWidget *parentWidget;
};


#endif
