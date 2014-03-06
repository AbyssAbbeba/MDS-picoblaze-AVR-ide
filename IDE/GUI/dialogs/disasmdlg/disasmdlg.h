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
