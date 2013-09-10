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
 * @file MScriptFuncTable.h
 */
// =============================================================================

#ifndef MSCRIPTFUNCTABLE_H
#define MSCRIPTFUNCTABLE_H

// Forward declarations.
class MScriptStatement;
class MScriptInterpretInterface;

// MScript language interpreter header files.
#include "MScriptValue.h"
#include "MScriptSrcLocation.h"

// Standard header files.
#include <map>
#include <vector>
#include <string>
#include <ostream>
#include <utility>

/**
 * @brief
 * @ingroup MoraviaScript
 * @class MScriptFuncTable
 */
class MScriptFuncTable
{
    ////    Friends    ////
    friend std::ostream & operator << ( std::ostream & out,
                                        const MScriptFuncTable & table );

    ////    Public Datatypes    ////
    public:
        /**
         * @brief
         */
        typedef std::pair<std::string,MScriptValue> Parameter;

        /**
         * @brief
         */
        struct Function
        {
            /// @brief
            const MScriptStatement * m_code;

            /// @brief
            const std::vector<Parameter> * m_params;
        };

    ////    Constructors and Destructors    ////
    public:
        /**
         * @brief
         * @param[in,out] interpret
         */
        MScriptFuncTable ( MScriptInterpretInterface * interpret )
                         : m_interpret ( interpret ) {}

    ////    Public Operations    ////
    public:
        /**
         * @brief
         * @param[in] name
         * @param[in] params
         * @return
         */
        bool define ( const std::string & name,
                      const std::vector<Parameter> * params );

        /**
         * @brief
         * @param[in] name
         * @return
         */
        bool undefine ( const std::string & name );

        /**
         * @brief
         * @param[in] name
         * @return
         */
        bool isDefined ( const std::string & name ) const;

        /**
         * @brief
         * @param[in] name
         * @param[in] arguments
         * @return
         */
        const Function & get ( const std::string & name,
                               const std::vector<MScriptValue> & arguments );

    ////    Private Attributes    ////
    private:
        /// @brief
        MScriptInterpretInterface * const m_interpret;

        /// @brief
        std::multimap<std::string,Function> m_funcTable;
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
                                 const MScriptFuncTable & table );
//@}

#endif // MSCRIPTFUNCTABLE_H
