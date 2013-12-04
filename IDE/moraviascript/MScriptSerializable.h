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
 * @file MScriptSerializable.h
 */
// =============================================================================

#ifndef MSCRIPTSERIALIZABLE_H
#define MSCRIPTSERIALIZABLE_H

#include "MScriptSerializer.h"

/**
 * @brief
 * @ingroup MoraviaScript
 * @class MScriptSerializable
 */
class MScriptSerializable
{
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
        virtual void serialize ( MScriptSerializer & output ) const = 0;

        /**
         * @brief
         * @param[in,out]
         */
        virtual void deserialize ( MScriptSerializer & input ) = 0;
};

/// @name Serialization operators.
//@{
    /**
     * @brief Serialize object.
     * @param[in,out] out
     * @param[in] object
     * @return
     */
    MScriptSerializer & operator << ( MScriptSerializer & out,
                                      const MScriptSerializable & object );

    /// @overload
    MScriptSerializer & operator << ( MScriptSerializer & out,
                                      const MScriptSerializable * object );

    /**
     * @brief Deserialize object.
     * @param[in,out] in
     * @param[out] object
     * @return
     */
    MScriptSerializer & operator >> ( MScriptSerializer & in,
                                      MScriptSerializable & object );

    /// @overload
    MScriptSerializer & operator >> ( MScriptSerializer & in,
                                      MScriptSerializable * object );

    /**
     * @brief 
     * @param[in,out] out
     * @param[in] mark
     * @return
     */
    MScriptSerializer & operator << ( MScriptSerializer & out,
                                      MScriptSerializable::SerializationMark mark );

    /**
     * @brief
     * @param[in,out] in
     * @param[out] mark
     * @return
     */
    MScriptSerializer & operator >> ( MScriptSerializer & in,
                                      MScriptSerializable::SerializationMark & mark );
//@}

#endif // MSCRIPTSERIALIZABLE_H
