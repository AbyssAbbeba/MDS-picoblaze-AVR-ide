// =============================================================================
/**
 * @brief
 * C++ Implementation: ...
 *
 * ...
 *
 * (C) copyright 2013, 2014 Moravia Microsystems, s.r.o.
 *
 * @ingroup Compiler
 * @file CompilerCPreprocessor.cxx
 */
// =============================================================================

#include "CompilerCPreprocessor.h"

// Common compiler header files.
#include "CompilerOptions.h"
#include "CompilerSemanticInterface.h"

// Standard headers.
#include <cstdlib>

CompilerCPreprocessor::CompilerCPreprocessor ( CompilerSemanticInterface * compilerCore,
                                               CompilerOptions * opts )
{
}

CompilerCPreprocessor::~CompilerCPreprocessor()
{
}

char * CompilerCPreprocessor::processFile ( FILE * sourceFile )
{
    char * line = NULL;
    size_t bufSize = 0;
    ssize_t lineLen = -1;

    // Iterate over lines in the file.
    while ( -1 != ( lineLen = getline(&line, &bufSize, sourceFile) ) )
    {
    }
}
