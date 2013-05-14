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
 * @ingroup Assembler
 * @file AsmDgbFileGen.h
 */
// =============================================================================

#ifndef ASMDGBFILEGEN_H
#define ASMDGBFILEGEN_H

// Common compiler header files.
#include "../CompilerSemanticInterface.h"
#include "../CompilerOptions.h"

// Standard headers.
#include <string>
#include <vector>

/**
 * @brief
 * @ingroup Assembler
 * @class AsmDgbFileGen
 */
class AsmDgbFileGen
{
    ////    Constructors and Destructors    ////
    public:
        /**
         * @brief
         */
        AsmDgbFileGen();

        /**
         * @brief
         */
        ~AsmDgbFileGen();

    ////    Public Operations    ////
    public:
        /**
         * @brief
         * @param[in] location
         * @param[in] code
         * @param[in] address
         */
        void setCode ( const CompilerBase::SourceLocation & location,
                       int code,
                       int address );

        /**
         * @brief
         * @param[in] filename
         */
        void output ( const std::string & filename ) const;

        /**
         * @brief
         * @param[in,out] compilerCore
         * @param[in] opts
         */
        void output ( CompilerSemanticInterface * compilerCore,
                      const CompilerOptions * opts );
};

#endif // ASMDGBFILEGEN_H
