// =============================================================================
/**
 * @brief
 * C++ Implementation: ...
 *
 * ...
 *
 * (C) copyright 2013 Moravia Microsystems, s.r.o.
 *
 * @author Martin Ošmera <martin.osmera@gmail.com>
 * @ingroup Kcpsm3Asm
 * @file AsmKcpsm3SemanticAnalyzer.cxx
 */
// =============================================================================

#include "AsmKcpsm3SemanticAnalyzer.h"
#include "StatementTypes.h"

#include <iostream> // DEBUG

AsmKcpsm3SemanticAnalyzer::AsmKcpsm3SemanticAnalyzer ( CompilerCore * compilerCore,
                                                       CompilerOptions * opts )
                                                     : CompilerSemanticInterface ( compilerCore, opts )
{
    m_symbolTable = new AsmKcpsm3SymbolTable ( compilerCore, opts );
    m_machineCode = new AsmMachineCodeGen;
    m_codeListing = new AsmKcpsm3CodeListing ( compilerCore, opts );
    m_instructionSet = new AsmKcpsm3InstructionSet ( compilerCore, opts );
}

AsmKcpsm3SemanticAnalyzer::~AsmKcpsm3SemanticAnalyzer()
{
    delete m_symbolTable;
    delete m_machineCode;
    delete m_codeListing;
    delete m_instructionSet;
}

void AsmKcpsm3SemanticAnalyzer::process ( CompilerStatement * codeTree )
{
    m_codeListing->loadSourceFiles();

    std::cout << "Entering phase 1\n" << codeTree;
    phase1 ( codeTree );
    std::cout << "Phase 1 complete, symbol table defined as follows:\n" << m_symbolTable;
    std::cout << "Entering phase 2\n" << codeTree;
    phase2 ( codeTree );
    std::cout << "Phase 2 complete, codeTree:\n" << codeTree;

    m_codeListing->output ( "output.lst" );
}

inline void AsmKcpsm3SemanticAnalyzer::phase1 ( CompilerStatement * codeTree )
{
    using namespace StatementTypes;

    for ( CompilerStatement * node = codeTree->next();
          NULL != node;
          node = node->next() )
    {
        StatementType type = node->type();
        switch ( (int) type )
        {
            case ASMKCPSM3_DIR_EQU:
                m_symbolTable->addSymbol ( node->args()->at(0)->copyChainLink(),
                                           type,
                                           node->args()->at(1)->copyChainLink() );
                break;
            case ASMKCPSM3_LABEL:
                m_symbolTable->addSymbol ( node->args()->at(0)->copyChainLink(),
                                           type );
                break;
            case ASMKCPSM3_DIR_END:
                break;
            default:
                // For now keep the nodes which were not understood yet.
                continue;
        }

        // Remove the inspected node.
        node = node->prev();
        delete node->next();
    }
}

inline void AsmKcpsm3SemanticAnalyzer::phase2 ( CompilerStatement * codeTree )
{
    using namespace StatementTypes;

    for ( CompilerStatement * node = codeTree->next();
          NULL != node;
          node = node->next() )
    {
        int opcode = m_instructionSet->resolveOPcode ( node, m_symbolTable );
        if ( -1 != opcode )
        {
            m_codeListing->setOPcode ( opcode, node->location(),
                                       (unsigned int) m_machineCode->setCode ( opcode ) );
        }
        else
        {
            continue;
        }

        // Remove the inspected node.
        node = node->prev();
        delete node->next();
    }
}
