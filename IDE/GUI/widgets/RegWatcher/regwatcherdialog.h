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
 * @file regwatcherdialog.h
 */


#ifndef REGWATCHERDIALOG_H
#define REGWATCHERDIALOG_H


#include <QDialog>
#include "ui_regwatcherdialog.h"


class RegWatcherDialog : public QDialog
{
    Q_OBJECT
    public:
        RegWatcherDialog(QWidget *parent, bool regbanks);

    signals:
        void newItem(QString name, int type, int address);

    private slots:
        void emitNewItem();
        void checkIfValid();
        void updateBanks(int index);

    private:
        Ui_RegWatcherDialog ui;
        bool m_regbanks;
};



#endif