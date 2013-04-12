/**
 * @brief
 * C++ Interface: ...
 *
 * ...
 *
 * Copyright: See COPYING file that comes with this distribution.
 *
 * @author Martin Ošmera <martin.osmera@gmail.com>, (C) 2012
 * @ingroup SimWidgets
 * @file McuMemoryView.h
 */

#ifndef MCUMEMORYVIEW_H
#define MCUMEMORYVIEW_H

// Forward declarations
class HexEdit;
class MCUSimControl;
class QGridLayout;


#include <cstdint>
#include <QWidget>

#include "MCUSim.h"
#include "MCUSimObserver.h"

class McuMemoryView : public QWidget, public MCUSimObserver  {
	Q_OBJECT
public:
	McuMemoryView(MCUSimControl * controlUnit, QWidget * parent = 0);
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

	QGridLayout * m_layout;

	inline void deleteHexEdit();
};

#endif // MCUMEMORYVIEW_H
