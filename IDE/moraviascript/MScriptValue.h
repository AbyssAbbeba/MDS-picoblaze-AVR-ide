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
 * @ingroup MoraviaScript
 * @file MScriptValue.h
 */
// =============================================================================

#ifndef MSCRIPTVALUE_H
#define MSCRIPTVALUE_H

// Forward declarations.
class MScriptExpr;
class MScriptInterpretInterface;

// MScript language interpreter header files.
#include "MScriptSrcLocation.h"

// Standard header files.
#include <cmath>
#include <string>
#include <cstring>
#include <ostream>

/**
 * @brief
 * @ingroup MoraviaScript
 * @class MScriptValue
 */
class MScriptValue
{
    ////    Public Datatypes    ////
    public:
        /**
         * @brief
         */
        enum Type
        {
            TYPE_EMPTY = 0, ///<
            TYPE_EXPR,      ///<
            TYPE_SYMBOL,    ///<
            TYPE_INT,       ///<
            TYPE_FLOAT,     ///<
            TYPE_STRING,    ///<
            TYPE_BOOL,      ///<
            TYPE_COMPLEX    ///<
        };

        /**
         * @brief
         */
        union Data
        {
            ///
            MScriptExpr * m_expr;

            ///
            char * m_symbol;

            ///
            long long m_integer;

            ///
            double m_float;

            ///
            bool m_bool;

            ///
            struct String
            {
                ///
                char * m_data;

                ///
                unsigned int m_size;
            } m_string;

            ///
            struct Complex
            {
                ///
                double m_r;

                ///
                double m_i;

                ///
                bool operator == ( const Complex & n ) const
                {
                    return ( ( n.m_r == m_r ) &&  ( n.m_i == m_i ) );
                }
            } m_complex;
        };

    ////    Constructors and Destructors    ////
    public:
        /**
         * @brief
         */
        MScriptValue();

        /**
         * @brief
         * @param[in] value
         */
        MScriptValue ( bool value );

        /**
         * @brief
         * @param[in] value
         */
        MScriptValue ( int value );

        /**
         * @brief
         * @param[in] value
         */
        MScriptValue ( long long value );

        /**
         * @brief
         * @param[in] value
         */
        MScriptValue ( float value );

        /**
         * @brief
         * @param[in] value
         */
        MScriptValue ( double value );

        /**
         * @brief
         * @param[in] real
         * @param[in] img
         */
        MScriptValue ( double real,
                       double img );

        /**
         * @brief
         * @param[in,out] expr
         */
        MScriptValue ( MScriptExpr * expr );

        /**
         * @brief
         * @param[in] string
         */
        MScriptValue ( const char * string );

        /**
         * @brief
         * @param[in] string
         */
        MScriptValue ( const std::string & string );

        /**
         * @brief
         * @param[in] array
         * @param[in] size
         */
        MScriptValue ( const char * array,
                       int size );

        /**
         * @brief
         * @param[in,out] array
         * @param[in] size
         * @param[in] copy
         */
        MScriptValue ( char * array,
                       int size,
                       bool copy = false );

    ////    Public Operations    ////
    public:
        /**
         * @brief
         * @return
         */
        MScriptValue & makeCopy() const;

        /**
         * @brief
         * @param[in,out] result
         */
        void makeCopy ( MScriptValue & result ) const;

        /**
         * @brief
         */
        void completeDelete();

        /**
         * @brief
         * @param[in] obj
         * @return
         */
        bool operator == ( const MScriptValue & obj ) const;

        /// @name Type conversion related operations.
        //@{
            /**
             * @brief
             * @param[in,out] interpret
             * @param[in] location
             * @return
             */
            long long toInt ( MScriptInterpretInterface * interpret,
                              const MScriptSrcLocation & location ) const;

            /**
             * @brief
             * @param[in,out] interpret
             * @param[in] location
             * @return
             */
            bool toBool ( MScriptInterpretInterface * interpret,
                          const MScriptSrcLocation & location ) const;
        //@}

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
                                 const MScriptValue & val );
//@}

#endif // MSCRIPTVALUE_H
