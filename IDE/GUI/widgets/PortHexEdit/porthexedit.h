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
 * @file porthexedit.h
 */


#ifndef PORTHEXEDIT_H
#define PORTHEXEDIT_H



#include <QWidget>

#include "../../../simulators/MCUSim/MCUSim.h"
#include "../../../simulators/MCUSim/MCUSimSubsys.h"
#include "../../../simulators/SimControl/MCUSimObserver.h"

// Forward declarations
class MCUSimControl;
class HexEdit;
class MCUSimPureLogicIO;
class HexEdit;
class MCUSimControl;
class QGridLayout;
class QLabel;

class PortHexEdit : public QWidget, public MCUSimObserver
{
	Q_OBJECT
    public:
        PortHexEdit(QWidget * parent, MCUSimControl * controlUnit, MCUSimSubsys::SubsysId subsys);
        virtual ~PortHexEdit();

        void handleEvent(int subsysId, int eventId, int locationOrReason, int detail);
        void deviceChanged();
        void deviceReset();
        void setReadOnly(bool readOnly);
        void fixHeight();
        void switchPorts();
        void unhighlight();
        void updateWidget();

        bool visibleIn;

    signals:
        void stopSimSig();

    public slots:
        void changeValueIn(int address);
        void changeValueOut(int address);

    private:
        int m_startingAddress;
        int m_size;
        HexEdit *m_hexEditIn;
        HexEdit *m_hexEditOut;
        MCUSimPureLogicIO * m_plio;
        MCUSimSubsys::SubsysId subsys;
        QLabel *m_lblRD;
        QLabel *m_lblWR;
        QLabel *m_lblWRK;
        QLabel *m_lblInput;
        QLabel *m_lblOutput;
        QGridLayout *m_layout;

        inline void deleteHexEdit();
        
    protected:
        void resizeEvent(QResizeEvent *event);
};

#endif // PORTHEXEDIT_H
