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
class MScriptNamespaces;
class MScriptInterpretInterface;

// MScript language interpreter header files.
#include "MScriptVariable.h"
#include "MScriptArrayIndex.h"
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
        typedef std::multimap<std::string,MScriptVariable> VarTable;

    ////    Constructors and Destructors    ////
    public:
        /**
         * @brief
         * @param[in,out] interpret
         * @param[in,out] namespaces
         */
        MScriptVarTable ( MScriptInterpretInterface * interpret,
                          MScriptNamespaces * namespaces );

         /**
          * @brief
          */
         ~MScriptVarTable();

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
         * @param[in] flags
         * @param[in] dimension
         * @param[in] constant
         * @return
         */
        void declare ( const std::string & variable,
                       const MScriptSrcLocation & location,
                       MScriptVariable::Flags flags = MScriptVariable::FLAG_NO_FLAGS,
                       unsigned int dimensions = 0 );

        /**
         * @brief
         * @param[in,out] ns
         * @param[in] variable
         * @param[in] id
         * @return
         */
        bool declare ( MScriptNamespaces::NsDesc * ns,
                       const std::string & variable,
                       int id );

        /**
         * @brief
         * @param[in] variable
         * @param[in] location
         * @param[in] index
         * @return
         */
        bool remove ( const std::string & variable,
                      const MScriptSrcLocation & location,
                      const MScriptArrayIndex * index = NULL );

        /**
         * @brief
         * @param[in] id
         * @return
         */
        bool remove ( int id );

        /**
         * @brief
         * @param[in] variable
         * @param[in] location
         * @param[in] value
         * @param[in] index
         * @return
         */
        void assign ( const std::string & variable,
                      const MScriptSrcLocation & location,
                      const MScriptValue & value,
                      const MScriptArrayIndex * index = NULL );

        /**
         * @brief
         * @param[in] variable
         * @param[in] index
         * @param[in] location If NULL, do not generate any error messages; othewise use this location in them.
         * @param[in,out] level
         * @return
         */
        MScriptValue * access ( const std::string & variable,
                                const MScriptArrayIndex * index = NULL,
                                const MScriptSrcLocation * location = NULL,
                                int * level = NULL );

        /**
         * @brief
         * @param[in] refName
         * @param[in] refTarget
         * @param[in] location
         * @param[in] refIndex
         * @param[in] targetIndex
         * @return
         */
        void refer ( const std::string & refName,
                     const std::string & refTarget,
                     const MScriptSrcLocation & location,
                     const MScriptArrayIndex * refIndex = NULL,
                     const MScriptArrayIndex * targetIndex = NULL );

        /**
         * @brief
         * @param[in] variable
         * @param[in] index
         * @return
         */
        bool declared ( const std::string & variable,
                        const MScriptArrayIndex * index = NULL );

        /**
         * @brief
         * @param[in] variable
         * @param[in] index
         * @return
         */
        bool defined ( const std::string & variable,
                       const MScriptArrayIndex * index = NULL );

        /**
         * @brief
         * @param[in] variable
         * @return
         */
        MScriptVariable::Flags getFlags ( const std::string & variable );

        /**
         * @brief
         * @param[in] variable
         * @return
         */
        const MScriptSrcLocation * getLocation ( const std::string & variable );

        /**
         * @brief
         */
        void clear();

    ////    Private Operations    ////
    private:
        /**
         * @brief
         * @param[in] flags
         * @return
         */
        std::string flags2Str ( MScriptVariable::Flags flags ) const;

    ////    Inline Private Operations    ////
    private:
        /**
         * @brief
         * @param[in] variable
         * @param[in,out] level
         * @return
         */
        inline MScriptVariable * rawAccess ( const std::string & variable,
                                             int * level = NULL );

        /**
         * @brief
         * @param[in] variable
         * @param[in] index
         * @param[in] location
         * @return
         */
        inline MScriptValue * newArrayElement ( const std::string & variable,
                                                const MScriptArrayIndex * index,
                                                const MScriptSrcLocation * location );

        /**
         * @brief
         * @param[in] variable
         * @param[in] index
         * @param[in] location
         * @param[in,out] var
         * @return
         */
        inline MScriptValue * newArrayElementIdx ( const std::string & variable,
                                                   const MScriptArrayIndex * index,
                                                   const MScriptSrcLocation * location,
                                                   MScriptVariable * var );

        /**
         * @brief
         * @param[in] variable
         * @param[in] index
         * @param[in] location
         * @param[in,out] var
         * @return
         */
        inline MScriptValue * newArrayElementKey ( const std::string & variable,
                                                   const MScriptArrayIndex * index,
                                                   const MScriptSrcLocation * location,
                                                   MScriptVariable * var );

        /**
         * @brief
         * @param[in] variable
         * @param[in] input
         * @param[in] location
         * @param[in] index
         * @return
         */
        inline MScriptValue * reaccess ( const std::string & variable,
                                         MScriptValue & input,
                                         const MScriptSrcLocation * location,
                                         const MScriptArrayIndex * index = NULL );

        /**
         * @brief
         * @param[in] reference
         * @param[out] level
         * @param[out] variable
         * @param[out] index
         * @return
         */
        inline void derefer ( const char * reference,
                              int * level,
                              std::string * variable,
                              MScriptArrayIndex * index ) const;

        /**
         * @brief
         * @param[in] variable
         * @param[in] index
         * @param[in] location
         * @param[in,out] cell
         * @return
         */
        inline MScriptValue * accessArray ( const std::string & variable,
                                            const MScriptArrayIndex * index,
                                            const MScriptSrcLocation * location,
                                            MScriptVariable * cell );

        /**
         * @brief
         * @param[in] variable
         * @param[in] index
         * @param[in] location
         * @param[in,out] cell
         * @return
         */
        inline MScriptValue * accessHash ( const std::string & variable,
                                           const MScriptArrayIndex * index,
                                           const MScriptSrcLocation * location,
                                           MScriptVariable * cell );

    ////    Private Attributes    ////
    private:
        /// @brief
        MScriptInterpretInterface * const m_interpret;

        /// @brief
        MScriptNamespaces * const m_namespaces;

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
