/**
 * @brief
 * C++ Interface: ...
 *
 * ...
 *
 * (C) copyright 2013, 2014 Moravia Microsystems, s.r.o.
 *
 * @author Martin Ošmera <martin.osmera@moravia-microsystems.com>
 * @author Erik Chalupa <erik.chalupa@moravia-microsystems.com>
 * @ingroup GUI
 * @file regdisplaylist.h
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

	int m_numberOfDisplays;
	RegDisplay ** m_regDisplays;

public slots:
	void changeValue(uint address, uint value);

private:
	QWidget *parentWidget;
	int m_startingAddress;
	MCUSim::Memory * m_memory;


	inline void deleteDisplays();
};

#endif // REGDISPLAYLIST_H
