// =============================================================================
/**
 * @brief
 * C++ Interface: ...
 *
 * ...
 *
 * (C) copyright 2014 Moravia Microsystems, s.r.o.
 *
 * @author Martin Ošmera <martin.osmera@moravia-microsystems.com>
 * @ingroup McuSimCfgMgr
 * @file McuDeviceSpecAdaptable.h
 */
// =============================================================================

#ifndef MCUDEVICESPECADAPTABLE_H
#define MCUDEVICESPECADAPTABLE_H

#include "McuDeviceSpec.h"
#include "AdaptableSim/AdaptableSimConfig.h"
#include "AdjSimProcDef/AdjSimProcDef.h"

/**
 * @brief
 * @ingroup McuSimCfgMgr
 * @class McuDeviceSpecAdaptable
 */
class McuDeviceSpecAdaptable : public McuDeviceSpec
{
    ////    Constructors and Destructors    ////
    public:
        /**
         * @brief
         * @param[in] procDef
         */
        McuDeviceSpecAdaptable ( const AdjSimProcDef & procDef );

    ////    Public Operations    ////
    public:
        /**
         * @brief
         * @param[out] mcuConfig
         * @return
         */
        bool setupSimulator ( AdaptableSimConfig & mcuConfig ) const;

    ////    Private Attributes    ////
    private:
        /// @brief
        const AdjSimProcDef m_procDef;
};

#endif // MCUDEVICESPECADAPTABLE_H
