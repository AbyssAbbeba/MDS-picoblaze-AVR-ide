// =============================================================================
/**
 * @brief
 * C++ Interface: ...
 *
 * ...
 *
 * (C) copyright 2013 Moravia Microsystems, s.r.o.
 *
 * @author Martin Ošmera <martin.osmera@moravia-microsystems.com>
 * @ingroup MCUSim
 * @file MCUSimConfig.h
 */
// =============================================================================

#ifndef MCUSIMCONFIG_H
#define MCUSIMCONFIG_H

#include "MCUSimBase.h"

/**
 * @brief
 * @ingroup MCUSim
 * @class MCUSimConfig
 */
class MCUSimConfig
{
    ////    Constructors and Destructors    ////
    public:
        /**
         * @brief
         * @param[in] arch
         */
        MCUSimConfig ( MCUSimBase::Arch arch ) : m_arch(arch) {}

        /**
         * @brief
         */
        virtual ~MCUSimConfig() {}

    ////    Inline Public Operations    ////
    public:
        /**
         * @brief
         * @return
         */
        MCUSimBase::Arch getArch() const
        {
            return m_arch;
        }

    ////    Private Attributes    ////
    private:
        /**
         * @brief
         */
        const MCUSimBase::Arch m_arch;
};

#endif // MCUSIMCONFIG_H
