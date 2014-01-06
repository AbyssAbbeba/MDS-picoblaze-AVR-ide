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
 * @ingroup Compiler
 * @file CompilerModules.h
 */
// =============================================================================

#ifndef COMPILERMODULES_H
#define COMPILERMODULES_H

// Compiler header files.
#include "CompilerBase.h"
#include "CompilerSemanticAnalyzer.h"

/**
 * @brief
 * @namespace CompilerModules
 * @ingroup Compiler
 */
namespace CompilerModules
{
    ////    Datatypes    ////

    /**
     * @brief Module employment status code.
     */
    enum ModEmplStatCode
    {
        MESC_OK,                 ///<
        MESC_IO_ERROR,           ///<
        MESC_ARCH_NOT_SUPPORTED, ///<
        MESC_LANG_NOT_SUPPORTED, ///<
        MESC_UNKNOWN_ERROR       ///<
    };


    ////    Operations    ////

    /**
     * @brief
     * @param[in] lang
     * @param[in] arch
     * @param[in,out] compilerCore
     * @param[out] semanticAnalyzer
     * @param[out] errStr
     * @return
     */
    ModEmplStatCode employModule ( CompilerBase::LangId lang,
                                   CompilerBase::TargetArch arch,
                                   CompilerCore * compilerCore,
                                   CompilerSemanticAnalyzer * & semanticAnalyzer,
                                   std::string * errStr );
};

#endif // COMPILERMODULES_H
