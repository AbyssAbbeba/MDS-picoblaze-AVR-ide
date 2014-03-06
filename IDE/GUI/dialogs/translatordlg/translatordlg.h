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
        void tabsChanged(const QString &text);

    signals:
        void output(std::vector<std::string> & text);
        void outputError(const std::vector<std::pair<unsigned int, std::string>> & text);
        
    private:
        Ui_TranslatorDlg ui;
        
};

#endif
