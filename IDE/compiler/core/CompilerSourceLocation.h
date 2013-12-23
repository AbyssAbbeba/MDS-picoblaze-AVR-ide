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
 * @file CompilerSourceLocation.h
 */
// =============================================================================

#ifndef COMPILERSOURCELOCATION_H
#define COMPILERSOURCELOCATION_H

// Compiler compiler header files.
#include "CompilerSerializable.h"

// Standard header files.
#include <ostream>

/**
 * @brief
 * @ingroup Compiler
 * @class CompilerSourceLocation
 */
class CompilerSourceLocation : public CompilerSerializable
{
    ////    Constructors and Destructors    ////
    public:
        /**
         * @brief
         */
        CompilerSourceLocation();

        /**
         * @brief
         * @param[in] fileNumber
         * @param[in] lineStart
         * @param[in] lineEnd
         * @param[in] colStart
         * @param[in] colEnd
         */
        CompilerSourceLocation ( int fileNumber,
                                 int lineStart,
                                 int lineEnd,
                                 int colStart,
                                 int colEnd );

    ////    Public Operations    ////
    public:
        /**
         * @brief
         * @return
         */
        bool isSet() const;

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

    ////    Public Attributes    ////
    public:
        ///
        int m_fileNumber;

        ///
        int m_lineStart;

        ///
        int m_lineEnd;

        ///
        int m_colStart;

        ///
        int m_colEnd;

        ///
        int m_origin;
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
                                 const CompilerSourceLocation & location );
//@}

#endif // COMPILERSOURCELOCATION_H
