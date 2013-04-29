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

#include "../../CompilerCore.h"
#include "../../CompilerOptions.h"

#include <string>
#include <vector>

/**
 * @brief
 * @ingroup Kcpsm3Asm
 * @class AsmKcpsm3CodeListing
 */
class AsmKcpsm3CodeListing
{
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
        AsmKcpsm3CodeListing ( CompilerCore * compilerCore,
                               CompilerOptions * opts )
                             : m_compilerCore ( compilerCore ),
                               m_opts ( opts ) {};

    ////    Public Datatypes    ////
    public:
        /**
         * @brief
         */
        struct LstLine
        {
            LstLine ( );
            LstLine ( const char * line,
                      int lineNumber,
                      int fileNumber );

            int m_address;
            int m_code;
            int m_macro;
            int m_inclusion;
            int m_value;
            int m_fileNumber;
            int m_lineNumber;
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
         * @param[in] filename
         */
        void output ( const std::string & filename );

        /**
         * @brief
         * @param[in] opCode
         * @param[in] location
         */
        void setOPcode ( int opCode,
                         CompilerBase::SourceLocation location,
                         unsigned int address );

    ////    Private Attributes    ////
    private:
        ///
        std::vector<LstLine> m_listing;

        ///
        CompilerCore * const m_compilerCore;

        ///
        CompilerOptions * const m_opts;
};

#endif // ASMKCPSM3CODELISTING_H
