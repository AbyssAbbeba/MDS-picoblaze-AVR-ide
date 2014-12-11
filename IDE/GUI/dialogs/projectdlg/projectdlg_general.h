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
        int getAsmType();

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
