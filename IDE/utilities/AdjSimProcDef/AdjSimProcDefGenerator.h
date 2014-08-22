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
 * @ingroup AdjSimProcDef
 * @file AdjSimProcDefGenerator.h
 */
// =============================================================================

#ifndef ADJSIMPROCDEFGENERATOR_H
#define ADJSIMPROCDEFGENERATOR_H

#include "AdjSimProcDef.h"

#include <string>

/**
 * @brief
 * @ingroup AdjSimProcDef
 * @class AdjSimProcDefGenerator
 */
class AdjSimProcDefGenerator
{
    ////    Public Static Constants    ////
    public:
        /// @brief
        static const char * FILE_HEADER;

        /// @brief
        static const char * FILE_VERSION;

    ////    Constructors and Destructors    ////
    public:
        /**
         * @brief
         * @param[in] def
         */
        AdjSimProcDefGenerator ( const AdjSimProcDef & def );

    ////    Inline Public Operations    ////
    public:
        /**
         * @brief
         * @return
         */
        const std::string & data() const
        {
            return m_data;
        }

    ////    Private Attributes    ////
    private:
        /// @brief
        std::string m_data;
};

#endif // ADJSIMPROCDEFGENERATOR_H
