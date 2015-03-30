// =============================================================================
/**
 * @brief
 * C++ Implementation: ...
 *
 * ...
 *
 * (C) copyright 2015 Moravia Microsystems, s.r.o.
 *
 * @ingroup CompilerC
 * @file CompilerCExprProcessor.cxx
 */
// =============================================================================

#include "CompilerCExprProcessor.h"

// Standard headers.
#include<iostream>//DEBUG

// Compiler header files.
#include "CompilerExpr.h"
#include "CompilerOptions.h"
#include "CompilerSemanticInterface.h"

CompilerCExprProcessor::CompilerCExprProcessor ( CompilerSemanticInterface * compilerCore,
                                                 CompilerOptions           * opts )
                                               :
                                                 m_compilerCore ( compilerCore ),
                                                 m_opts ( opts )
{
}

CompilerCExprProcessor::~CompilerCExprProcessor()
{
}
