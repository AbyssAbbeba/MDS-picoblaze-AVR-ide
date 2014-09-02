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
 * @file trialexpired.h
 */


#ifndef TRIALEXPIRED_H
#define TRIALEXPIRED_H

#include <QDialog>
#include "ui_trialexpired.h"


class TrialExpired : public QDialog
{
    Q_OBJECT
    public:
        TrialExpired(QWidget *parent);
        
    private:
        Ui_TrialExpired ui;
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
