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
 * @ingroup Disassembler
 * @file DAsmCore.h
 */
// =============================================================================

#ifndef DASMCORE_H
#define DASMCORE_H

// Disassembler header files.
#include "DAsm.h"

/**
 * @brief
 * @ingroup Disassembler
 * @class DAsmCore
 */
class DAsmCore : public DAsm
{
    ////    Protected Operations    ////
    protected:
        /**
         * @brief
         * @param[in,out] line
         * @param[in] column
         */
        void indent ( std::string & line,
                      unsigned int column );

        /**
         * @brief
         * @param[in,out] line
         * @param[in] str
         */
        void appendStr ( std::string & line,
                         const std::string & str );

        /**
         * @brief
         * @param[in] number
         * @return
         */
        std::string num2str ( unsigned int number );

        /**
         * @brief
         * @param[in] str
         * @return
         */
        std::string letterCase ( const char * str );
};

#endif // DASMCORE_H
