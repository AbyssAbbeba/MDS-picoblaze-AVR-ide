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
 * @file projectdlg.h
 */




#ifndef PROJECTDLG_GENERAL_H
#define PROJECTDLG_GENERAL_H

#include "ui_projectdlg_general.h"



/**
 * @brief Project Dialog General tab
 * @ingroup GUI
 * @class ProjectDialog
 * @details Used in creating new project
 */
class Projectdlg_General : public QWidget
{
    Q_OBJECT
    public:
        Projectdlg_General(QWidget *parent);
        QString getName();
        QString getPath();
        QString getArch();
        QString getFamily();

    private slots:
        void setPath();
        void familyChanged(const QString &text);

    signals:
        void setScratchpadMaximum(int value);
        void setProgMemMaximum(int value);
        void setHWBuildEnabled(bool enabled);

    private:
        Ui_Projectdlg_General ui;
        QStringList notes;
        
};

#endif
