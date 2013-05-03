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

// Standard headers.
#include <cstdint>
#include <string>
#include <vector>
#include <fstream>

#include <iostream> // DEBUG

AsmKcpsm3SemanticAnalyzer::AsmKcpsm3SemanticAnalyzer ( CompilerSemanticInterface * compilerCore,
                                                       CompilerOptions * opts )
                                                     : SemanticAnalyzer ( compilerCore, opts )
{
    m_symbolTable = new AsmKcpsm3SymbolTable ( compilerCore, opts );
    m_machineCode = new AsmMachineCodeGen ( AsmMachineCodeGen::WORD_3B );
    m_codeListing = new AsmKcpsm3CodeListing ( compilerCore, opts );
    m_instructionSet = new AsmKcpsm3InstructionSet ( compilerCore, opts );
    m_macros = new AsmKcpsm3Macros ( compilerCore, opts, m_symbolTable, m_codeListing );
}

AsmKcpsm3SemanticAnalyzer::~AsmKcpsm3SemanticAnalyzer()
{
    delete m_symbolTable;
    delete m_machineCode;
    delete m_codeListing;
    delete m_instructionSet;
    delete m_macros;
}

void AsmKcpsm3SemanticAnalyzer::printCodeTree ( const CompilerStatement * codeTree )
{
    if ( true == m_opts->m_codeTree.empty() )
    {
        return;
    }

    if ( true == m_opts->m_makeBackupFiles )
    {
        rename(m_opts->m_codeTree.c_str(), (m_opts->m_codeTree + "~").c_str());
    }

    std::ofstream file ( m_opts->m_codeTree, ( std::fstream::out | std::fstream::trunc ) );

    if ( false == file.is_open() )
    {
        m_compilerCore -> compilerMessage ( CompilerBase::MT_ERROR,
                                            QObject::tr("Unable to open ").toStdString() + "\"" + m_opts->m_codeTree  + "\"" );
        return;
    }

    file << codeTree;

    if ( true == file.bad() )
    {
        m_compilerCore -> compilerMessage ( CompilerBase::MT_ERROR,
                                            QObject::tr("Unable to write to ").toStdString() + "\"" + m_opts->m_codeTree  + "\"" );
        return;
    }
}

void AsmKcpsm3SemanticAnalyzer::process ( CompilerStatement * codeTree )
{
    m_codeListing->loadSourceFiles();

    printCodeTree(codeTree);

    std::cout << "Entering phase 1\n" << codeTree;

    phase1 ( codeTree );

    std::cout << "Phase 1 complete, symbol table defined as follows:\n" << m_symbolTable;
    std::cout << "Entering phase 2\n" << codeTree;

    phase2 ( codeTree );

    std::cout << "Phase 2 complete, codeTree:\n" << codeTree;
    std::cout << "Final symbol table defined as follows:\n" << m_symbolTable;

    m_codeListing->output();

    if ( true == m_compilerCore->successful() )
    {
        m_macros->output();
        m_symbolTable->output();
        m_machineCode->output(AsmMachineCodeGen::E_BIG_ENDIAN, m_compilerCore, m_opts);
    }
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
            {
                int value = m_symbolTable -> addSymbol ( node->args()->m_lValue.m_data.m_symbol,
                                                         node->args()->m_next,
                                                         &(node->location()),
                                                         AsmKcpsm3SymbolTable::STYPE_NUMBER,
                                                         true );
                m_codeListing->setValue(node->location(), value);
                break;
            }
            case ASMKCPSM3_LABEL:
                m_symbolTable->addSymbol(node->args()->m_lValue.m_data.m_symbol, NULL, &(node->location()), AsmKcpsm3SymbolTable::STYPE_LABEL);
                continue;
            case ASMKCPSM3_INCLUDE:
                m_codeListing->setInclusion(node->location(), (int) (node->args()->m_lValue.m_data.m_integer));
                break;
            case ASMKCPSM3_DIR_END:
                break;
            case ASMKCPSM3_DIR_MACRO: // getName(node, 0)
            {
                m_macros->define ( node->location(),
                                   node->args()->m_lValue.m_data.m_symbol,
                                   node->args()->m_next,
                                   node->branch() );
                node->m_branch = NULL;
                break;
            }
            case ASMKCPSM3_MACRO:
            {
                if ( -1 != m_opts->m_maxMacroExp && node->m_userData >= m_opts->m_maxMacroExp )
                {
                    m_compilerCore -> compilerMessage ( node->location(),
                                                        CompilerBase::MT_ERROR,
                                                        QObject::tr("maximum macro expansion level (%1) reached ").arg(m_opts->m_maxMacroExp).toStdString() );
                }
                else
                {
                    CompilerStatement * macro = m_macros->expand(node->location(), node->args()->m_lValue.m_data.m_symbol, node->args()->m_next);
                    node->insertLink(macro);
                }
                break;
            }
            case ASMKCPSM3_COND_ASM:
            {
                node->insertLink(conditionalCompilation(node->branch()));
                break;
            }
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

    unsigned int a = 0;
    for ( CompilerStatement * node = codeTree->next();
          NULL != node;
          node = node->next() )
    {
        if ( ASMKCPSM3_DIR_ORG == node->type() )
        {
            a = m_symbolTable->resolveExpr(node->args());
            m_codeListing->setValue(node->location(), a);
        }
        else if ( ASMKCPSM3_LABEL == node->type() )
        {
            CompilerExpr e((int)a);
            m_symbolTable->assignValue(node->args()->m_lValue.m_data.m_symbol, &e, AsmKcpsm3SymbolTable::STYPE_LABEL, true);

            // Remove the inspected node.
            node = node->prev();
            delete node->next();
        }
        else if ( true == m_instructionSet->isInstruction ( node ) )
        {
            a++;
        }
    }

    for ( CompilerStatement * node = codeTree->next();
          NULL != node;
          node = node->next() )
    {
        if ( ASMKCPSM3_DIR_ORG == node->type() )
        {
            m_machineCode->setOrigin((uint32_t)m_symbolTable->resolveExpr(node->args()));
        }
        else
        {
            int opcode = m_instructionSet->resolveOPcode ( node, m_symbolTable );
            if ( -1 != opcode )
            {
                m_codeListing->setCode ( node->location(),
                                        opcode,
                                        m_machineCode->setCode ( opcode ) );
            }
            else
            {
                continue;
            }
        }

        // Remove the inspected node.
        node = node->prev();
        delete node->next();
    }
}

CompilerStatement * AsmKcpsm3SemanticAnalyzer::conditionalCompilation ( CompilerStatement * ifTree )
{
    using namespace StatementTypes;

    CompilerStatement * result = NULL;

    if ( NULL == ifTree )
    {
        return NULL;
    }

    unsigned int conditionVal = 0;
    for ( CompilerStatement * node = ifTree;
          NULL != node;
          node = node->next() )
    {
        switch ( (int) node->type() )
        {
            case ASMKCPSM3_DIR_IF:
            case ASMKCPSM3_DIR_ELSEIF:
                conditionVal = m_symbolTable->resolveExpr(node->args());
                m_codeListing->setValue(node->location(), conditionVal);
                break;

            case ASMKCPSM3_DIR_IFN:
            case ASMKCPSM3_DIR_ELSEIFN:
                conditionVal = m_symbolTable->resolveExpr(node->args());
                m_codeListing->setValue(node->location(), conditionVal);
                conditionVal = ( ( 0 == conditionVal ) ? 1 : 0 );
                break;

            case ASMKCPSM3_DIR_IFDEF:
            case ASMKCPSM3_DIR_ELSEIFDEF:
                conditionVal = ( ( true == m_symbolTable->isDefined(node->args()->lVal().m_data.m_symbol) ) ? 1 : 0 );
                break;

            case ASMKCPSM3_DIR_IFNDEF:
            case ASMKCPSM3_DIR_ELSEIFNDEF:
                conditionVal = ( ( true == m_symbolTable->isDefined(node->args()->lVal().m_data.m_symbol) ) ? 0 : 1 );
                break;

            case ASMKCPSM3_DIR_IFB:
            case ASMKCPSM3_DIR_ELSEIFB:
                conditionVal = ( ( CompilerExpr::Value::TYPE_EMPTY == node->args()->lVal().m_type ) ? 1 : 0 );
                break;

            case ASMKCPSM3_DIR_IFNB:
            case ASMKCPSM3_DIR_ELSEIFNB:
                conditionVal = ( ( CompilerExpr::Value::TYPE_EMPTY == node->args()->lVal().m_type ) ? 0 : 1 );
                break;

            case ASMKCPSM3_DIR_ELSE:
                conditionVal = 1;
                break;
        }

        if ( 0 != conditionVal )
        {
            result = node->m_branch;
            node->m_branch = NULL;
            return result;
        }
    }

    return result;
}
