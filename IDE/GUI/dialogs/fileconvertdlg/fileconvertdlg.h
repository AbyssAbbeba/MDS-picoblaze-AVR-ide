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
