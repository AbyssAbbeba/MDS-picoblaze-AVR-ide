// =============================================================================
/**
 * @brief
 * C++ Implementation: ...
 *
 * ...
 *
 * (C) copyright 2013 Moravia Microsystems, s.r.o.
 *
 * @author Martin Ošmera <martin.osmera@moravia-microsystems.com>
 * @ingroup PicoBlazeAsm
 * @file AsmPicoBlazeSemanticAnalyzer.cxx
 */
// =============================================================================

#include "AsmPicoBlazeSemanticAnalyzer.h"

// Common compiler header files.
#include "CompilerStatementTypes.h"
#include "../AsmMachineCodeGen.h"
#include "../AsmDgbFileGen.h"

// PicoBlaze assembler semantic analyzer header files.
#include "AsmPicoBlazeSymbolTable.h"
#include "AsmPicoBlazeCodeListing.h"
#include "AsmPicoBlazeInstructionSet.h"
#include "AsmPicoBlazeMacros.h"
#include "AsmPicoBlazeMemoryPtr.h"
#include "AsmPicoBlazeSpecialMacros.h"
#include "AsmPicoBlazeInstructionSet2.h"
#include "AsmPicoBlazeInstructionSet3.h"
#include "AsmPicoBlazeInstructionSet6.h"

// Standard headers.
#include <cstdint>
#include <cstring>
#include <string>
#include <vector>
#include <fstream>
#include <cctype>

AsmPicoBlazeSemanticAnalyzer::AsmPicoBlazeSemanticAnalyzer ( CompilerSemanticInterface * compilerCore,
                                                            CompilerOptions * opts )
                                                           : CompilerSemanticAnalyzer ( compilerCore, opts )
{
    m_symbolTable    = new AsmPicoBlazeSymbolTable ( compilerCore, opts );
    m_machineCode    = new AsmMachineCodeGen ( AsmMachineCodeGen::WORD_3B );
    m_codeListing    = new AsmPicoBlazeCodeListing ( compilerCore, opts );
    m_instructionSet = new AsmPicoBlazeInstructionSet ( compilerCore, opts, m_symbolTable );
    m_macros         = new AsmPicoBlazeMacros ( compilerCore, opts, m_symbolTable, m_codeListing );
    m_dgbFile        = new AsmDgbFileGen();
    m_memoryPtr      = new AsmPicoBlazeMemoryPtr ( compilerCore );
    m_specialMacros  = new AsmPicoBlazeSpecialMacros ( compilerCore, m_symbolTable, m_codeListing );

    m_device = DEV_UNSPEC;

    m_memoryPtr->clear();
}

AsmPicoBlazeSemanticAnalyzer::~AsmPicoBlazeSemanticAnalyzer()
{
    delete m_symbolTable;
    delete m_machineCode;
    delete m_codeListing;
    delete m_instructionSet;
    delete m_macros;
    delete m_dgbFile;
    delete m_memoryPtr;
    delete m_specialMacros;
}

void AsmPicoBlazeSemanticAnalyzer::printCodeTree ( const CompilerStatement * codeTree )
{
    if ( true == m_opts->m_codeTree.empty() )
    {
        return;
    }

    std::ofstream file ( m_opts->m_codeTree, ( std::fstream::out | std::fstream::trunc ) );

    if ( false == file.is_open() )
    {
        m_compilerCore -> compilerMessage ( CompilerBase::MT_ERROR,
                                            QObject::tr ( "Unable to open " )
                                                        . toStdString()
                                                        + "\"" + m_opts->m_codeTree  + "\"" );
        return;
    }

    file << codeTree;

    if ( true == file.fail() )
    {
        m_compilerCore -> compilerMessage ( CompilerBase::MT_ERROR,
                                            QObject::tr ( "Unable to write to " ).toStdString()
                                                        + "\"" + m_opts->m_codeTree  + "\"" );
        return;
    }
}

void AsmPicoBlazeSemanticAnalyzer::setDevice ( const std::string & deviceName )
{
    if ( "kcpsm2" == deviceName )
    {
        m_device = DEV_KCPSM2;
        m_instructionSet->setStrategy ( new AsmPicoBlazeInstructionSet2() );
    }
    else if ( "kcpsm3" == deviceName )
    {
        m_device = DEV_KCPSM3;
        m_instructionSet->setStrategy ( new AsmPicoBlazeInstructionSet3() );
    }
    else if ( "kcpsm6" == deviceName )
    {
        m_device = DEV_KCPSM6;
        m_instructionSet->setStrategy ( new AsmPicoBlazeInstructionSet6() );
    }
}

void AsmPicoBlazeSemanticAnalyzer::process ( CompilerStatement * codeTree )
{
    m_memoryPtr->clear();

    m_codeListing->loadSourceFiles();
    printCodeTree(codeTree);

    if ( false == phase1(codeTree) )
    {
        m_compilerCore->compilerMessage(CompilerBase::MT_ERROR,
                                        QObject::tr("the last error was critical, compilation aborted").toStdString());

        m_codeListing->output();
        m_macros->output();
        m_symbolTable->output();

        return;
    }

    phase2(codeTree);

    m_codeListing->output();
    m_macros->output();
    m_symbolTable->output();

    if ( true == m_compilerCore->successful() )
    {
        m_dgbFile->output(m_compilerCore, m_opts);
        m_machineCode->output(AsmMachineCodeGen::E_BIG_ENDIAN, m_compilerCore, m_opts);
    }
}

bool AsmPicoBlazeSemanticAnalyzer::phase1 ( CompilerStatement * codeTree,
                                            const CompilerSourceLocation * origLocation,
                                            const std::string * macroName )
{
    using namespace CompilerStatementTypes;

    std::vector<std::string> localSymbols;

    for ( CompilerStatement * node = codeTree->next();
          NULL != node;
          node = node->next() )
    {
        const CompilerSourceLocation * location = origLocation;
        if ( NULL == location )
        {
            location = &(node->location());
        }

        if ( true == m_instructionSet->isInstruction ( node ) )
        {
            m_instructionSet->encapsulate(node, m_memoryPtr->m_code);
            m_memoryPtr->m_code++;
            continue;
        }

        switch ( (int) node->type() )
        {
            case ASMPICOBLAZE_RT_COND:
            {
                CompilerStatement * body = new CompilerStatement();
                body->appendLink(m_specialMacros->runTimeCondition(node));

                if ( false == phase1(body, location) )
                {
                    body->completeDelete();
                    return false;
                }

                node->insertLink(body);
                break;
            }
            case ASMPICOBLAZE_DIR_RTWHILE:
            {
                CompilerStatement * body = new CompilerStatement();
                body->appendLink(m_specialMacros->runTimeWhile(node));

                if ( false == phase1(body, location) )
                {
                    body->completeDelete();
                    return false;
                }
                node->insertLink(body);
                break;
            }
            case ASMPICOBLAZE_DIR_RTFOR:
            {
                CompilerStatement * body = new CompilerStatement();
                body->appendLink(m_specialMacros->runTimeFor(node));

                if ( false == phase1(body, location) )
                {
                    body->completeDelete();
                    return false;
                }
                node->insertLink(body);
                break;
            }
            case ASMPICOBLAZE_DIR_DEVICE:
            {
                std::string deviceName = node->args()->lVal().m_data.m_symbol;
                for ( size_t i = 0; i < deviceName.size(); i++ )
                {
                    deviceName[i] = (char) tolower(deviceName[i]);
                }

                CompilerBase::DevSpecLoaderFlag loaderFlag;
                CompilerStatement * devSpecCode = m_compilerCore->loadDevSpecCode(deviceName, &loaderFlag);

                if ( NULL == devSpecCode )
                {
                    if ( CompilerBase::DSLF_DOES_NOT_EXIST == loaderFlag )
                    {
                            m_compilerCore->compilerMessage ( *location,
                                                              CompilerBase::MT_ERROR,
                                                              QObject::tr("Device not supported: ").toStdString()
                                                              + "\"" + deviceName + "\"" );
                    }
                    else if ( CompilerBase::DSLF_ALREADY_LOADED == loaderFlag )
                    {
                        m_compilerCore->compilerMessage ( *location,
                                                          CompilerBase::MT_ERROR,
                                                          QObject::tr ( "Device specification code is already "
                                                                        "loaded" ).toStdString() );
                    }
                    return false;
                }

                node->insertLink(devSpecCode->next());
                delete devSpecCode;
                break;
            }
            case ASMPICOBLAZE_DIR_LIMIT:
            {
                const char * limSel = node->args()->lVal().m_data.m_symbol;
                int limVal = m_symbolTable->resolveExpr ( node->args()->next() );

                if ( -1 == limVal )
                {
                    m_compilerCore->compilerMessage ( *location,
                                                      CompilerBase::MT_REMARK,
                                                      QObject::tr("limit value -1 means unlimited").toStdString() );
                }
                else if ( -1 > limVal )
                {
                    m_compilerCore->compilerMessage ( *location,
                                                      CompilerBase::MT_ERROR,
                                                      QObject::tr("limit value %1 is not valid").toStdString() );
                    break;
                }

                if ( 0 != strcmp("R", limSel) )
                {
                    m_opts->m_processorlimits.m_regFileSize = limVal;
                }
                else if ( 0 != strcmp("D", limSel) )
                {
                    m_opts->m_processorlimits.m_iDataMemSize = limVal;
                }
                else if ( 0 != strcmp("C", limSel) )
                {
                    m_opts->m_processorlimits.m_iCodeMemSize = limVal;
                }

                break;
            }
            case ASMPICOBLAZE_DIR_CODE:
            case ASMPICOBLAZE_DIR_PORT:
            case ASMPICOBLAZE_DIR_DATA:
            case ASMPICOBLAZE_DIR_REG:
            case ASMPICOBLAZE_DIR_EQU:
            {
                AsmPicoBlazeSymbolTable::SymbolType symbolType;
                switch ( (int) node->type() )
                {
                    case ASMPICOBLAZE_DIR_CODE:
                        symbolType = AsmPicoBlazeSymbolTable::STYPE_LABEL;
                        break;
                    case ASMPICOBLAZE_DIR_DATA:
                        symbolType = AsmPicoBlazeSymbolTable::STYPE_DATA;
                        break;
                    case ASMPICOBLAZE_DIR_PORT:
                        symbolType = AsmPicoBlazeSymbolTable::STYPE_PORT;
                        break;
                    case ASMPICOBLAZE_DIR_REG:
                        symbolType = AsmPicoBlazeSymbolTable::STYPE_REGISTER;
                        break;
                    case ASMPICOBLAZE_DIR_EQU:
                        symbolType = AsmPicoBlazeSymbolTable::STYPE_NUMBER;
                        break;
                    default:
                        symbolType = AsmPicoBlazeSymbolTable::STYPE_UNSPECIFIED;
                        break;
                }
                int value = m_symbolTable -> addSymbol ( node->args()->lVal().m_data.m_symbol,
                                                         node->args()->m_next,
                                                         location,
                                                         symbolType,
                                                         true );

                m_codeListing->setValue(node->location(), value);

                if ( ( value >= 0 ) && ( -1 != location->m_fileNumber ) )
                {
                    if ( node->type() == ASMPICOBLAZE_DIR_REG )
                    {
                        m_memoryPtr -> tryReserve ( *location, AsmPicoBlazeMemoryPtr::MS_REG, (unsigned int) value );
                    }
                    else if ( node->type() == ASMPICOBLAZE_DIR_DATA )
                    {
                        m_memoryPtr -> tryReserve ( *location, AsmPicoBlazeMemoryPtr::MS_DATA, (unsigned int) value );
                    }
                }
                break;
            }
            case ASMPICOBLAZE_DIR_SET:
            {
                int value;
                std::string name = node->args()->lVal().m_data.m_symbol;

                if ( true == m_symbolTable->isDefined(name) )
                {
                    value = m_symbolTable -> assignValue ( name,
                                                           node->args()->next(),
                                                           location,
                                                           AsmPicoBlazeSymbolTable::STYPE_NUMBER,
                                                           true );
                }
                else
                {
                    value = m_symbolTable -> addSymbol ( name,
                                                         node->args()->next(),
                                                         location,
                                                         AsmPicoBlazeSymbolTable::STYPE_NUMBER,
                                                         true,
                                                         true );
                }
                m_codeListing->setValue(node->location(), value);
                break;
            }
            case ASMPICOBLAZE_DIR_DEFINE:
            {
                int value = m_symbolTable -> addSymbol ( node->args()->lVal().m_data.m_symbol,
                                                         node->args()->next(),
                                                         location,
                                                         AsmPicoBlazeSymbolTable::STYPE_EXPRESSION,
                                                         false );
                m_codeListing->setValue(node->location(), value);
                break;
            }
            case ASMPICOBLAZE_DIR_UNDEFINE:
                m_symbolTable -> removeSymbol ( node->args()->lVal().m_data.m_symbol,
                                                node->location() );
                break;
            case ASMPICOBLAZE_LABEL:
            {
                CompilerExpr e(m_memoryPtr->m_code);
                m_symbolTable -> addSymbol ( node->args()->lVal().m_data.m_symbol,
                                             &e,
                                             location,
                                             AsmPicoBlazeSymbolTable::STYPE_LABEL,
                                             true );
                break;
            }
            case ASMPICOBLAZE_DIR_ORG:
                m_memoryPtr->m_code = m_symbolTable->resolveExpr(node->args());
                m_codeListing->setValue(node->location(), m_memoryPtr->m_code);
                node->args()->completeDelete();
                node->m_args = new CompilerExpr(m_memoryPtr->m_code);
                continue; // This directive must stay in the code tree until phase 2.
            case ASMPICOBLAZE_DIR_SKIP:
            {
                int skip = m_symbolTable->resolveExpr(node->args());
                m_memoryPtr->m_code += skip;
                node->args()->completeDelete();
                node->m_args = new CompilerExpr(skip);
                continue; // This directive must stay in the code tree until phase 2.
            }
            case ASMPICOBLAZE_LOCAL:
                if ( true == m_macros->isFromMacro(node) )
                {
                    const std::string local = node->args()->lVal().m_data.m_symbol;
                    if ( false == m_macros -> mangleName ( *location,
                                                           &localSymbols,
                                                           local,
                                                           *macroName,
                                                           node->next() ) )
                    {
                        m_compilerCore->compilerMessage ( *location,
                                                          CompilerBase::MT_WARNING,
                                                          QObject::tr ( "symbol `%1' declared as local but never used, "
                                                                        "declaration ignored"  )
                                                                       . arg ( local.c_str() )
                                                                       . toStdString() );
                    }
                }
                else
                {
                    m_compilerCore->compilerMessage ( node->location(),
                                                      CompilerBase::MT_ERROR,
                                                      QObject::tr ( "directive `LOCAL' cannot apper outside macro "
                                                                    "definition" )
                                                                  . toStdString() );
                }
                break;
            case ASMPICOBLAZE_DIR_EXITM:
                if ( true == m_macros->isFromMacro(node) )
                {
                    node->m_prev = NULL;
                    node->completeDelete();
                    return true;
                }
                else
                {
                    m_compilerCore->compilerMessage(node->location(),
                                                    CompilerBase::MT_ERROR,
                                                    QObject::tr("directive EXITM' cannot apper outside macro definition")
                                                               .toStdString());
                    break;
                }
            case ASMPICOBLAZE_INCLUDE:
                m_codeListing -> setInclusion ( node->location(),
                                                (int) (node->args()->m_lValue.m_data.m_integer) );
                break;
            case ASMPICOBLAZE_DIR_END:
                break;
            case ASMPICOBLAZE_DIR_MACRO:
            {
                m_macros -> define ( node->location(),
                                     node->args()->lVal().m_data.m_symbol,
                                     node->args()->m_next,
                                     node->branch() );
                node->m_branch = NULL;
                break;
            }
            case ASMPICOBLAZE_MACRO:
            {
                if ( -1 != m_opts->m_maxMacroExp && node->m_userData >= m_opts->m_maxMacroExp )
                {
                    m_compilerCore -> compilerMessage ( node->location(),
                                                        CompilerBase::MT_ERROR,
                                                        QObject::tr ( "maximum macro expansion level (%1) reached " )
                                                                    . arg ( m_opts->m_maxMacroExp )
                                                                    . toStdString() );
                }
                else
                {
                    const std::string nameOfMacro = node->args()->lVal().m_data.m_symbol;

                    CompilerStatement * macro = m_macros -> expand ( *location,
                                                                     node->location(),
                                                                     nameOfMacro,
                                                                     node->args()->next() );

                    if ( CompilerStatementTypes::EMPTY_STATEMENT == macro->type() )
                    {
                        macro->completeDelete();
                        break;
                    }

                    //
                    macro->prependLink(new CompilerStatement());
                    macro = macro->first();
                    if ( false == phase1(macro, location, &nameOfMacro) )
                    {
                        macro->completeDelete();
                        return false;
                    }

                    node = node->prev();
                    delete node->next();

                    CompilerStatement * nodeNext = macro->last();
                    node->insertLink(macro);
                    node = nodeNext;
                }
                continue;
            }
            case ASMPICOBLAZE_DIR_WHILE:
            {
                if ( node->branch()->last() == node->branch() )
                {
                    break;
                }

                CompilerSourceLocation lastLocation = node->branch()->last()->location();
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
                        body = new CompilerStatement();
                        body->appendLink(node->branch()->copyEntireChain());
                        m_codeListing->repeatCode(lastLocation, body, true);
                        lastLocation = body->lastLeaf()->location();
                        if ( false == phase1(body, location) )
                        {
                            body->completeDelete();
                            return false;
                        }
                    }
                    else
                    {
                        CompilerStatement * exp = new CompilerStatement();
                        exp->appendLink(node->branch()->copyEntireChain());
                        m_codeListing->repeatCode(lastLocation, exp, false);
                        lastLocation = exp->lastLeaf()->location();
                        if ( false == phase1(exp, location) )
                        {
                            body->completeDelete();
                            exp->completeDelete();
                            return false;
                        }
                        body->appendLink(exp);
                    }
                }

                if ( MAX_WHILE_ITERATIONS == i )
                {
                    m_compilerCore -> compilerMessage ( *location,
                                                        CompilerBase::MT_ERROR,
                                                        QObject::tr ( "maximum number of WHILE directive iterations "
                                                                      "(%1) reached" )
                                                                    . arg ( MAX_WHILE_ITERATIONS )
                                                                    . toStdString() );
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
            case ASMPICOBLAZE_DIR_REPT:
            {
                if ( node->branch()->last() == node->branch() )
                {
                    break;
                }

                unsigned int times = m_symbolTable->resolveExpr(node->args());
                if ( 0 == times )
                {
                    break;
                }
                if ( times >= MAX_REPEAT_ITERATIONS )
                {
                    m_compilerCore -> compilerMessage ( *location,
                                                        CompilerBase::MT_ERROR,
                                                        QObject::tr("maximum number of REPEAT directive iterations (%1)"
                                                                    " reached")
                                                                   .arg(MAX_REPEAT_ITERATIONS).toStdString() );
                }

                CompilerSourceLocation lastLocation = node->branch()->last()->location();
                delete node->branch()->last(); // remove `ENDR' directive

                CompilerStatement * body = new CompilerStatement();
                body->appendLink(node->branch()->copyEntireChain());
                m_codeListing->repeatCode(lastLocation, body, true);
                lastLocation = body->lastLeaf()->location();
                if ( false == phase1(body, location) )
                {
                    body->completeDelete();
                    return false;
                }

                for ( unsigned int i = 1; i < times; i++ )
                {
                    CompilerStatement * exp = new CompilerStatement();
                    exp->appendLink(node->branch()->copyEntireChain());
                    m_codeListing->repeatCode(lastLocation, exp, false);
                    lastLocation = exp->lastLeaf()->location();
                    if ( false == phase1(exp, location) )
                    {
                        body->completeDelete();
                        exp->completeDelete();
                        return false;
                    }
                    body->appendLink(exp);
                }

                node = node->prev();
                delete node->next();
                CompilerStatement * nodeNext = body->last();
                node->insertLink(body);
                node = nodeNext;
                continue;
            }
            case ASMPICOBLAZE_DIR_LIST:
                m_codeListing->setNoList(node->location(), false);
                break;
            case ASMPICOBLAZE_DIR_NOLIST:
                m_codeListing->setNoList(node->location(), true);
                break;
            case ASMPICOBLAZE_DIR_EXPAND:
                m_macros->setExpEnabled(true);
                break;
            case ASMPICOBLAZE_DIR_NOEXPAND:
                m_macros->setExpEnabled(false);
                break;
            case ASMPICOBLAZE_DIR_TITLE:
            {
                const CompilerValue::Data::CharArray & argCharArray = node->args()->lVal().m_data.m_array;
                std::string arg ( (char*) argCharArray.m_data, argCharArray.m_size );
                m_codeListing->setTitle(arg);
                break;
            }
            case ASMPICOBLAZE_DIR_MESSG:
            {
                const CompilerValue::Data::CharArray & argCharArray = node->args()->lVal().m_data.m_array;
                std::string arg ( (char*) argCharArray.m_data, argCharArray.m_size );
                m_compilerCore->compilerMessage(*location, CompilerBase::MT_REMARK, arg);
                break;
            }
            case ASMPICOBLAZE_DIR_WARNING:
            {
                const CompilerValue::Data::CharArray & argCharArray = node->args()->lVal().m_data.m_array;
                std::string arg ( (char*) argCharArray.m_data, argCharArray.m_size );
                m_compilerCore->compilerMessage(*location, CompilerBase::MT_WARNING, arg);
                break;
            }
            case ASMPICOBLAZE_DIR_ERROR:
            {
                const CompilerValue::Data::CharArray & argCharArray = node->args()->lVal().m_data.m_array;
                std::string arg ( (char*) argCharArray.m_data, argCharArray.m_size );
                m_compilerCore->compilerMessage(*location, CompilerBase::MT_ERROR, arg);
                break;
            }
            case ASMPICOBLAZE_DIR_DB:
            {
                std::vector<unsigned char> dbData;

                for ( CompilerExpr * arg = node->args();
                      NULL != arg;
                      arg = arg->next() )
                {
                    if ( CompilerValue::TYPE_ARRAY == arg->lVal().m_type )
                    {
                        const CompilerValue::Data::CharArray & charArray = arg->lVal().m_data.m_array;
                        for ( int i = 0; i < charArray.m_size; i++ )
                        {
                            dbData.push_back ( charArray.m_data[i] );
                        }
                    }
                    else
                    {
                        int value = m_symbolTable->resolveExpr(arg);
                        if ( value & 0xff0000 )
                        {
                            dbData.push_back ( ( value >> 16 ) & 0xff );
                        }
                        if ( value & 0xff00 )
                        {
                            dbData.push_back ( ( value >> 8 ) & 0xff );
                        }
                        dbData.push_back ( value & 0xff );
                    }
                }

                node->args()->completeDelete();
                node->m_args = NULL;

                m_memoryPtr->m_code += ( dbData.size() / 3 );
                m_memoryPtr->m_code += ( 0 == ( dbData.size() % 3 ) ? 0 : 1);

                for ( size_t i = 0; i < dbData.size(); i++ )
                {
                    unsigned int code = ( dbData[i] << 16 );

                    i++;
                    if ( i < dbData.size() )
                    {
                        code |= ( dbData[i] << 8 );
                    }

                    i++;
                    if ( i < dbData.size() )
                    {
                        code |= dbData[i];
                    }

                    if ( 0 != ( code & ~0x3ffff ) )
                    {
                        m_compilerCore -> compilerMessage ( *location,
                                                            CompilerBase::MT_WARNING,
                                                            QObject::tr ( "instruction word is only 18 bits wide, value"
                                                                          " `%1' trimmed to `%2'" )
                                                                        . arg ( code )
                                                                        . arg ( code & 0x3ffff )
                                                                        . toStdString() );
                        code &= 0x3ffff;
                    }

                    if ( NULL == node->m_args )
                    {
                        node->m_args = new CompilerExpr((int)code);
                    }
                    else
                    {
                        node->m_args->appendLink(new CompilerExpr((int)code));
                    }
                }

                // This directive must stay in the code tree until phase 2.
                continue;
            }
            case ASMPICOBLAZE_DIR_AUTOSPR:
            case ASMPICOBLAZE_DIR_AUTOREG:
            {
                int addr;
                AsmPicoBlazeSymbolTable::SymbolType symbolType;

                if ( ASMPICOBLAZE_DIR_AUTOSPR == node->type() )
                {
                    if ( NULL != node->args()->next() )
                    {
                        m_memoryPtr->m_data = m_symbolTable->resolveExpr(node->args()->next());
                    }

                    addr = m_memoryPtr->m_data;
                    m_memoryPtr->m_data++;
                    symbolType = AsmPicoBlazeSymbolTable::STYPE_DATA;

                    m_memoryPtr -> tryReserve ( *location, AsmPicoBlazeMemoryPtr::MS_DATA, addr );
                }
                else
                {
                    if ( NULL != node->args()->next() )
                    {
                        m_memoryPtr->m_reg = m_symbolTable->resolveExpr(node->args()->next());
                    }

                    addr = m_memoryPtr->m_reg;
                    m_memoryPtr->m_reg++;
                    symbolType = AsmPicoBlazeSymbolTable::STYPE_REGISTER;

                    m_memoryPtr -> tryReserve ( *location, AsmPicoBlazeMemoryPtr::MS_REG, addr );
                }

                CompilerExpr value(addr);
                m_symbolTable -> addSymbol ( node->args()->lVal().m_data.m_symbol,
                                             &value,
                                             location,
                                             symbolType,
                                             true );
                m_codeListing->setValue(node->location(), addr);
                break;
            }
            case ASMPICOBLAZE_COND_ASM:
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

    return true;
}

inline void AsmPicoBlazeSemanticAnalyzer::phase2 ( CompilerStatement * codeTree )
{
    using namespace CompilerStatementTypes;

    m_symbolTable->maskNonLabels();

    for ( CompilerStatement * node = codeTree->next();
          NULL != node;
          node = node->next() )
    {
        switch ( node->type() )
        {
            case EMPTY_STATEMENT:
                // Do not interpret empty statements and let them be deleted (later in this method).
                break;

            case ASMPICOBLAZE_DIR_ORG:
                m_machineCode->setOrigin((uint32_t)m_symbolTable->resolveExpr(node->args()));
                break;

            case ASMPICOBLAZE_DIR_SKIP:
                m_machineCode->incrementAddr((unsigned int)m_symbolTable->resolveExpr(node->args()));
                break;

            case ASMPICOBLAZE_DIR_DB:
                for ( CompilerExpr * arg = node->args();
                      NULL != arg;
                      arg = arg->next() )
                {
                    long long code = arg->lVal().m_data.m_integer;
                    int address = m_machineCode->setCode ( (uint32_t) code );
                    m_codeListing -> setCode ( node->location(), (int) code, address );
                    m_dgbFile -> setCode ( node->location(), (int) code, address );
                    m_memoryPtr -> tryReserve ( node->location(), AsmPicoBlazeMemoryPtr::MS_CODE, address );
                }
                break;

            default: // Only instructions are expected here, anything else will be ignored and deleted.
            {
                int opcode = m_instructionSet->resolveOPcode(node);
                if ( -1 == opcode )
                {
                    m_compilerCore->compilerMessage ( node->location(),
                                                      CompilerBase::MT_ERROR,
                                                      QObject::tr ( "instruction not supported on the this device: " )
                                                                  . toStdString()
                                                                  + m_instructionSet->getInstructionName(node) );
                }
                else
                {
                    int address = m_machineCode -> setCode ( opcode );
                    m_codeListing -> setCode ( node->location(), opcode, address );
                    m_dgbFile -> setCode ( node->location(), opcode, address );
                    m_memoryPtr->tryReserve ( node->location(), AsmPicoBlazeMemoryPtr::MS_CODE, address );
                }
                break;
            }
        }

        // Remove the inspected node.
        node = node->prev();
        delete node->next();
    }
}

CompilerStatement * AsmPicoBlazeSemanticAnalyzer::conditionalCompilation ( CompilerStatement * ifTree )
{
    using namespace CompilerStatementTypes;

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
            case ASMPICOBLAZE_DIR_IF:
            case ASMPICOBLAZE_DIR_ELSEIF:
                conditionVal = m_symbolTable->resolveExpr(node->args());
                break;

            case ASMPICOBLAZE_DIR_IFN:
            case ASMPICOBLAZE_DIR_ELSEIFN:
                conditionVal = m_symbolTable->resolveExpr(node->args());
                conditionVal = ( ( 0 == conditionVal ) ? 1 : 0 );
                break;

            case ASMPICOBLAZE_DIR_IFDEF:
            case ASMPICOBLAZE_DIR_ELSEIFDEF:
                if ( CompilerValue::TYPE_EMPTY == node->args()->lVal().m_type )
                {
                    conditionVal = 0;
                }
                else if ( true == m_symbolTable->isDefined(node->args()->lVal().m_data.m_symbol) )
                {
                    conditionVal = 1;
                }
                else
                {
                    conditionVal = 0;
                }
                break;

            case ASMPICOBLAZE_DIR_IFNDEF:
            case ASMPICOBLAZE_DIR_ELSEIFNDEF:
                if ( CompilerValue::TYPE_EMPTY == node->args()->lVal().m_type )
                {
                    conditionVal = 1;
                }
                else if ( true == m_symbolTable->isDefined(node->args()->lVal().m_data.m_symbol) )
                {
                    conditionVal = 0;
                }
                else
                {
                    conditionVal = 1;
                }
                break;

            case ASMPICOBLAZE_DIR_IFB:
            case ASMPICOBLAZE_DIR_ELSEIFB:
                conditionVal = ( ( CompilerValue::TYPE_EMPTY == node->args()->lVal().m_type ) ? 1 : 0 );
                break;

            case ASMPICOBLAZE_DIR_IFNB:
            case ASMPICOBLAZE_DIR_ELSEIFNB:
                conditionVal = ( ( CompilerValue::TYPE_EMPTY == node->args()->lVal().m_type ) ? 0 : 1 );
                break;

            case ASMPICOBLAZE_DIR_ELSE:
                conditionVal = 1;
                break;

            case ASMPICOBLAZE_DIR_ENDIF:
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
