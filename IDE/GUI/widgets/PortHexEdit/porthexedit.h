/**
 * @brief
 * C++ Interface: ...
 *
 * ...
 *
 * Copyright: See COPYING file that comes with this distribution.
 *
 * @author Erik Chalupa <erik.chalupa@moravia-microsystems.com>, (C) 2013
 * @ingroup SimWidgets
 * @file PortHexEdit.h
 */

#ifndef PORTHEXEDIT_H
#define PORTHEXEDIT_H

// Forward declarations
class HexEdit;
class MCUSimControl;
class QHBoxLayout;


#include <cstdint>
#include <QWidget>
#include <QHBoxLayout>

#include "../HexEdit/hexedit.h"
#include "../../../simulators/MCUSim/MCUSim.h"
#include "../../../simulators/SimControl/MCUSimObserver.h"

class PortHexEdit : public QWidget, public MCUSimObserver
{
	Q_OBJECT
    public:
        PortHexEdit(QWidget * parent, MCUSimControl * controlUnit, MCUSim::Subsys::SubsysId subsys);
        virtual ~PortHexEdit();

        void handleEvent(int subsysId, int eventId, int locationOrReason, int detail);
        void deviceChanged();
        void deviceReset();
        void setReadOnly(bool readOnly);
        void fixHeight();

    public slots:
        void changeValue(int address);

    private:
        int m_startingAddress;
        int m_size;
        HexEdit * m_hexEdit;
        MCUSim::Memory * m_memory;
        MCUSim::Subsys::SubsysId subsys;

        QHBoxLayout * m_layout;

        inline void deleteHexEdit();
};

#endif // PORTHEXEDIT_H
