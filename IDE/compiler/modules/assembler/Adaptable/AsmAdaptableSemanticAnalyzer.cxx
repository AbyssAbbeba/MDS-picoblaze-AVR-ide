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
 * @file AsmAdaptableSemanticAnalyzer.cxx
 */
// =============================================================================

#include "AsmAdaptableSemanticAnalyzer.h"

// Common compiler header files.
#include "AsmMacros.h"
#include "AsmMemoryPtr.h"
#include "AsmDgbFileGen.h"
#include "AsmSymbolTable.h"
#include "AsmCodeListing.h"
#include "CompilerStatementTypes.h"

// Adaptable assembler semantic analyzer header files.
#include "AsmAdaptableCodeGenerator.h"
#include "AsmAdaptableTreeDecoder.h"

// Support for processor definition files used Adaptable Simulator
#include "AdjSimProcDef.h"
#include "AdjSimProcDefParser.h"

AsmAdaptableSemanticAnalyzer::AsmAdaptableSemanticAnalyzer ( CompilerSemanticInterface * compilerCore,
                                                             CompilerOptions * opts )
{
    m_dgbFile        = new AsmDgbFileGen();
    m_machineCode    = new AsmMachineCodeGen();
    m_memoryPtr      = new AsmMemoryPtr ( compilerCore );
    m_symbolTable    = new AsmSymbolTable ( compilerCore, opts );
    m_codeGenerator  = new AsmAdaptableCodeGenerator();
    m_codeListing    = new AsmCodeListing ( compilerCore, opts, m_symbolTable, m_codeGenerator );
    m_macros         = new AsmMacros ( compilerCore, opts, m_symbolTable, m_codeListing );
    m_treeDecoder    = new AsmAdaptableTreeDecoder ( m_compilerCore,    m_opts,         m_dgbFile,
                                                     m_machineCode,     m_macros,       m_memoryPtr,
                                                     m_symbolTable,     m_codeListing,  m_device );

    m_memoryPtr->clear();
}

AsmAdaptableSemanticAnalyzer::~AsmAdaptableSemanticAnalyzer()
{
    delete m_macros;
    delete m_dgbFile;
    delete m_memoryPtr;
    delete m_treeDecoder;
    delete m_symbolTable;
    delete m_machineCode;
    delete m_codeListing;
    delete m_codeGenerator;
}

void AsmAdaptableSemanticAnalyzer::setDevice ( const std::string & deviceName )
{
}

void AsmAdaptableSemanticAnalyzer::process ( CompilerStatement * codeTree )
{
}
