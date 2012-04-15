/**
 * @brief
 * C++ Interface: ...
 *
 * ...
 *
 * Copyright: See COPYING file that comes with this distribution.
 *
 * @author Martin Ošmera <martin.osmera@gmail.com>, (C) 2012
 * @ingroup McuSimCfgMgr
 * @file McuDeviceSpec.h
 */

#ifndef MCUDEVICESPEC_H
#define MCUDEVICESPEC_H

#include "MCUSim.h"
#include <QString>

/**
 * @brief
 * @ingroup McuSimCfgMgr
 * @class McuDeviceSpec
 */
class McuDeviceSpec {
public:
	McuDeviceSpec() {}
	McuDeviceSpec(MCUSim::Arch arch) {
		m_arch = arch;
	}
	virtual ~McuDeviceSpec() {}

	MCUSim::Arch m_arch;
	MCUSim::Family m_family;
	QString m_name;
};

#endif // MCUDEVICESPEC_H
