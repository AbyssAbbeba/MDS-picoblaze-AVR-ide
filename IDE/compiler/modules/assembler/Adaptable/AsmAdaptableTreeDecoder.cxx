// =============================================================================
/**
 * @brief
 * C++ Implementation: ...
 *
 * ...
 *
 * (C) copyright 2013, 2014 Moravia Microsystems, s.r.o.
 *
 * @author Martin Ošmera <martin.osmera@moravia-microsystems.com>
 * @ingroup AdaptableAsm
 * @file AsmAdaptableTreeDecoder.cxx
 */
// =============================================================================

#include "AsmAdaptableTreeDecoder.h"

// Common compiler header files.
#include "AsmMacros.h"
#include "AsmMemoryPtr.h"
#include "AsmDgbFileGen.h"
#include "AsmSymbolTable.h"
#include "AsmCodeListing.h"
#include "CompilerStatementTypes.h"

// Support for processor definition files used Adaptable Simulator
#include "AdjSimProcDef.h"

AsmAdaptableTreeDecoder::AsmAdaptableTreeDecoder ( CompilerSemanticInterface * compilerCore,
                                                   CompilerOptions           * opts,
                                                   AsmDgbFileGen             * dgbFile,
                                                   AsmMachineCodeGen         * machineCode,
                                                   AsmMacros                 * macros,
                                                   AsmMemoryPtr              * memoryPtr,
                                                   AsmSymbolTable            * symbolTable,
                                                   AsmCodeListing            * codeListing,
                                                   AdjSimProcDef             * device )
{
}

AsmAdaptableTreeDecoder::~AsmAdaptableTreeDecoder()
{
}
