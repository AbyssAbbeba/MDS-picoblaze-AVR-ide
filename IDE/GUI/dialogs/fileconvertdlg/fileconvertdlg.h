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
 * @file fileconvertdlg.h
 */




#ifndef FILECONVERTDLG_H
#define FILECONVERTDLG_H

#include <QDialog>
#include "ui_fileconvertdlg.h"



/**
 * @brief Disassembler dialog
 * @ingroup GUI
 * @class ProjectDialog
 * @details Used in creating new project
 */
class FileConvertDlg : public QDialog
{
    Q_OBJECT
    public:
        FileConvertDlg(QWidget *parent);

    private slots:
        void setPathIn();
        void setPathOut();
        void setPathTemplate();
        void create();
        void setOptIn(int index);
        void setOptOut(int index);
        
    private:
        Ui_FileConvertDlg ui;
        
};

#endif
