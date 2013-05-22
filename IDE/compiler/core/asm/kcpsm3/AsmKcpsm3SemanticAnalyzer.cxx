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

// Common compiler header files.
#include "StatementTypes.h"
#include "../AsmMachineCodeGen.h"
#include "../AsmDgbFileGen.h"

// KCPSM3 assembler semantic analyzer header files.
#include "AsmKcpsm3SymbolTable.h"
#include "AsmKcpsm3CodeListing.h"
#include "AsmKcpsm3InstructionSet.h"
#include "AsmKcpsm3Macros.h"

// Standard headers.
#include <cstdint>
#include <string>
#include <vector>
#include <fstream>
#include <iostream> // DEBUG

void AsmKcpsm3SemanticAnalyzer::MemoryPtr::clear()
{
    m_code = 0;
    m_reg = 0;
    m_data = 0;
}

AsmKcpsm3SemanticAnalyzer::AsmKcpsm3SemanticAnalyzer ( CompilerSemanticInterface * compilerCore,
                                                       CompilerOptions * opts )
                                                     : SemanticAnalyzer ( compilerCore, opts )
{
    m_symbolTable = new AsmKcpsm3SymbolTable ( compilerCore, opts );
    m_machineCode = new AsmMachineCodeGen ( AsmMachineCodeGen::WORD_3B );
    m_codeListing = new AsmKcpsm3CodeListing ( compilerCore, opts );
    m_instructionSet = new AsmKcpsm3InstructionSet ( compilerCore, opts );
    m_macros = new AsmKcpsm3Macros ( compilerCore, opts, m_symbolTable, m_codeListing );
    m_dgbFile = new AsmDgbFileGen();

    m_memoryPtr.clear();
}

AsmKcpsm3SemanticAnalyzer::~AsmKcpsm3SemanticAnalyzer()
{
    delete m_symbolTable;
    delete m_machineCode;
    delete m_codeListing;
    delete m_instructionSet;
    delete m_macros;
    delete m_dgbFile;
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
    m_memoryPtr.clear();
    m_codeListing->loadSourceFiles();

    printCodeTree(codeTree);

    std::cout << "Entering phase 1, codeTree:\n" << codeTree;

    phase1 ( codeTree->next() );

    std::cout << "Entering phase 2, codeTree:\n" << codeTree;

    phase2 ( codeTree );

    std::cout << "Phase 2 complete, codeTree:\n" << codeTree;

    m_codeListing->output();

    if ( true == m_compilerCore->successful() )
    {
        m_macros->output();
        m_symbolTable->output();
        m_dgbFile->output(m_compilerCore, m_opts);
        m_machineCode->output(AsmMachineCodeGen::E_BIG_ENDIAN, m_compilerCore, m_opts);
    }
}

void AsmKcpsm3SemanticAnalyzer::phase1 ( CompilerStatement * codeTree,
                                         const CompilerBase::SourceLocation * origLocation,
                                         const std::string * macroName )
{
    using namespace StatementTypes;

    std::vector<std::string> localSymbols;

    for ( CompilerStatement * node = codeTree;
          NULL != node;
          node = node->next() )
    {
        if ( true == m_instructionSet->isInstruction ( node ) )
        {
            m_instructionSet->encapsulate(node, m_symbolTable, m_memoryPtr.m_code);
            m_memoryPtr.m_code++;
            continue;
        }

        const CompilerBase::SourceLocation * location = origLocation;
        if ( NULL == location )
        {
            location = &(node->location());
        }

        switch ( (int) node->type() )
        {
            case ASMKCPSM3_DIR_CODE:
            case ASMKCPSM3_DIR_PORT:
            case ASMKCPSM3_DIR_DATA:
            case ASMKCPSM3_DIR_REG:
            case ASMKCPSM3_DIR_EQU:
            {
                AsmKcpsm3SymbolTable::SymbolType symbolType;
                switch ( (int) node->type() )
                {
                    case ASMKCPSM3_DIR_CODE:
                        symbolType = AsmKcpsm3SymbolTable::STYPE_LABEL;
                        break;
                    case ASMKCPSM3_DIR_DATA:
                        symbolType = AsmKcpsm3SymbolTable::STYPE_DATA;
                        break;
                    case ASMKCPSM3_DIR_PORT:
                        symbolType = AsmKcpsm3SymbolTable::STYPE_PORT;
                        break;
                    case ASMKCPSM3_DIR_REG:
                        symbolType = AsmKcpsm3SymbolTable::STYPE_REGISTER;
                        break;
                    case ASMKCPSM3_DIR_EQU:
                        symbolType = AsmKcpsm3SymbolTable::STYPE_NUMBER;
                        break;
                    default:
                        symbolType = AsmKcpsm3SymbolTable::STYPE_UNSPECIFIED;
                        break;
                }
                int value = m_symbolTable -> addSymbol ( node->args()->lVal().m_data.m_symbol,
                                                         node->args()->m_next,
                                                         location,
                                                         symbolType,
                                                         true );
                m_codeListing->setValue(node->location(), value);
                break;
            }
            case ASMKCPSM3_DIR_SET:
            {
                int value;
                std::string name = node->args()->lVal().m_data.m_symbol;
                if ( true == m_symbolTable->isDefined(name) )
                {
                    value = m_symbolTable -> assignValue ( name,
                                                           node->args()->m_next,
                                                           location,
                                                           AsmKcpsm3SymbolTable::STYPE_NUMBER,
                                                           true );

                    m_compilerCore -> compilerMessage ( CompilerBase::MT_REMARK,
                                                        QObject::tr ( "redefining symbol value: " ).toStdString()
                                                                    + "\"" + name + "\"" );
                }
                else
                {
                    value = m_symbolTable -> addSymbol ( name,
                                                         node->args()->m_next,
                                                         location,
                                                         AsmKcpsm3SymbolTable::STYPE_NUMBER,
                                                         true,
                                                         true );
                }
                m_codeListing->setValue(node->location(), value);
                break;
            }
            case ASMKCPSM3_DIR_DEFINE:
            {
                int value = m_symbolTable -> addSymbol ( node->args()->lVal().m_data.m_symbol,
                                                         node->args()->m_next,
                                                         location,
                                                         AsmKcpsm3SymbolTable::STYPE_EXPRESSION,
                                                         false );
                m_codeListing->setValue(node->location(), value);
                break;
            }
            case ASMKCPSM3_DIR_UNDEFINE:
                m_symbolTable -> removeSymbol ( node->args()->lVal().m_data.m_symbol,
                                                node->location() );
                break;
            case ASMKCPSM3_LABEL:
            {
                CompilerExpr e(m_memoryPtr.m_code);
                m_symbolTable -> addSymbol ( node->args()->lVal().m_data.m_symbol,
                                             &e,
                                             location,
                                             AsmKcpsm3SymbolTable::STYPE_LABEL,
                                             true );
                break;
            }
            case ASMKCPSM3_DIR_ORG:
                m_memoryPtr.m_code = m_symbolTable->resolveExpr(node->args());
                m_codeListing->setValue(node->location(), m_memoryPtr.m_code);
                continue; // This directive must stay in the code tree until phase 2.
            case ASMKCPSM3_DIR_SKIP:
                m_memoryPtr.m_code++;
                continue; // This directive must stay in the code tree until phase 2.
            case ASMKCPSM3_LOCAL:
                if ( true == m_macros->isFromMacro(node) )
                {
                    const std::string local = node->args()->lVal().m_data.m_symbol;
                    if ( false == m_macros -> mangleName ( *location,
                                                           &localSymbols,
                                                           local,
                                                           *macroName,
                                                           node->next() ) )
                    {
                        m_compilerCore->compilerMessage(*origLocation,
                                                        CompilerBase::MT_WARNING,
                                                        QObject::tr("symbol `%1' declared as local but never used, declaration ignored").arg(local.c_str()).toStdString());
                    }
                }
                else
                {
                    m_compilerCore->compilerMessage(node->location(),
                                                    CompilerBase::MT_ERROR,
                                                    QObject::tr("directive `LOCAL' cannot apper outside macro definition")
                                                               .toStdString());
                }
                break;
            case ASMKCPSM3_DIR_EXITM:
                if ( true == m_macros->isFromMacro(node) )
                {
                    node->m_prev = NULL;
                    node->completeDelete();
                    return;
                }
                else
                {
                    m_compilerCore->compilerMessage(node->location(),
                                                    CompilerBase::MT_ERROR,
                                                    QObject::tr("directive EXITM' cannot apper outside macro definition")
                                                               .toStdString());
                    break;
                }
            case ASMKCPSM3_INCLUDE:
                m_codeListing -> setInclusion ( node->location(),
                                                (int) (node->args()->m_lValue.m_data.m_integer) );
                break;
            case ASMKCPSM3_DIR_END:
                break;
            case ASMKCPSM3_DIR_MACRO:
            {
                m_macros -> define ( node->location(),
                                     node->args()->lVal().m_data.m_symbol,
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
                    const std::string nameOfMacro = node->args()->lVal().m_data.m_symbol;

                    CompilerStatement * macro = m_macros -> expand ( *location,
                                                                     node->location(),
                                                                     nameOfMacro,
                                                                     node->args()->next() );

                    if ( StatementTypes::EMPTY_STATEMENT == macro->type() )
                    {
                        break;
                    }

                    //
                    node = node->prev();
                    delete node->next();
                    phase1(macro, location, &nameOfMacro);
                    CompilerStatement * nodeNext = macro->last();
                    node->insertLink(macro);
                    node = nodeNext;
                }
                continue;
            }
            case ASMKCPSM3_DIR_WHILE:
            {
                CompilerBase::SourceLocation endwLoc = node->branch()->last()->location();
                delete node->branch()->last(); // remove `ENDW' directive

                unsigned int i;
                CompilerStatement * body = NULL;
                for ( i = 0; i < MAX_WHILE_ITERATIONS; i++ )
                {
                    if ( 0 == m_symbolTable->resolveExpr(node->args()) )
                    {
                        break;
                    }

                    if ( NULL == body )
                    {
                        body = node->branch()->copyEntireChain();
                        m_codeListing->expandMacro ( endwLoc, body, body );
                        phase1(body, location);
                    }
                    else
                    {
                        CompilerStatement * exp = node->branch()->copyEntireChain();
                        m_codeListing->expandMacro ( body->last()->location(), exp, exp );
                        phase1(exp, location);
                        body->appendLink(exp);
                    }
                }
                if ( MAX_WHILE_ITERATIONS == i )
                {
                    m_compilerCore -> compilerMessage ( *location,
                                                        CompilerBase::MT_ERROR,
                                                        QObject::tr("maximum number of WHILE directive iterations (%1) reached").arg(MAX_WHILE_ITERATIONS).toStdString() );
                }

                if ( NULL != body)
                {
                    node = node->prev();
                    delete node->next();
                    CompilerStatement * nodeNext = body->last();
                    node->insertLink(body);
                    node = nodeNext;
                    continue;
                }
                else
                {
                    break;
                }
            }
            case ASMKCPSM3_DIR_REPT:
            {
                unsigned int times = m_symbolTable->resolveExpr(node->args());
                if ( 0 != times )
                {
                    CompilerBase::SourceLocation endrLoc = node->branch()->last()->location();
                    delete node->branch()->last(); // remove `ENDR' directive
                    CompilerStatement * body = node->branch()->copyEntireChain();
                    for ( unsigned int i = 1; i < times; i++ )
                    {
                        CompilerStatement * exp = node->branch()->copyEntireChain();
                        m_codeListing->expandMacro ( endrLoc, exp, exp );
                        body->appendLink(exp);
                    }

                    node->insertLink(body);
                }
                break;
            }
            case ASMKCPSM3_DIR_LIST:
                m_codeListing->setNoList(node->location(), false);
                break;
            case ASMKCPSM3_DIR_NOLIST:
                m_codeListing->setNoList(node->location(), true);
                break;
            case ASMKCPSM3_DIR_EXPAND:
                m_macros->setExpEnabled(true);
                break;
            case ASMKCPSM3_DIR_NOEXPAND:
                m_macros->setExpEnabled(false);
                break;
            case ASMKCPSM3_DIR_TITLE:
            {
                const CompilerExpr::Value::Data::CharArray & argCharArray = node->args()->lVal().m_data.m_array;
                std::string arg ( (char*) argCharArray.m_data, argCharArray.m_size );
                m_codeListing->setTitle(arg);
                break;
            }
            case ASMKCPSM3_DIR_MESSG:
            {
                const CompilerExpr::Value::Data::CharArray & argCharArray = node->args()->lVal().m_data.m_array;
                std::string arg ( (char*) argCharArray.m_data, argCharArray.m_size );
                m_compilerCore->compilerMessage(*location, CompilerBase::MT_REMARK, arg);
                break;
            }
            case ASMKCPSM3_DIR_WARNING:
            {
                const CompilerExpr::Value::Data::CharArray & argCharArray = node->args()->lVal().m_data.m_array;
                std::string arg ( (char*) argCharArray.m_data, argCharArray.m_size );
                m_compilerCore->compilerMessage(*location, CompilerBase::MT_WARNING, arg);
                break;
            }
            case ASMKCPSM3_DIR_ERROR:
            {
                const CompilerExpr::Value::Data::CharArray & argCharArray = node->args()->lVal().m_data.m_array;
                std::string arg ( (char*) argCharArray.m_data, argCharArray.m_size );
                m_compilerCore->compilerMessage(*location, CompilerBase::MT_ERROR, arg);
                break;
            }
            case ASMKCPSM3_DIR_DB:
            {
                std::vector<unsigned char> dbData;

                for ( CompilerExpr * arg = node->args();
                      NULL != arg;
                      arg = arg->next() )
                {
                    if ( CompilerExpr::Value::TYPE_ARRAY == arg->lVal().m_type )
                    {
                        const CompilerExpr::Value::Data::CharArray & charArray = arg->lVal().m_data.m_array;
                        for ( int i = 0; i < charArray.m_size; i++ )
                        {
                            dbData.push_back ( charArray.m_data[i] );
                        }
                    }
                    else
                    {
                        dbData.push_back(m_symbolTable->resolveExpr(arg));
                    }
                }

                m_memoryPtr.m_code += ( dbData.size() / 2 ) + ( dbData.size() % 2 );
                node->args()->completeDelete();
                node->m_args = NULL;

                for ( size_t i = 0; i < dbData.size(); i++ )
                {
                    int code = ( dbData[i] << 8 );

                    i++;
                    if ( i < dbData.size() )
                    {
                        code |= dbData[i];
                    }

                    if ( NULL == node->m_args )
                    {
                        node->m_args = new CompilerExpr(code);
                    }
                    else
                    {
                        node->m_args->appendLink(new CompilerExpr(code));
                    }
                }

                // This directive must stay in the code tree until phase 2.
                continue;
            }
            case ASMKCPSM3_DIR_SPRAUTO:
            case ASMKCPSM3_DIR_REGAUTO:
            {
                int addr;
                AsmKcpsm3SymbolTable::SymbolType symbolType;

                if ( ASMKCPSM3_DIR_SPRAUTO == node->type() )
                {
                    if ( NULL != node->args()->next() )
                    {
                        m_memoryPtr.m_data = m_symbolTable->resolveExpr(node->args()->next());
                    }

                    addr = m_memoryPtr.m_data;
                    m_memoryPtr.m_data++;
                    symbolType = AsmKcpsm3SymbolTable::STYPE_DATA;
                }
                else
                {
                    if ( NULL != node->args()->next() )
                    {
                        m_memoryPtr.m_reg = m_symbolTable->resolveExpr(node->args()->next());
                    }

                    addr = m_memoryPtr.m_reg;
                    m_memoryPtr.m_reg++;
                    symbolType = AsmKcpsm3SymbolTable::STYPE_REGISTER;
                }

                CompilerExpr value(addr);
                m_symbolTable -> addSymbol ( node->args()->lVal().m_data.m_symbol,
                                             &value,
                                             location,
                                             symbolType );
                m_codeListing->setValue(node->location(), addr);
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

    for ( CompilerStatement * node = codeTree->next();
          NULL != node;
          node = node->next() )
    {
        switch ( node->type() )
        {
            case ASMKCPSM3_DIR_ORG:
                m_machineCode->setOrigin((uint32_t)m_symbolTable->resolveExpr(node->args()));
                break;

            case ASMKCPSM3_DIR_SKIP:
                m_machineCode->incrementAddr();
                break;

            case ASMKCPSM3_DIR_DB:
                for ( CompilerExpr * arg = node->args();
                      NULL != arg;
                      arg = arg->next() )
                {
                    long long code = arg->lVal().m_data.m_integer;
                    int address = m_machineCode->setCode ( (uint32_t) code );
                    m_codeListing->setCode(node->location(), (int) code, address);
                    m_dgbFile->setCode(node->location(), (int) code, address);
                }
                break;

            default: // Only instructions are expected here.
            {
                int opcode = m_instructionSet->resolveOPcode(node, m_symbolTable);
                if ( -1 != opcode )
                {
                    int address = m_machineCode -> setCode ( opcode );
                    m_codeListing -> setCode ( node->location(), opcode, address );
                    m_dgbFile -> setCode ( node->location(), opcode, address );
                }
                break;
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
                break;

            case ASMKCPSM3_DIR_IFN:
            case ASMKCPSM3_DIR_ELSEIFN:
                conditionVal = m_symbolTable->resolveExpr(node->args());
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

            case ASMKCPSM3_DIR_ENDIF:
                return NULL;
        }

        m_codeListing->setValue(node->location(), conditionVal);

        if ( 0 != conditionVal )
        {
            result = node->m_branch;
            node->m_branch = NULL;
            return result;
        }
    }

    return result;
}
