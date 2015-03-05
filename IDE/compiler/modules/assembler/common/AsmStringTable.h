// =============================================================================
/**
 * @brief
 * C++ Interface: ...
 *
 * ...
 *
 * (C) copyright 2014 Moravia Microsystems, s.r.o.
 *
 * @author Martin Ošmera <martin.osmera@moravia-microsystems.com>
 * @ingroup Assembler
 * @file AsmStringTable.h
 */
// =============================================================================

#ifndef ASMSTRINGTABLE_H
#define ASMSTRINGTABLE_H

// Common compiler header files.
#include "CompilerOptions.h"
#include "CompilerSemanticInterface.h"

// Standard headers.
#include <map>
#include <string>
#include <ostream>

/**
 * @brief
 * @ingroup Assembler
 * @class AsmStringTable
 */
class AsmStringTable
{
    ////    Friends    ////
    friend std::ostream & operator << ( std::ostream & out,
                                        const AsmStringTable * stringTable );

    ////    Public Datatypes    ////
    public:
        /**
         * @brief
         */
        struct String
        {
            /// @brief
            std::string m_value;

            /// @brief
            CompilerSourceLocation m_location;
        };

    ////    Private Datatypes    ////
    private:
        /**
         * @brief
         */
        enum PredefinedStringID
        {
            PRE_DEF_DATE, ///<
            PRE_DEF_TIME, ///<
            PRE_DEF_FILE, ///<
            PRE_DEF_LINE  ///<
        };

    ////    Private Static Constants    ////
    private:
        ///
        static const std::map<std::string, PredefinedStringID> PREDEFINED_STRINGS;

    ////    Constructors and Destructors    ////
    public:
        /**
         * @brief
         * @param[in,out] compilerCore
         * @param[in] opts
         * @return
         */
        AsmStringTable ( CompilerSemanticInterface * compilerCore,
                         CompilerOptions * opts )
                       : m_compilerCore ( compilerCore ),
                         m_opts ( opts ) {};

        /**
         * @brief
         */
        ~AsmStringTable() {};

    ////    Public Operations    ////
    public:
        /**
         * @brief
         * @param[in] name
         * @param[in] value
         * @param[in] location
         */
        void add ( const std::string & name,
                   const std::string & value,
                   const CompilerSourceLocation * location = nullptr );

        /**
         * @brief
         * @param[in] name
         * @return
         */
        bool find ( const std::string & name ) const;

        /**
         * @brief
         * @param[in] name
         * @param[out] value
         * @param[in] location
         * @return
         */
        bool get ( const std::string & name,
                   std::string & value,
                   const CompilerSourceLocation * location = nullptr );

        /**
         * @brief
         * @param[in] name
         * @return
         */
        const CompilerSourceLocation * getLocation ( const std::string & name ) const;

        /**
         * @brief
         */
        void output();

        /**
         * @brief
         */
        void clear();

    ////    Inline Private Operations    ////
    private:
        /**
         * @brief
         * @param[in,out] out
         * @param[in] location
         */
        inline void printSymLocation ( std::ostream & out,
                                      const CompilerSourceLocation & location ) const;

    ////    Private Attributes    ////
    private:
        ///
        std::map<std::string,String> m_table;

        ///
        CompilerSemanticInterface * const m_compilerCore;

        ///
        CompilerOptions * const m_opts;
};

/// @name Tracing operators
//@{
    /**
     * @brief
     * @param[in,out] out
     * @param[in] stringTable
     * @return
     */
    std::ostream & operator << ( std::ostream & out,
                                 const AsmStringTable * stringTable );
//@}

#endif // ASMSTRINGTABLE_H
