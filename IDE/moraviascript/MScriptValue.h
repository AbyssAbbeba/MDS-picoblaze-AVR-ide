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

// Standard header files.
#include <string>
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
            MScriptExpr * m_expr;

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
        MScriptValue();

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
        MScriptValue ( const unsigned char * array,
                       int size );

        /**
         * @brief
         * @param[in,out] array
         * @param[in] size
         * @param[in] copy
         */
        MScriptValue ( unsigned char * array,
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
         */
        void completeDelete();

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
