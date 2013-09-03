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
 * @file MScriptVarTable.h
 */
// =============================================================================

#ifndef MSCRIPTVARTABLE_H
#define MSCRIPTVARTABLE_H

// Forward declarations.
class MScriptValue;

// Standard header files.
#include <map>
#include <vector>
#include <string>
#include <ostream>

/**
 * @brief
 * @ingroup MoraviaScript
 * @class MScriptVarTable
 */
class MScriptVarTable
{
    ////    Private Datatypes    ////
    private:
        /**
         * @brief
         */
        enum Flags
        {
            FLAG_CONST = 0x01, ///< 0 == variable, 1 == constant.
            FLAG_ARRAY = 0x02, ///< 0 == scalar, 1 == array.
            FLAG_HASH  = 0x04, ///< 0 == indexed array, 1 == hash - associative array.
        };

        /**
         * @brief
         */
        union Value
        {
            MScriptValue * m_scalar;
            std::vector<MScriptValue*> * m_array;
            std::map<std::string,MScriptValue*> * m_hash;
        };

        /**
         * @brief
         */
        struct Variable
        {
            /// @brief
            Flags m_type;

            /// @brief
            Value m_value;
        };

        /**
         * @brief
         */
        typedef std::map<std::string,Variable> VarTable;

    ////    Public Operations    ////
    public:
        /**
         * @brief
         */
        void pushScope();

        /**
         * @brief
         */
        void popScope();

        /**
         * @brief
         * @param[in] variable
         * @return
         */
        bool remove ( const std::string & variable );

    ////    Private Attributes    ////
    private:
        /// @brief
        std::vector<VarTable> m_varTables;
};

/// @name Tracing operators
//@{
    /**
     * @brief
     * @param[in,out] out
     * @param[in] table
     * @return
     */
    std::ostream & operator << ( std::ostream & out,
                                 const MScriptVarTable & table );
//@}

#endif // MSCRIPTVARTABLE_H
