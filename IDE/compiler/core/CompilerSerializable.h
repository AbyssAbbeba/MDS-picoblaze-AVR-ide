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
 * @ingroup Compiler
 * @file CompilerSerializable.h
 */
// =============================================================================

#ifndef COMPILERSERIALIZABLE_H
#define COMPILERSERIALIZABLE_H

#include "CompilerSerializer.h"

class CompilerSerializable
{
    ////    Public Static Constants    ////
    static const int INTERFACE_VERSION = 1;

    ////    Public Datatypes    ////
    public:
        /**
         * @brief
         */
        enum SerializationMark
        {
            MARK_NEXT,    ///<
            MARK_TERMINAL ///<
        };

    ////    Public Operations    ////
    public:
        /**
         * @brief
         * @param[in,out]
         */
        virtual void serialize ( CompilerSerializer & output ) const = 0;

        /**
         * @brief
         * @param[in,out]
         */
        virtual void deserialize ( CompilerSerializer & input ) = 0;
};

/// @name Serialization operators.
//@{
    /**
     * @brief Serialize object.
     * @param[in,out] out
     * @param[in] object
     * @return
     */
    CompilerSerializer & operator << ( CompilerSerializer & out,
                                       const CompilerSerializable & object );

    /// @overload
    CompilerSerializer & operator << ( CompilerSerializer & out,
                                       const CompilerSerializable * object );

    /**
     * @brief Deserialize object.
     * @param[in,out] in
     * @param[out] object
     * @return
     */
    CompilerSerializer & operator >> ( CompilerSerializer & in,
                                       CompilerSerializable & object );

    /// @overload
    CompilerSerializer & operator >> ( CompilerSerializer & in,
                                       CompilerSerializable * object );

    /**
     * @brief 
     * @param[in,out] out
     * @param[in] mark
     * @return
     */
    CompilerSerializer & operator << ( CompilerSerializer & out,
                                       CompilerSerializable::SerializationMark mark );

    /**
     * @brief
     * @param[in,out] in
     * @param[out] mark
     * @return
     */
    CompilerSerializer & operator >> ( CompilerSerializer & in,
                                       CompilerSerializable::SerializationMark & mark );
//@}

#endif // COMPILERSERIALIZABLE_H
