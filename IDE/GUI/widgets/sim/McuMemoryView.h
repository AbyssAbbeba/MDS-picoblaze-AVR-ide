/**
 * @brief
 * C++ Interface: ...
 *
 * ...
 *
 * Copyright: See COPYING file that comes with this distribution.
 *
 * @author Martin Ošmera <martin.osmera@moravia-microsystems.com>, (C) 2012
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
#include "../../../simulators/SimControl/MCUSimObserver.h"

class McuMemoryView : public QWidget, public MCUSimObserver  {
	Q_OBJECT
public:
	McuMemoryView(QWidget * parent, MCUSimControl * controlUnit, MCUSim::Subsys::SubsysId subsys);
	virtual ~McuMemoryView();

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

#endif // MCUMEMORYVIEW_H
