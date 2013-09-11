/**
 * @brief
 * C++ Interface: ...
 *
 * ...
 *
 * Copyright: See COPYING file that comes with this distribution.
 *
 * @author Erik Chalupa <xchalu10@stud.fit.vutbr.cz>, (C) 2013
 * @ingroup GUI
 * @file disasmdlg.h
 */




#ifndef DISASMDLG_H
#define DISASMDLG_H

#include <QDialog>
#include "ui_disasmdlg.h"



/**
 * @brief Disassembler dialog
 * @ingroup GUI
 * @class ProjectDialog
 * @details Used in creating new project
 */
class DisAsmDialog : public QDialog
{
    Q_OBJECT
    public:
        DisAsmDialog(QWidget *parent);

    private slots:
        void setPath();
        void create();

    signals:
        void output(std::vector<std::string> text);
        
    private:
        Ui_Dialog ui;
        //void freeDialog();
        //QGridLayout *layout;
        /*QComboBox *cmbArchitecture;
        QComboBox *cmbFamily;
        QLineEdit *lePath;
        QLabel *lblPath;
        QLabel *lblOptions;
        QPushButton *btnPath;
        QDialogButtonBox *buttonBox;*/
        
};

#endif
