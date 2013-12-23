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
 * @ingroup Compiler
 * @file CompilerLocationTracker.h
 */
// =============================================================================

#ifndef COMPILERLOCATIONTRACKER_H
#define COMPILERLOCATIONTRACKER_H

// Compiler core header files.
#include "CompilerSourceLocation.h"

// Standard header files.
#include <vector>
#include <utility>

/**
 * @brief
 * @ingroup Compiler
 * @class CompilerLocationTracker
 */
class CompilerLocationTracker
{
    ////    Public Operations    ////
    public:
        /**
         * @brief
         * @param[in] location
         * @param[in] next
         * @return
         */
        int add ( const CompilerSourceLocation & location,
                  int next = -1 );

        /**
         * @brief
         * @param[in] origin
         * @return
         */
        const CompilerSourceLocation & getLocation ( int origin );

        /**
         * @brief
         * @param[in] origin
         * @return
         */
        int getNext ( int origin );

        /**
         * @brief
         */
        void clear();

    ////    Private Attributes    ////
    private:
        /// @brief
        std::vector<std::pair<CompilerSourceLocation,int> > m_locations;
};

#endif // COMPILERLOCATIONTRACKER_H
