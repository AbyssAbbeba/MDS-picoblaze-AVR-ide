/**
 * @brief
 * C++ Interface: ...
 *
 * ...
 *
 * Copyright: See COPYING file that comes with this distribution.
 *
 * @author Martin Ošmera <martin.osmera@gmail.com>, (C) 2012
 *
 */

#ifndef AVR8FUSES_H
#define AVR8FUSES_H

#include "../MCUSim.h"

class AVR8Fuses : public MCUSim::Subsys {
public:
	enum Fuses {
		FUSE_JTAGEN = 0,
		FUSE_OCDEN,
		FUSE_BOOTRST,
		FUSE_BOOTSZ0,
		FUSE_BOOTSZ1,

		FUSE__MAX__
	};

	AVR8Fuses(MCUSim::EventLogger * eventLogger);

	bool operator[] (Fuses fuse) {
		return m_fuses[fuse];
	}

	void setFuse(Fuses fuse, bool value) {
		m_fuses[fuse] = value;
	}

	void reset(SubsysResetMode mode);

protected:
	bool m_fuses[FUSE__MAX__];

private:
	AVR8Fuses();
	inline void resetToInitialValues();
};

#endif // AVR8FUSES_H
