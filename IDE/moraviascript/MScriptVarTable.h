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
class MScriptInterpretInterface;

// MScript language interpreter header files.
#include "MScriptVariable.h"
#include "MScriptSrcLocation.h"

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
    ////    Friends    ////
    friend std::ostream & operator << ( std::ostream & out,
                                        const MScriptVarTable & table );

    ////    Public Datatypes    ////
    public:
        /**
         * @brief
         */
        typedef std::map<std::string,MScriptVariable> VarTable;

        /**
         * @brief
         */
        struct Index
        {
            /// @brief
            std::vector<unsigned int> m_index;

            /// @brief
            std::vector<std::string> m_key;
        };

    ////    Constructors and Destructors    ////
    public:
        /**
         * @brief
         * @param[in,out] interpret
         */
        MScriptVarTable ( MScriptInterpretInterface * interpret )
                        : m_interpret ( interpret ) {}

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
         * @param[in] location
         * @return
         */
        bool remove ( const std::string & variable,
                      const MScriptSrcLocation & location );

        /**
         * @brief
         * @param[in] variable
         * @param[in] location
         * @param[in] flags
         * @param[in] dimension
         * @return
         */
        bool declare ( const std::string & variable,
                       const MScriptSrcLocation & location,
                       MScriptVariable::Flags flags = MScriptVariable::Flags(0),
                       unsigned int dimensions = 0 );

        /**
         * @brief
         * @param[in] variable
         * @param[in] location
         * @param[in] value
         * @param[in] index
         * @return
         */
        bool assign ( const std::string & variable,
                      const MScriptSrcLocation & location,
                      const MScriptValue & value,
                      const Index & index );

        /**
         * @brief
         * @param[in] variable
         * @param[in] location
         * @param[in] index
         * @return
         */
        const MScriptValue * get  ( const std::string & variable,
                                    const MScriptSrcLocation & location,
                                    const Index & index );

        /**
         * @brief
         * @param[in] variable
         * @return
         */
        bool exists ( const std::string & variable ) const;

        /**
         * @brief
         * @param[in] variable
         * @param[in] index
         * @return
         */
        bool isDefined ( const std::string & variable,
                         const Index & index ) const;

        /**
         * @brief
         * @param[in] variable
         * @return
         */
        MScriptVariable::Flags getFlags ( const std::string & variable ) const;

    ////    Private Attributes    ////
    private:
        /// @brief
        std::vector<VarTable> m_varTables;

        /// @brief
        MScriptInterpretInterface * const m_interpret;
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
