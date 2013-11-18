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
class MScriptNamespaces;
class MScriptInterpretInterface;

// MScript language interpreter header files.
#include "MScriptValue.h"
#include "MScriptSrcLocation.h"
#include "MScriptNamespaces.h"

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
        struct Parameter
        {
            /**
             * @brief
             */
            Parameter();

            /**
             * @brief
             * @param[in] name
             */
            Parameter ( const char * name );

            /**
             * @brief
             * @param[in] name
             * @param[in] defaulValue
             */
            Parameter ( const char * name,
                        const MScriptValue & defaulValue );

            /**
             * @brief
             * @param[in] name
             * @param[in] reference
             */
            Parameter ( const char * name,
                        bool reference );

            /// @brief
            std::string m_name;

            /// @brief
            MScriptValue * m_value;

            /// @brief
            bool m_reference;
        };

        /**
         * @brief
         */
        struct Function
        {
            /**
             * @brief
             * @param[in,out] params
             * @param[in,out] code
             */
            Function ( std::vector<Parameter> * params,
                       MScriptStatement * code,
                       const MScriptSrcLocation & location,
                       unsigned int argsRequired,
                       MScriptNamespaces::NsDesc * ns );

            /**
             * @brief
             * @param[in] obj
             */
            Function ( const Function & obj );

            /**
             * @brief
             */
            ~Function();

            /// @brief
            std::vector<Parameter> * m_params;

            /// @brief
            MScriptStatement * m_code;

            /// @brief
            MScriptSrcLocation m_location;

            /// @brief
            MScriptNamespaces::NsDesc * m_ns;

            /// @brief
            unsigned int m_argsRequired;

            /// @brief
            int m_id;
        };

        /// @brief
        typedef std::multimap<std::string,Function> FuncMultimap;

    ////    Constructors and Destructors    ////
    public:
        /**
         * @brief
         * @param[in,out] interpret
         * @param[in,out] namespaces
         */
        MScriptFuncTable ( MScriptInterpretInterface * interpret,
                           MScriptNamespaces * namespaces )
                         :
                           m_interpret ( interpret ),
                           m_namespaces ( namespaces ) {}

         /**
          * @brief
          */
         ~MScriptFuncTable();

    ////    Public Operations    ////
    public:
        /**
         * @brief
         * @param[in] name
         * @param[in] location
         * @param[in,out] params
         * @param[in,out] code
         */
        void define ( const std::string & name,
                      const MScriptSrcLocation & location,
                      std::vector<Parameter> * params,
                      MScriptStatement * code );

        /**
         * @brief
         * @param[in,out] ns
         * @param[in] name
         * @param[in,out] params
         * @param[in,out] code
         * @return
         */
        void define ( MScriptNamespaces::NsDesc * ns,
                      const std::string & name,
                      std::vector<Parameter> * params,
                      int id );

        /**
         * @brief
         * @param[in] name
         * @param[in] location
         * @return
         */
        bool undefine ( const std::string & name,
                        const MScriptSrcLocation & location );

        /**
         * @brief
         * @param[in] name
         * @param[in] id
         * @return
         */
        bool undefine ( const std::string & name,
                        int id );

        /**
         * @brief
         * @param[in] name
         * @param[in] location
         * @param[in] argc
         * @param[out] defLocation
         * @param[in] exactNsMatch
         * @return
         */
        bool isDefined ( const std::string & name,
                         const MScriptSrcLocation & location,
                         int argc = -1,
                         MScriptSrcLocation * defLocation = NULL,
                         bool exactNsMatch = false ) const;

        /**
         * @brief
         * @param[in] name
         * @param[in] location
         * @param[in] arguments
         * @return
         */
        Function * get ( const std::string & name,
                         const MScriptSrcLocation & location,
                         std::vector<MScriptValue> & arguments );

        /**
         * @brief
         */
        void clear();

    ////    Private Attributes    ////
    private:
        /// @brief
        MScriptInterpretInterface * const m_interpret;

        /// @brief
        MScriptNamespaces * const m_namespaces;

        /// @brief
        FuncMultimap m_funcTable;
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

    /**
     * @brief
     * @param[in,out] out
     * @param[in] parameterList
     * @return
     */
    std::ostream & operator << ( std::ostream & out,
                                 const std::vector<MScriptFuncTable::Parameter> * parameterList );
//@}

#endif // MSCRIPTFUNCTABLE_H
