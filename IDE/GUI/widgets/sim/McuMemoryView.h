/**
 * @brief
 * C++ Interface: ...
 *
 * ...
 *
 * Copyright: See COPYING file that comes with this distribution.
 *
 * @author Martin Ošmera <martin.osmera@moravia-microsystems.com>, (C) 2012
 * @author Erik Chalupa <erik.chalupa@moravia-microsystems.com>, (C) 2012
 * @ingroup SimWidgets
 * @file McuMemoryView.h
 */

#ifndef MCUMEMORYVIEW_H
#define MCUMEMORYVIEW_H

// Forward declarations
class HexEdit;
class MCUSimControl;
class QHBoxLayout;


#include <cstdint>
#include <QWidget>
#include <QHBoxLayout>

#include "../../../simulators/MCUSim/MCUSim.h"
#include "../../../simulators/MCUSim/MCUSimSubsys.h"
#include "../../../simulators/SimControl/MCUSimObserver.h"

class McuMemoryView : public QWidget, public MCUSimObserver  {
	Q_OBJECT
    public:
        McuMemoryView(QWidget * parent, MCUSimControl * controlUnit, MCUSimSubsys::SubsysId subsys);
        virtual ~McuMemoryView();

        void handleEvent(int subsysId, int eventId, int locationOrReason, int detail);
        void deviceChanged();
        void deviceReset();
        void setReadOnly(bool readOnly);
        void fixHeight();
        void unhighlight();

    signals:
        void stopSimSig();

    public slots:
        void changeValue(int address);

    private:
        int m_startingAddress;
        int m_size;
        HexEdit * m_hexEdit;
        MCUSimMemory * m_memory;
        MCUSimSubsys::SubsysId subsys;

        QHBoxLayout * m_layout;

        inline void deleteHexEdit();
};

#endif // MCUMEMORYVIEW_H
