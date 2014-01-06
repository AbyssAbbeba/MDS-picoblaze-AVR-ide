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
 * @file CompilerValue.h
 */
// =============================================================================

#ifndef COMPILERVALUE_H
#define COMPILERVALUE_H

// Forward declarations.
class CompilerExpr;

// Compiler compiler header files.
#include "CompilerBase.h"
#include "CompilerSerializable.h"

// Standard header files.
#include <string>
#include <ostream>

/**
 * @brief
 * @ingroup Compiler
 * @class CompilerValue
 */
class CompilerValue : public CompilerSerializable
{
    ////    Public Datatypes    ////
    public:
        /**
         * @brief
         */
        enum Type
        {
            TYPE_EMPTY = 0, ///<
            TYPE_INT,       ///<
            TYPE_REAL,      ///<
            TYPE_EXPR,      ///<
            TYPE_SYMBOL,    ///<
            TYPE_ARRAY      ///<
        };

        /**
         * @brief
         */
        union Data
        {
            ///
            long long m_integer;

            ///
            double m_real;

            ///
            CompilerExpr * m_expr;

            ///
            char * m_symbol;

            ///
            struct CharArray
            {
                ///
                unsigned char * m_data;

                ///
                int m_size;
            } m_array;
        };

    ////    Constructors and Destructors    ////
    public:
        /**
         * @brief
         */
        CompilerValue();

        /**
         * @brief
         * @param[in,out] input
         */
        CompilerValue ( CompilerSerializer & input );

        /**
         * @brief
         * @param[in] value
         */
        CompilerValue ( int value );

        /**
         * @brief
         * @param[in] value
         */
        CompilerValue ( long long value );

        /**
         * @brief
         * @param[in] value
         */
        CompilerValue ( float value );

        /**
         * @brief
         * @param[in] value
         */
        CompilerValue ( double value );

        /**
         * @brief
         * @param[in,out] expr
         */
        CompilerValue ( CompilerExpr * expr );

        /**
         * @brief
         * @param[in] string
         */
        CompilerValue ( const char * string );

        /**
         * @brief
         * @param[in] string
         */
        CompilerValue ( const std::string & string );

        /**
         * @brief
         * @param[in] array
         * @param[in] size
         */
        CompilerValue ( const unsigned char * array,
                        int size );

        /**
         * @brief
         * @param[in,out] array
         * @param[in] size
         * @param[in] copy
         */
        CompilerValue ( unsigned char * array,
                        int size,
                        bool copy = false );
        /**
         * @brief
         */
        virtual ~CompilerValue() {}

    ////    Public Operations    ////
    public:
        /**
         * @brief
         * @return
         */
        CompilerValue * makeCopy() const;

        /**
         * @brief
         */
        void completeDelete();

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
        /**
         * @brief
         */
        Data m_data;

        /**
         * @brief
         */
        Type m_type;
};

/// @name Tracing operators
//@{
    /**
     * @brief
     * @param[in,out] out
     * @param[in] val
     * @return
     */
    std::ostream & operator << ( std::ostream & out,
                                 const CompilerValue & val );
//@}

#endif // COMPILERVALUE_H
