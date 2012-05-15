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
 * @file RegDisplayList.h
 */

#ifndef REGDISPLAYLIST_H
#define REGDISPLAYLIST_H

// Forward declarations
class RegDisplay;
class MCUSimControl;
class QVBoxLayout;

#include "MCUSim.h"
#include "MCUSimObserver.h"

#include <cstdint>
#include <QWidget>

class RegDisplayList : public QWidget, public MCUSimObserver  {
	Q_OBJECT
public:
	RegDisplayList(MCUSimControl * controlUnit, QWidget * parent = 0);
	virtual ~RegDisplayList();

	void handleEvent(int subsysId, int eventId, int locationOrReason, int detail);
	void deviceChanged();
	void deviceReset();
	void setReadOnly(bool readOnly);

public slots:
	void changeValue(uint address, uint value);

private:
	int m_startingAddress;
	int m_numberOfDisplays;
	RegDisplay ** m_regDisplays;
	MCUSim::Memory * m_memory;

	QVBoxLayout * m_layout;

	inline void deleteDisplays();
};

#endif // REGDISPLAYLIST_H
