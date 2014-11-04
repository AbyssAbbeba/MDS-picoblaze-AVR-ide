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
 * @file registerswidget.h
 */


#ifndef REGISTERSWIDGET_H
#define REGISTERSWIDGET_H


#include <QTableWidget>
#include "../../../simulators/MCUSim/MCUSimSubsys.h"
#include "../../../simulators/SimControl/MCUSimObserver.h"

class MCUSimControl;
class MCUSimMemory;

class RegistersWidget : public QTableWidget, public MCUSimObserver
{
    Q_OBJECT
    public:
        RegistersWidget(QWidget *parent, MCUSimControl * controlUnit, MCUSimSubsys::SubsysId subsys);
        virtual ~RegistersWidget();

        void handleEvent(int subsysId, int eventId, int locationOrReason, int detail);
        void deviceChanged();
        void deviceReset();
        void setReadOnly(bool readOnly);
        //void fixHeight();
        void unhighlight();
        void updateWidget();

    signals:
        void stopSimSig();
        
    private slots:
        void updateValue(int row, int column);
        void unhighlightCell(int row, int column);
        
    private:
        MCUSimMemory * m_memory;
        MCUSimSubsys::SubsysId subsys;
        int m_startingAddress;
        int m_size;
        bool update;
};


#endif