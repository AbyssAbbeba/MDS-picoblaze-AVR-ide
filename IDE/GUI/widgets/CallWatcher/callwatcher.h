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


#ifndef CALLWATCHER_H
#define CALLWATCHER_H


#include <QWidget>
#include <QList>
#include "ui_callwatcher.h"
#include "ui_calllistwidget.h"
#include "../../../simulators/SimControl/MCUSimObserver.h"

class MCUSimControl;


class CallWatcher : public QWidget, public MCUSimObserver
{
    Q_OBJECT
    public:
        CallWatcher(QWidget *parent, MCUSimControl *controlUnit);
        void handleEvent(int subsysId, int eventId, int locationOrReason, int detail);
        void deviceChanged();
        void deviceReset();
        void setReadOnly(bool readOnly);

    private slots:
        void returnSlot();
        void handleUpdateRequest(int mask);

    private:
        Ui_CallWatcher ui;
        Ui_CallListWidget ui_widget;
        MCUSimControl *m_simControlUnit;
        //0 call, 1 int, 2 ret, 3 reti
        QList<int> m_callStack;
        bool m_run;
};




#endif