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


#ifndef REGWATCHERWIDGET_H
#define REGWATCHERWIDGET_H


#include <QWidget>
#include <QString>
#include "ui_regwatcherwidget.h"
#include "../../../simulators/SimControl/MCUSimObserver.h"


class MCUSimControl;
class MCUSimMemory;
class MCUSimPureLogicIO;



class RegWatcherWidget : public QWidget, public MCUSimObserver
{
    Q_OBJECT
    public:
        RegWatcherWidget(QWidget *parent, MCUSimControl *controlUnit, QString name, int type, int address);
        void handleEvent(int subsysId, int eventId, int locationOrReason, int detail);
        void deviceChanged();
        void deviceReset();
        void setReadOnly(bool readOnly);

    private slots:
        void handleUpdateRequest(int mask);

    private:
        Ui_RegWatcherWidget ui;
        MCUSimControl *m_simControl;
        int m_address;
        int m_type;
        int m_size;
        MCUSimPureLogicIO *m_plio;
        MCUSimMemory *m_memory;
};




#endif