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

            /**
             * @brief
             * @return
             */
            std::string toString() const;

            /**
             * @brief
             * @return 0 == scalar, lower than 0 == associative array, higher than 0 == indexed array.
             */
            int dimensions() const;

            /**
             * @brief
             */
            void clear();
        };

    ////    Constructors and Destructors    ////
    public:
        /**
         * @brief
         * @param[in,out] interpret
         */
        MScriptVarTable ( MScriptInterpretInterface * interpret );

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
         * @param[in] index
         * @return
         */
        bool remove ( const std::string & variable,
                      const MScriptSrcLocation & location,
                       const Index * index = NULL );

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
         * @param[in] variable
         * @param[in] location
         * @param[in] value
         * @param[in] index
         * @return
         */
        void assign ( const std::string & variable,
                      const MScriptSrcLocation & location,
                      const MScriptValue & value,
                      const Index * index = NULL );

        /**
         * @brief
         * @param[in] variable
         * @param[in] index
         * @param[in] location If NULL, do not generate any error messages; othewise use this location in them.
         * @param[in,out] level
         * @return
         */
        MScriptValue * access ( const std::string & variable,
                                const Index * index = NULL,
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
                     const Index * refIndex = NULL,
                     const Index * targetIndex = NULL );

        /**
         * @brief
         * @param[in] variable
         * @param[in] index
         * @return
         */
        bool declared ( const std::string & variable,
                        const Index * index = NULL );

        /**
         * @brief
         * @param[in] variable
         * @param[in] index
         * @return
         */
        bool defined ( const std::string & variable,
                       const Index * index = NULL );

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
                                                const Index * index,
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
                                                   const Index * index,
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
                                                   const Index * index,
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
                                         const Index * index = NULL );

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
                              Index * index ) const;

        /**
         * @brief
         * @param[in] variable
         * @param[in] index
         * @param[in] location
         * @param[in,out] cell
         * @return
         */
        inline MScriptValue * accessArray ( const std::string & variable,
                                            const Index * index,
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
                                           const Index * index,
                                           const MScriptSrcLocation * location,
                                           MScriptVariable * cell );

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
