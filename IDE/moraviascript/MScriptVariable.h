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
 * @file MScriptVariable.h
 */
// =============================================================================

#ifndef MSCRIPTVARIABLE_H
#define MSCRIPTVARIABLE_H

// Forward declarations.
class MScriptValue;

// MScript language interpreter header files.
#include "MScriptSrcLocation.h"

// Standard header files.
#include <map>
#include <vector>
#include <string>
#include <ostream>

/**
 * @brief
 * @ingroup MoraviaScript
 * @class MScriptVariable
 */
class MScriptVariable
{
    ////    Friends    ////
    friend std::ostream & operator << ( std::ostream & out,
                                        const MScriptVariable & variable );

    ////    Public Datatypes    ////
    public:
        /**
         * @brief
         */
        enum Flags
        {
            FLAG_CONST = 0x01, ///< 0 == variable, 1 == constant.
            FLAG_ARRAY = 0x02, ///< 0 == scalar, 1 == array.
            FLAG_HASH  = 0x04, ///< 0 == indexed array, 1 == hash - associative array; when FLAG_ARRAY is set to 1.
        };

        /**
         * @brief
         */
        union Value
        {
            /// @brief
            MScriptValue * m_scalar;

            /// @brief
            std::vector<MScriptVariable*> * m_array;

            /// @brief
            std::map<std::string,MScriptVariable*> * m_hash;
        };

    ////    Constructors and Destructors    ////
    public:
        /**
         * @brief
         */
        ~MScriptVariable();

    ////    Public Attributes    ////
    public:
        /// @brief
        Flags m_flags;

        /// @brief
        Value m_value;

        /// @brief
        MScriptSrcLocation m_location;
};

/// @name Tracing operators
//@{
    /**
     * @brief
     * @param[in,out] out
     * @param[in] variable
     * @return
     */
    std::ostream & operator << ( std::ostream & out,
                                 const MScriptVariable & variable );

    /**
     * @brief
     * @param[in,out] out
     * @param[in] flags
     * @return
     */
    std::ostream & operator << ( std::ostream & out,
                                 MScriptVariable::Flags flags );
//@}
    
#endif // MSCRIPTVARIABLE_H