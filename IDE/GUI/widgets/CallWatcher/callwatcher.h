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
#include "../../../simulators/SimControl/MCUSimObserver.h"

class MCUSimControl;
class MCUSimCPU;


class CallWatcher : public QWidget, public MCUSimObserver
{
    Q_OBJECT
    public:
        CallWatcher(QWidget *parent, MCUSimControl *controlUnit);
        void handleEvent(int subsysId, int eventId, int locationOrReason, int detail);
        void deviceChanged();
        void deviceReset();
        void setReadOnly(bool readOnly);
        void setRun(bool run);
        void setInterruptAddr(int addr);
        void setSimulated(bool simulated);

    private slots:
        void returnSlot();
        void handleUpdateRequest(int mask);

    private:
        Ui_CallWatcher ui;
        MCUSimCPU *m_cpu;
        //0 call, 1 int, 2 ret, 3 reti
        QList<int> m_callStack;
        QList<int> m_locationStack;
        QList<int> m_detailStack;
        bool m_run;
        int m_intAddr;
};




#endif