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
 * @file callwatcher.h
 */


#ifndef REGWATCHER_H
#define REGWATCHER_H


#include <QWidget>
#include <QList>
#include <QString>
#include "ui_regwatcher.h"

class MCUSimControl;


class RegWatcher : public QWidget
{
    Q_OBJECT
    public:
        RegWatcher(QWidget *parent, MCUSimControl *controlUnit);

    private slots:
        void addItem();
        void removeItem();
        void createItem(QString name, int type, int address, int regbank);
        void editItem();
        void editCurrentItem(QString name, int type, int address, int regbank);

    private:
        Ui_RegWatcher ui;
        MCUSimControl *m_simControl;
};




#endif