// =============================================================================
/**
 * @brief
 * C++ Interface: ...
 *
 * ...
 *
 * (C) copyright 2013, 2014 Moravia Microsystems, s.r.o.
 *
 * @author Martin Ošmera <martin.osmera@moravia-microsystems.com>
 * @ingroup AdaptableAsm
 * @file AsmAdaptableSemanticAnalyzer.h
 */
// =============================================================================

#ifndef ASMADAPTABLESEMANTICANALYZER_H
#define ASMADAPTABLESEMANTICANALYZER_H

// Common compiler header files.
#include "CompilerSemanticAnalyzer.h"
#include "AsmMachineCodeGen.h"

/**
 * @brief
 * @ingroup PicoBlazeAsm
 * @class AsmAdaptableSemanticAnalyzer
 */
class AsmAdaptableSemanticAnalyzer : public CompilerSemanticAnalyzer
{
    ////    Constructors and Destructors    ////
    public:
        /**
         * @brief
         * @param[in,out] compilerCore
         * @param[in] opts
         */
        AsmAdaptableSemanticAnalyzer ( CompilerSemanticInterface * compilerCore,
                                       CompilerOptions * opts );

        /**
         * @brief
         */
        virtual ~AsmAdaptableSemanticAnalyzer();

    ////    Public Operations    ////
    public:
        /**
         * @brief
         * @param[in,out] codeTree
         */
        virtual void process ( CompilerStatement * codeTree ) override;

        /**
         * @brief
         * @param[in] deviceName
         */
        virtual void setDevice ( const std::string & deviceName ) override;
};

#endif // ASMADAPTABLESEMANTICANALYZER_H
