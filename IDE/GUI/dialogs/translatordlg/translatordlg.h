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
 * @file translatordlg.h
 */




#ifndef TRANSLATORDLG_H
#define TRANSLATORDLG_H

#include <QDialog>
#include "ui_translatordlg.h"



/**
 * @brief Disassembler dialog
 * @ingroup GUI
 * @class ProjectDialog
 * @details Used in creating new project
 */
class TranslatorDlg : public QDialog
{
    Q_OBJECT
    public:
        TranslatorDlg(QWidget *parent);

    private slots:
        void setPath();
        void create();

    signals:
        void output(std::vector<std::string> text);
        
    private:
        Ui_TranslatorDlg ui;
        
};

#endif
