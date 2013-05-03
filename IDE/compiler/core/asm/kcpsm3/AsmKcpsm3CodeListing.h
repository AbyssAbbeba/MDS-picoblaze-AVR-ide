// =============================================================================
/**
 * @brief
 * C++ Interface: ...
 *
 * ...
 *
 * (C) copyright 2013 Moravia Microsystems, s.r.o.
 *
 * @author Martin Ošmera <martin.osmera@gmail.com>
 * @ingroup Kcpsm3Asm
 * @file AsmKcpsm3CodeListing.h
 */
// =============================================================================

#ifndef ASMKCPSM3CODELISTING_H
#define ASMKCPSM3CODELISTING_H

// Common compiler header files.
#include "../../CompilerSemanticInterface.h"
#include "../../CompilerOptions.h"

// Standard headers.
#include <string>
#include <vector>
#include <ostream>

/**
 * @brief
 * @ingroup Kcpsm3Asm
 * @class AsmKcpsm3CodeListing
 */
class AsmKcpsm3CodeListing
{
    ////    Friends    ////
    friend std::ostream & operator << ( std::ostream & out,
                                        const AsmKcpsm3CodeListing * codeListing );

    ////    Public Static Constants    ////
    public:
        /// @brief
        static const unsigned int MAX_LINE_LENGTH = 1024;

    ////    Constructors and Destructors    ////
    public:
        /**
         * @brief
         * @param[in,out] compilerCore
         * @param[in] opts
         * @return
         */
        AsmKcpsm3CodeListing ( CompilerSemanticInterface * compilerCore,
                               CompilerOptions * opts );

        /**
         * @brief
         */
        ~AsmKcpsm3CodeListing();

    ////    Public Datatypes    ////
    public:
        /**
         * @brief
         */
        struct LstLine
        {
            LstLine ( );
            LstLine ( const char * line );

            int m_address;
            int m_code;
            int m_macro;
            int m_inclusion;
            int m_value;
            std::string m_line;
        };

    ////    Public Operations    ////
    public:
        /**
         * @brief
         */
        void loadSourceFiles();

        /**
         * @brief
         */
        void output();

        /**
         * @brief
         * @param[in] location
         * @param[in] fileNumber
         */
        void setInclusion ( CompilerBase::SourceLocation location,
                            int fileNumber );

        /**
         * @brief
         * @param[in] location
         * @param[in] code
         * @param[in] address
         */
        void setCode ( CompilerBase::SourceLocation location,
                       int code,
                       int address );

        /**
         * @brief
         * @param[in] location
         * @param[in] value
         */
        void setValue ( CompilerBase::SourceLocation location,
                        int value );

        /**
         * @brief
         * @param[in] location
         * @param[in] definition
         * @param[in,out] expansion
         */
        void expandMacro ( CompilerBase::SourceLocation location,
                           const CompilerStatement * definition,
                           CompilerStatement * expansion );

    ////    Private Operations    ////
    private:
        /**
         * @brief
         * @param[in,out] out
         * @param[in,out] lineNumber
         * @param[in] fileNumber
         * @param[in] inclusionLevel
         * @param[in] macroLevel
         */
        void printCodeListing ( std::ostream & out,
                                unsigned int & lineNumber,
                                unsigned int fileNumber = 0,
                                unsigned int inclusionLevel = 0,
                                unsigned int macroLevel = 0 ) const;

        /**
         * @brief
         * @param[in] definition
         */
        void copyMacroBody ( const CompilerStatement * definition );

        /**
         * @brief
         * @param[in,out] lineNumber
         * @param[in,out] macro
         */
        void rewriteMacroLoc ( unsigned int & lineNumber,
                               CompilerStatement * macro );

    ////    Inline Private Operations    ////
    private:
        /**
         * @brief
         * @param[in] location
         * @return
         */
        inline bool checkLocation ( CompilerBase::SourceLocation location );

    ////    Private Attributes    ////
    private:
        ///
        unsigned int m_numberOfFiles;

        ///
        unsigned int m_numberOfMacros;

        ///
        std::vector<std::vector<LstLine>> m_listing;

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
     * @param[in] codeListing
     * @return
     */
    std::ostream & operator << ( std::ostream & out,
                                 const AsmKcpsm3CodeListing * codeListing );
//@}

#endif // ASMKCPSM3CODELISTING_H
