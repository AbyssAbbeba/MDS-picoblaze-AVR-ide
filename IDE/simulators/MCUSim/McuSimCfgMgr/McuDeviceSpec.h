// =============================================================================
/**
 * @brief
 * C++ Interface: ...
 *
 * ...
 *
 * (C) copyright 2013, 2014 Moravia Microsystems, s.r.o.
 *
 * @author Martin Ošmera <martin.osmera@moravia-microsystems.com>
 * @ingroup McuSimCfgMgr
 * @file McuDeviceSpec.h
 */
// =============================================================================

#ifndef MCUDEVICESPEC_H
#define MCUDEVICESPEC_H

#include "MCUSim.h"
#include <QString>

/**
 * @brief
 * @ingroup McuSimCfgMgr
 * @class McuDeviceSpec
 */
class McuDeviceSpec
{
    ////    Constructors and Destructors    ////
    public:
        /**
         * @brief Constructor
         */
        McuDeviceSpec() {}

        /**
         * @brief Constructor
         * @param[in] arch
         */
        McuDeviceSpec ( MCUSim::Arch arch )
        {
            m_arch = arch;
        }

        /**
         * @brief Destructor
         */
        virtual ~McuDeviceSpec() {}

    ////    Public Attributes    ////
    public:
        /**
         * @brief
         */
        MCUSim::Arch m_arch;

        /**
         * @brief
         */
        MCUSim::Family m_family;

        /**
         * @brief
         */
        QString m_name;
};

#endif // MCUDEVICESPEC_H
