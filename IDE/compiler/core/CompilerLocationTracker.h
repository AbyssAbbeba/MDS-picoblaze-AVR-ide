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
#include "CompilerSerializable.h"
#include "CompilerSourceLocation.h"

// Standard header files.
#include <vector>
#include <utility>
#include <ostream>

/**
 * @brief
 * @ingroup Compiler
 * @class CompilerLocationTracker
 */
class CompilerLocationTracker : public CompilerSerializable
{
    ////    Friends    ////
    friend std::ostream & operator << ( std::ostream & out,
                                        const CompilerLocationTracker & tracker );

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
        const CompilerSourceLocation & getLocation ( int origin ) const;

        /**
         * @brief
         * @param[in] origin
         * @return
         */
        int getNext ( int origin ) const;

        /**
         * @brief
         * @param[in] a
         * @param[in] b
         * @return
         */
        bool differs ( const CompilerSourceLocation & a,
                       const CompilerSourceLocation & b ) const;

        /**
         * @brief
         * @param[in] source
         * @param[in] target
         */
        void traverse ( const CompilerSourceLocation & source,
                        std::vector<const CompilerSourceLocation *> * target ) const;

        /**
         * @brief
         */
        void clear();

        /**
         * @brief
         * @return
         */
        int getInitShift();

        /**
         * @brief
         * @param[in,out]
         */
        virtual void serialize ( CompilerSerializer & output ) const;

        /**
         * @brief
         * @param[in,out]
         */
        virtual void deserialize ( CompilerSerializer & input );

    ////    Private Attributes    ////
    private:
        /// @brief
        std::vector<std::pair<CompilerSourceLocation,int> > m_locations;
};

/// @name Tracing operators
//@{
    /**
     * @brief
     * @param[in,out] out
     * @param[in] location
     * @return
     */
    std::ostream & operator << ( std::ostream & out,
                                 const CompilerLocationTracker & tracker );
//@}

#endif // COMPILERLOCATIONTRACKER_H
