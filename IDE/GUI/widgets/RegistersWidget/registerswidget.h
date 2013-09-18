#ifndef REGISTERSWIDGET_H
#define REGISTERSWIDGET_H


#include <QTableWidget>
#include "../../../simulators/MCUSim/MCUSim.h"
#include "../../../simulators/MCUSim/MCUSimSubsys.h"
#include "../../../simulators/SimControl/MCUSimObserver.h"
#include "../../../simulators/SimControl/MCUSimControl.h"

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

    private slots:
        void updateValue(int row, int column);
        
    private:
        MCUSimMemory * m_memory;
        MCUSimSubsys::SubsysId subsys;
        int m_startingAddress;
        int m_size;
        bool update;
};


#endif