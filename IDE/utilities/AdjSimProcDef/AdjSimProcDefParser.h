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
 * @file AdjSimProcDefParser.h
 */
// =============================================================================

#ifndef ADJSIMPROCDEFPARSER_H
#define ADJSIMPROCDEFPARSER_H

#include "AdjSimProcDef.h"

#include <string>

/**
 * @brief
 * @ingroup AdjSimProcDef
 * @class AdjSimProcDef
 */
class AdjSimProcDefParser
{
    ////    Constructors and Destructors    ////
    public:
        /**
         * @brief
         * @param[in] def
         */
        AdjSimProcDefParser ( const std::string & def );

    ////    Inline Public Operations    ////
    public:
        /**
         * @brief
         * @return
         */
        const AdjSimProcDef & data() const
        {
            return m_data;
        }

        bool isValid() const
        {
            return m_valid;
        }

    ////    Private Attributes    ////
    private:
        /// @brief
        AdjSimProcDef m_data;

        /// @brief
        bool m_valid;
};

#endif // ADJSIMPROCDEFPARSER_H
