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
 * @file extappoutput.h
 */


#ifndef EXTAPPOUTPUT_H
#define EXTAPPOUTPUT_H


#include <QWidget>
#include "ui_extappoutput.h"


class QPlainTextEdit;





/**
 * @brief QWidget
 * @ingroup GUI
 * @class ExtAppOutput
 */
class ExtAppOutput : public QWidget
{
    public:
        ExtAppOutput(QWidget *parent);
        QPlainTextEdit* getTextEdit(int tab);
        void cleanOutput(int tab);
        void setActiveTab(int index);
        void setTabStats(int index, bool enabled, QString name);

    private:
        Ui_ExtAppOutput ui;
};


#endif
