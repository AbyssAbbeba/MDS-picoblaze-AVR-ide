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




#ifndef PROJECTDLG_H
#define PROJECTDLG_H

#include <QDialog>
#include <QDialogButtonBox>
#include <QLabel>
#include <QMainWindow>
#include <QGridLayout>
#include <QLineEdit>
#include <QComboBox>
#include "../mainform/project.h"



/**
 * @brief
 * @ingroup GUI
 * @class ProjectDialog
 */
class ProjectDialog : public QDialog
{
    Q_OBJECT
    public:
        ProjectDialog(QMainWindow *dialogParent, ProjectMan *dialogProjectMan);

    private slots:
        void bSetPath();
        void bCreate();
        void bReject();

    private:
        void freeDialog();
        QGridLayout *layout;
        QComboBox *architecture;
        QLineEdit *projName, *projDir;
        QLabel *labelName, *labelDir;
        QPushButton *chooseName;
        QDialogButtonBox *buttonBox;
        QMainWindow *parent;
        ProjectMan *projectMan;
        
};

#endif
