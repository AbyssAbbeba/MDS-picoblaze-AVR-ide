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
 * @ingroup PicoBlazeAsm
 * @file AsmPicoBlazeTreeDecoder.cxx
 */
// =============================================================================

#include "AsmPicoBlazeTreeDecoder.h"

// Common compiler header files.
#include "AsmDgbFileGen.h"
#include "CompilerExpr.h"
#include "CompilerOptions.h"
#include "CompilerStatement.h"
#include "AsmMachineCodeGen.h"
#include "CompilerSemanticInterface.h"

// PicoBlaze assembler semantic analyzer header files.
#include "AsmMacros.h"
#include "AsmMemoryPtr.h"
#include "AsmStringTable.h"
#include "AsmSymbolTable.h"
#include "AsmCodeListing.h"
#include "AsmPicoBlazeSpecialMacros.h"
#include "AsmPicoBlazeInstructionSet.h"

// Header files of libMCUDataFiles.
#include "HexFile.h"

// Standard headers.
#include <vector>
#include <iostream>
#include <algorithm>

#define HANDLE_ACTION(arg)                  \
    switch ( arg )                          \
    {                                       \
        case CA_NO_ACTION:    break;        \
        case CA_CONTINUE:     continue;     \
        case CA_RETURN_TRUE:  return true;  \
        case CA_RETURN_FALSE: return false; \
    }

AsmPicoBlazeTreeDecoder::AsmPicoBlazeTreeDecoder ( CompilerSemanticInterface            * compilerCore,
                                                   CompilerOptions                      * opts,
                                                   AsmDgbFileGen                        * dgbFile,
                                                   AsmMachineCodeGen                    * machineCode,
                                                   AsmMacros                            * macros,
                                                   AsmMemoryPtr                         * memoryPtr,
                                                   AsmSymbolTable                       * symbolTable,
                                                   AsmCodeListing                       * codeListing,
                                                   AsmPicoBlazeSpecialMacros            * specialMacros,
                                                   AsmPicoBlazeInstructionSet           * instructionSet,
                                                   AsmStringTable                       * stringTable,
                                                   AsmPicoBlazeSemanticAnalyzer::Device & device )
                                                 :
                                                   m_compilerCore   ( compilerCore   ),
                                                   m_opts           ( opts           ),
                                                   m_dgbFile        ( dgbFile        ),
                                                   m_machineCode    ( machineCode    ),
                                                   m_macros         ( macros         ),
                                                   m_memoryPtr      ( memoryPtr      ),
                                                   m_symbolTable    ( symbolTable    ),
                                                   m_codeListing    ( codeListing    ),
                                                   m_specialMacros  ( specialMacros  ),
                                                   m_instructionSet ( instructionSet ),
                                                   m_stringTable    ( stringTable    ),
                                                   m_device         ( device         )
{
    m_mergeAddr = -1;
    m_failjmp   = nullptr;
    m_forceNext = nullptr;
}

AsmPicoBlazeTreeDecoder::~AsmPicoBlazeTreeDecoder()
{
    if ( nullptr != m_failjmp )
    {
        delete m_failjmp;
    }
}

bool AsmPicoBlazeTreeDecoder::phase1 ( CompilerStatement * codeTree,
                                       const std::string * macroName )
{
    using namespace CompilerStatementTypes;
    std::vector<std::string> localSymbols;

    for ( CompilerStatement * node = codeTree->next();
          nullptr != node;
          node = ( ( nullptr == m_forceNext ) ? node : m_forceNext )->next() )
    {
        m_forceNext = nullptr;

        if ( true == m_instructionSet->isInstruction ( node ) )
        {
            m_instructionSet->encapsulate(node, m_memoryPtr->m_code);
            m_memoryPtr->m_code++;
            continue;
        }

        switch ( (int) node->type() )
        {
            case ASMPICOBLAZE_DIR_EQU:
            case ASMPICOBLAZE_DIR_REG:
            case ASMPICOBLAZE_DIR_CODE:
            case ASMPICOBLAZE_DIR_PORT:
            case ASMPICOBLAZE_DIR_PORTIN:
            case ASMPICOBLAZE_DIR_PORTOUT:
            case ASMPICOBLAZE_DIR_DATA:
                dir_EQU_etc(node);
                break;

            case ASMPICOBLAZE_DIR_STRING:
                dir_STRING(node);
                break;

            case ASMPICOBLAZE_DIR_AUTOREG:
            case ASMPICOBLAZE_DIR_AUTOSPR:
                dir_AUTOxxx(node);
                break;

            case ASMPICOBLAZE_LOCAL:
                dir_LOCAL ( node, macroName, localSymbols );
                break;

            case ASMPICOBLAZE_DIR_END:
                break;

            case ASMPICOBLAZE_DIR_OUTPUTK_STR_P: dir_OUTPUTK_STR_P(node); break;
            case ASMPICOBLAZE_DIR_LD_RET_SX_STR: dir_LD_RET_SX_STR(node); break;

            case ASMPICOBLAZE_MACRO:       HANDLE_ACTION( macro       ( node ) ); break;
            case ASMPICOBLAZE_DIR_RTFOR:   HANDLE_ACTION( dir_RTFOR   ( node ) ); break;
            case ASMPICOBLAZE_DIR_REPT:    HANDLE_ACTION( dir_REPT    ( node ) ); break;
            case ASMPICOBLAZE_DIR_WHILE:   HANDLE_ACTION( dir_WHILE   ( node ) ); break;
            case ASMPICOBLAZE_RT_COND:     HANDLE_ACTION( dir_RTCOND  ( node ) ); break;
            case ASMPICOBLAZE_DIR_DEVICE:  HANDLE_ACTION( dir_DEVICE  ( node ) ); break;
            case ASMPICOBLAZE_DIR_RTWHILE: HANDLE_ACTION( dir_RTWHILE ( node ) ); break;
            case ASMPICOBLAZE_DIR_EXITM:   HANDLE_ACTION( dir_EXITM   ( node ) ); break;

            case ASMPICOBLAZE_DIR_DB:       dir_DB       ( node ); continue;
            case ASMPICOBLAZE_DIR_ORG:      dir_ORG      ( node ); continue;
            case ASMPICOBLAZE_DIR_SKIP:     dir_SKIP     ( node ); continue;
            case ASMPICOBLAZE_COND_ASM:     dir_IF       ( node ); break;
            case ASMPICOBLAZE_DIR_LIST:     dir_LIST     ( node ); break;
            case ASMPICOBLAZE_DIR_NOLIST:   dir_NOLIST   ( node ); break;
            case ASMPICOBLAZE_DIR_MACRO:    dir_MACRO    ( node ); break;
            case ASMPICOBLAZE_DIR_TITLE:    dir_TITLE    ( node ); break;
            case ASMPICOBLAZE_DIR_ENTITY:   dir_ENTITY   ( node ); break;
            case ASMPICOBLAZE_INCLUDE:      dir_INCLUDE  ( node ); break;
            case ASMPICOBLAZE_DIR_UNDEFINE: dir_UNDEFINE ( node ); break;
            case ASMPICOBLAZE_LABEL:        label        ( node ); break;
            case ASMPICOBLAZE_DIR_SET:      dir_SET      ( node ); break;
            case ASMPICOBLAZE_DIR_LIMIT:    dir_LIMIT    ( node ); break;
            case ASMPICOBLAZE_DIR_ERROR:    dir_ERROR    ( node ); break;
            case ASMPICOBLAZE_DIR_MESSG:    dir_MESSG    ( node ); break;
            case ASMPICOBLAZE_DIR_DEFINE:   dir_DEFINE   ( node ); break;
            case ASMPICOBLAZE_DIR_WARNING:  dir_WARNING  ( node ); break;
            case ASMPICOBLAZE_DIR_FAILJMP:  dir_FAILJMP  ( node ); break;
            case ASMPICOBLAZE_DIR_ORGSPR:   dir_ORGSPR   ( node ); break;
            case ASMPICOBLAZE_DIR_INITSPR:  dir_INITSPR  ( node ); break;
            case ASMPICOBLAZE_DIR_MERGESPR: dir_MERGESPR ( node ); break;

            case ASMPICOBLAZE_DIR_EXPAND:   dir_EXPAND   (); break;
            case ASMPICOBLAZE_DIR_NOEXPAND: dir_NOEXPAND (); break;

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

void AsmPicoBlazeTreeDecoder::phase2 ( CompilerStatement * codeTree )
{
    using namespace CompilerStatementTypes;

    m_symbolTable->maskNonLabels();

    for ( CompilerStatement * node = codeTree->next();
          nullptr != node;
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
                      nullptr != arg;
                      arg = arg->next() )
                {
                    long long code = arg->lVal().m_data.m_integer;
                    int address = m_machineCode->setCode ( (uint32_t) code );
                    m_codeListing -> setCode ( node->location(), (int) code, address );
                    m_dgbFile -> setCode ( node->location(), (int) code, address );
                    m_memoryPtr -> tryReserve ( node->location(), AsmMemoryPtr::MS_CODE, address );
                }
                break;

            default: // Only instructions are expected here, anything else will be ignored and deleted.
            {
                int opcode = m_instructionSet->resolveOPcode(node);
                if ( -1 == opcode )
                {
                    m_compilerCore->semanticMessage ( node->location(),
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
                    m_memoryPtr->tryReserve ( node->location(), AsmMemoryPtr::MS_CODE, address );
                }
                break;
            }
        }

        // Remove the inspected node.
        node = node->prev();
        delete node->next();
    }
}

void AsmPicoBlazeTreeDecoder::phase3()
{
    if ( false == m_sprInit.empty() )
    {
        if ( -1 == m_mergeAddr )
        {
            if ( true == m_opts->m_second.empty() )
            {
                m_compilerCore->semanticMessage ( CompilerSourceLocation(),
                                                  CompilerBase::MT_WARNING,
                                                  QObject::tr ( "no target file for SPR initialization specified" )
                                                              . toStdString() );
            }
            else
            {
                HexFile hexFile;
                for ( const auto & i : m_sprInit )
                {
                    hexFile.set ( i.m_address, (uint8_t) i.m_octet );
                }

                try
                {
                    hexFile.save(m_opts->m_second, m_opts->m_makeBackupFiles);
                }
                catch ( const DataFileException & e )
                {
                    std::cerr << e.toString() << std::endl;
                    m_compilerCore->semanticMessage ( CompilerSourceLocation(),
                                                      CompilerBase::MT_ERROR,
                                                      QObject::tr ( "unable to save file `%1'" )
                                                                  . arg ( m_opts->m_second.c_str() )
                                                                  . toStdString() );
                }
            }
        }
        else
        {
            int size = -1;
            for ( const auto & i : m_sprInit )
            {
                if ( (int) i.m_address > size )
                {
                    size = (int) i.m_address;
                }
            }

            size++;
            size = ( size / 2 ) + ( size % 2 );

            const CompilerSourceLocation * location[size];
            unsigned int data[size];

            for ( int i = 0; i < size; i++ )
            {
                data[i] = 0;
                location[i] = nullptr;
            }

            unsigned int shift, addr, mask;
            for ( const auto & i : m_sprInit )
            {
                shift = ( ( i.m_address % 2 ) ? 8 : 0 );
                addr  = ( i.m_address / 2 );
                mask  = ( 0xff << shift );

                data[addr] &= ~mask;
                data[addr] |= ( i.m_octet << shift );

                location[addr] = &( i.m_location );
            }

            addr = m_mergeAddr;
            const CompilerSourceLocation * loc;
            CompilerSourceLocation emptyLocation;
            for ( int i = 0; i < size; i++, addr++ )
            {
                if ( nullptr == location[i] )
                {
                    loc = &emptyLocation;
                }
                else
                {
                    loc = location[i];
                }

                m_machineCode->setCode(addr, data[i]);
                m_dgbFile->setCode(*loc, data[i], addr);
                m_memoryPtr->tryReserve(*loc, AsmMemoryPtr::MS_CODE, addr);
            }
            for ( int i = size; i < ( m_opts->m_processorlimits.m_iDataMemSize / 2 ); i++, addr++ )
            {
                m_machineCode->setCode(addr, 0);
                m_dgbFile->setCode(m_mergeSprLoc, 0, addr);
                m_memoryPtr->tryReserve(m_mergeSprLoc, AsmMemoryPtr::MS_CODE, addr);
            }
        }

        m_sprInit.clear();
    }

    if ( nullptr != m_failjmp )
    {
        if ( true == m_opts->m_strict )
        {
            if ( AsmSymbolTable::STYPE_LABEL != m_symbolTable->getType(m_failjmp->args()) )
            {
                m_compilerCore->semanticMessage ( m_failjmp->location(),
                                                  CompilerBase::MT_WARNING,
                                                  QObject::tr("jump address is not specified by a label").toStdString(),
                                                  true );
            }
        }

        int memSize = m_opts->m_processorlimits.m_iCodeMemSize;
        if ( -1 == memSize )
        {
            m_compilerCore->semanticMessage ( m_failjmp->location(),
                                              CompilerBase::MT_ERROR,
                                              QObject::tr ( "no user defined limit for the program memory size, fail "
                                                            "jump cannot be used" ).toStdString() );
        }
        else
        {
            int opcode = m_instructionSet->resolveOPcode(m_failjmp);

            for ( int addr = 0; addr < memSize; addr++ )
            {
                if ( false == m_memoryPtr->isReserved ( AsmMemoryPtr::MS_CODE, (unsigned int) addr ) )
                {
                    m_machineCode->setCode(addr, opcode);
                    m_dgbFile->setCode(m_failjmp->location(), opcode, addr);
                }
            }
        }

        delete m_failjmp;
        m_failjmp = nullptr;
    }
}

inline void AsmPicoBlazeTreeDecoder::dir_IF ( CompilerStatement * rootNode )
{
    using namespace CompilerStatementTypes;

    CompilerStatement * ifTree = rootNode->branch();

    if ( nullptr == ifTree )
    {
        return;
    }

    unsigned int conditionVal = 0;
    for ( CompilerStatement * node = ifTree;
          nullptr != node;
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
                if ( true == isBlank(node->args()) )
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
                if ( true == isBlank(node->args()) )
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
                conditionVal = ( isBlank(node->args()) ? 1 : 0 );
                break;

            case ASMPICOBLAZE_DIR_IFNB:
            case ASMPICOBLAZE_DIR_ELSEIFNB:
                conditionVal = ( isBlank(node->args()) ? 0 : 1 );
                break;

            case ASMPICOBLAZE_DIR_ELSE:
                conditionVal = 1;
                break;

            case ASMPICOBLAZE_DIR_ENDIF:
                return;
        }

        m_codeListing->setValue(node->location(), conditionVal);

        if ( 0 != conditionVal )
        {
            rootNode->insertLink(node->branch());
            node->m_branch = nullptr;
            return;
        }
    }
}

inline void AsmPicoBlazeTreeDecoder::dir_AUTOxxx ( CompilerStatement * node )
{
    using namespace CompilerStatementTypes;

    int addr;
    AsmSymbolTable::SymbolType symbolType;

    if ( ASMPICOBLAZE_DIR_AUTOSPR == node->type() )
    {
        int size = 1;

        CompilerExpr * arg = node->args()->next();
        if ( nullptr != arg )
        {
            if ( nullptr != arg->next() )
            {
                size = m_symbolTable->resolveExpr(arg->next());

                if (
                       ( size <= 0 )
                           ||
                       (
                           ( -1 != m_opts->m_processorlimits.m_iDataMemSize )
                               &&
                           ( size >= m_opts->m_processorlimits.m_iDataMemSize )
                       )
                   )
                {
                    m_compilerCore -> semanticMessage ( arg->next()->location(),
                                                        CompilerBase::MT_ERROR,
                                                        QObject::tr("invalid size: %1").arg(size).toStdString() );
                    size = 1;
                }
            }
            else
            {
                m_memoryPtr->m_data = m_symbolTable->resolveExpr(arg);
            }
        }

        addr = m_memoryPtr->m_data;
        m_memoryPtr->m_data += size;
        symbolType = AsmSymbolTable::STYPE_DATA;

        for ( int i = 0; i < size; i++ )
        {
            m_memoryPtr -> tryReserve ( node->location(), AsmMemoryPtr::MS_DATA, ( addr + i ) );
        }
    }
    else
    {
        if ( nullptr != node->args()->next() )
        {
            m_memoryPtr->m_reg = m_symbolTable->resolveExpr(node->args()->next());
        }

        addr = m_memoryPtr->m_reg;
        m_memoryPtr->m_reg++;
        symbolType = AsmSymbolTable::STYPE_REGISTER;

        m_memoryPtr -> tryReserve ( node->location(), AsmMemoryPtr::MS_REG, addr );
    }

    const CompilerValue * val;
    for ( val = &( node->args()->lVal() );
          CompilerValue::TYPE_EXPR == val->m_type;
          val = &( val->m_data.m_expr->lVal() ) );
    std::string name ( val->m_data.m_symbol );

    if ( true == m_stringTable->find(name) )
    {
        m_compilerCore->semanticMessage ( node->location(),
                                          CompilerBase::MT_ERROR,
                                          QObject::tr("attempting to override string: %1")
                                                     .arg(name.c_str()).toStdString());
        return;
    }

    CompilerExpr value(addr);
    m_symbolTable -> addSymbol ( name, &value, &( node->location() ), symbolType, true );
    m_codeListing->setValue(node->location(), addr);
}

inline void AsmPicoBlazeTreeDecoder::dir_ORGSPR ( CompilerStatement * node )
{
    m_memoryPtr->m_data = m_symbolTable->resolveExpr(node->args());
}

inline void AsmPicoBlazeTreeDecoder::dir_INITSPR ( CompilerStatement * node )
{
    std::vector<unsigned char> initData;

    for ( CompilerExpr * arg = node->args()->next();
          nullptr != arg;
          arg = arg->next() )
    {
        if ( CompilerValue::TYPE_ARRAY == arg->lVal().m_type )
        {
            const CompilerValue::Data::CharArray & charArray = arg->lVal().m_data.m_array;
            for ( int i = 0; i < charArray.m_size; i++ )
            {
                initData.push_back ( charArray.m_data[i] );
            }
        }
        else
        {
            initData.push_back ( 0xff & m_symbolTable->resolveExpr(arg) );
        }
    }

    int size = (int) initData.size();
    int addr = m_memoryPtr->m_data;

    CompilerExpr value(addr);
    m_symbolTable -> addSymbol ( node->args()->lVal().m_data.m_symbol,
                                 &value,
                                 &( node->location() ),
                                 AsmSymbolTable::STYPE_DATA,
                                 true );
    m_codeListing->setValue(node->location(), addr);

    m_memoryPtr->m_data += size;
    for ( int i = 0; i < size; i++ )
    {
        m_memoryPtr->tryReserve( node->location(), AsmMemoryPtr::MS_DATA, addr);
        m_sprInit.push_back ( { node->location(), (unsigned int) addr, initData[i] } );
        addr++;
    }
}

inline void AsmPicoBlazeTreeDecoder::dir_MERGESPR ( CompilerStatement * node )
{
    m_mergeSprLoc = node->location();
    m_mergeAddr = m_symbolTable->resolveExpr(node->args());

    if ( m_mergeAddr <= 0 )
    {
        m_compilerCore -> semanticMessage ( node->location(),
                                            CompilerBase::MT_ERROR,
                                            QObject::tr ( "cannot merge DATA memory (SPR) with the CODE memory at "
                                                          "address: " )
                                                        . arg ( m_mergeAddr )
                                                        . toStdString() );
    }

    if (
           ( -1 != m_opts->m_processorlimits.m_iCodeMemSize )
               &&
           ( m_mergeAddr >= m_opts->m_processorlimits.m_iCodeMemSize )
       )
    {
        m_compilerCore -> semanticMessage ( node->location(),
                                            CompilerBase::MT_ERROR,
                                            QObject::tr ( "address is crossing CODE memory size boundary: %1" )
                                                        . arg ( m_mergeAddr )
                                                        . toStdString() );
        m_mergeAddr = -1;
    }
}

inline void AsmPicoBlazeTreeDecoder::dir_DB ( CompilerStatement * node )
{
    std::vector<unsigned char> dbData;

    for ( const CompilerExpr * argument = node->args();
          nullptr != argument;
          argument = argument->next() )
    {
        const CompilerExpr * arg;
        for ( arg = argument;
              ( CompilerExpr::OPER_NONE == arg->oper() ) && ( CompilerValue::TYPE_EXPR == arg->lVal().m_type );
              arg = arg->lVal().m_data.m_expr );

        const CompilerValue::Type type = arg->lVal().m_type;

        if ( CompilerValue::TYPE_ARRAY == type )
        {
            const CompilerValue::Data::CharArray & charArray = arg->lVal().m_data.m_array;
            for ( int i = 0; i < charArray.m_size; i++ )
            {
                dbData.push_back ( charArray.m_data[i] );
            }
        }
        else if ( CompilerValue::TYPE_SYMBOL == type )
        {
            std::string stringValue;
            if ( false == m_stringTable->get ( arg->lVal().m_data.m_symbol, stringValue, &(node->location()) ) )
            {
                return;
            }
            for ( const char c : stringValue )
            {
                dbData.push_back ( (unsigned char) c );
            }
        }
        else
        {
            int value = m_symbolTable->resolveExpr(arg);
            if ( ( true == isInstWord3B() ) && ( value & 0xff0000 ) )
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
    node->m_args = nullptr;

    if ( true == isInstWord3B() )
    {
        m_memoryPtr->m_code += ( dbData.size() / 3 );
        m_memoryPtr->m_code += ( 0 == ( dbData.size() % 3 ) ? 0 : 1);
    }
    else
    {
        m_memoryPtr->m_code += ( dbData.size() / 2 );
        m_memoryPtr->m_code += ( 0 == ( dbData.size() % 2 ) ? 0 : 1);
    }

    for ( size_t i = 0; i < dbData.size(); i++ )
    {
        unsigned int code = 0;

        if ( true == isInstWord3B() )
        {
            code |= ( dbData[i] << 16 );
            i++;
        }

        if ( i < dbData.size() )
        {
            code |= ( dbData[i] << 8 );
        }
        i++;

        if ( i < dbData.size() )
        {
            code |= dbData[i];
        }

        if ( ( true == isInstWord3B() ) && ( 0 != ( code & ~0x3ffff ) ) )
        {
            m_compilerCore -> semanticMessage ( node->location(),
                                                CompilerBase::MT_WARNING,
                                                QObject::tr ( "instruction word is only 18 bits wide, value"
                                                              " 0x%1 trimmed to 0x%2" )
                                                            . arg ( code, 0, 16 )
                                                            . arg ( (code & 0x3ffff), 5, 16, QLatin1Char('0') )
                                                            . toStdString() );
            code &= 0x3ffff;
        }

        if ( nullptr == node->m_args )
        {
            node->m_args = new CompilerExpr((int)code);
        }
        else
        {
            node->m_args->appendLink(new CompilerExpr((int)code));
        }
    }
}

inline AsmPicoBlazeTreeDecoder::CourseOfAction
       AsmPicoBlazeTreeDecoder::dir_REPT ( CompilerStatement * node )
{
    if ( node->branch()->last() == node->branch() )
    {
        return CA_CONTINUE;
    }

    unsigned int times = m_symbolTable->resolveExpr(node->args());
    if ( 0 == times )
    {
        return CA_CONTINUE;
    }
    if ( times >= MAX_REPEAT_ITERATIONS )
    {
        m_compilerCore -> semanticMessage ( node->location(),
                                            CompilerBase::MT_ERROR,
                                            QObject::tr("maximum number of REPEAT directive iterations (%1) reached")
                                                       .arg(MAX_REPEAT_ITERATIONS).toStdString(),
                                            true );
    }

    const CompilerSourceLocation location = node->branch()->last()->location();
    delete node->branch()->last(); // remove `ENDR' directive

    CompilerStatement * body = new CompilerStatement();
    body->appendLink(node->branch()->copyEntireChain());
    m_codeListing->repeatCode(location, body->next(), true);
    if ( false == phase1(body) )
    {
        body->completeDelete();
        return CA_RETURN_FALSE;
    }

    for ( unsigned int i = 1; i < times; i++ )
    {
        CompilerStatement * exp = new CompilerStatement();
        exp->appendLink(node->branch()->copyEntireChain());
        m_codeListing->repeatCode(location, exp->next(), false);
        if ( false == phase1(exp) )
        {
            body->completeDelete();
            exp->completeDelete();
            return CA_RETURN_FALSE;
        }
        body->appendLink(exp);
    }

    node = node->prev();
    delete node->next();
    m_forceNext = body->last();
    node->insertLink(body);

    return CA_CONTINUE;
}

inline AsmPicoBlazeTreeDecoder::CourseOfAction
       AsmPicoBlazeTreeDecoder::dir_WHILE ( CompilerStatement * node )
{
    if ( node->branch()->last() == node->branch() )
    {
        return CA_NO_ACTION;
    }

    const CompilerSourceLocation location = node->branch()->last()->location();
    delete node->branch()->last(); // remove `ENDW' directive

    unsigned int i;
    CompilerStatement * body = nullptr;
    for ( i = 0; i < MAX_WHILE_ITERATIONS; i++ )
    {
        if ( 0 == m_symbolTable->resolveExpr(node->args()) )
        {
            break;
        }

        if ( nullptr == body )
        {
            body = (new CompilerStatement())->appendLink(node->branch()->copyEntireChain())->first();
            m_codeListing->repeatCode(location, body->next(), true);
            if ( false == phase1(body) )
            {
                body->completeDelete();
                return CA_RETURN_FALSE;
            }
        }
        else
        {
            CompilerStatement * exp = (new CompilerStatement())->appendLink(node->branch()->copyEntireChain())->first();
            m_codeListing->repeatCode(location, exp->next(), false);
            if ( false == phase1(exp) )
            {
                body->completeDelete();
                exp->completeDelete();
                return CA_RETURN_FALSE;
            }
            body->appendLink(exp);
        }
    }

    if ( MAX_WHILE_ITERATIONS == i )
    {
        m_compilerCore -> semanticMessage ( node->location(),
                                            CompilerBase::MT_ERROR,
                                            QObject::tr ( "maximum number of WHILE directive iterations "
                                                          "(%1) reached" )
                                                        . arg ( MAX_WHILE_ITERATIONS )
                                                        . toStdString(),
                                            true );
    }

    if ( nullptr != body)
    {
        node = node->prev();
        delete node->next();
        m_forceNext = body->last();
        node->insertLink(body);
        return CA_CONTINUE;
    }
    else
    {
        return CA_NO_ACTION;
    }
}

inline AsmPicoBlazeTreeDecoder::CourseOfAction
       AsmPicoBlazeTreeDecoder::macro ( CompilerStatement * node )
{
    if ( ( -1 != m_opts->m_maxMacroExp )
             &&
         ( (int) ( 0xffff & node->m_userData ) >= m_opts->m_maxMacroExp ) )
    {
        m_compilerCore -> semanticMessage ( node->location(),
                                            CompilerBase::MT_ERROR,
                                            QObject::tr ( "maximum macro expansion depth (%1) reached " )
                                                        . arg ( m_opts->m_maxMacroExp )
                                                        . toStdString(),
                                            true );
        return CA_NO_ACTION;
    }

    CompilerExpr * name = node->args();
    while ( CompilerValue::TYPE_EXPR == name->lVal().m_type )
    {
        name = name->lVal().m_data.m_expr;
    }

    const std::string nameOfMacro = name->lVal().m_data.m_symbol;

    CompilerStatement * macro = m_macros -> expand ( node->location(),
                                                     nameOfMacro,
                                                     node->args()->next() );

    if ( CompilerStatementTypes::EMPTY_STATEMENT == macro->type() )
    {
        macro->completeDelete();
        return CA_NO_ACTION;
    }

    //
    macro = (new CompilerStatement())->appendLink(macro)->first();

    if ( false == phase1(macro, &nameOfMacro) )
    {
        macro->completeDelete();
        return CA_RETURN_FALSE;
    }

    node = node->prev();
    delete node->next();

    m_forceNext = macro->last();
    node->insertLink(macro);

    return CA_CONTINUE;
}

inline void AsmPicoBlazeTreeDecoder::dir_MACRO ( CompilerStatement * node )
{
    m_macros -> define ( node->location(),
                         node->args()->lVal().m_data.m_symbol,
                         node->args()->m_next,
                         node->branch() );
    node->m_branch = nullptr;
}

inline void AsmPicoBlazeTreeDecoder::dir_ENTITY ( CompilerStatement * node )
{
    const CompilerValue * nameVal;
    for ( nameVal = &( node->args()->lVal() );
          CompilerValue::TYPE_EXPR == nameVal->m_type;
          nameVal = &( nameVal->m_data.m_expr->lVal() ) );

    m_entityName = std::string( (char*) nameVal->m_data.m_array.m_data, nameVal->m_data.m_array.m_size );
}

inline void AsmPicoBlazeTreeDecoder::dir_TITLE ( CompilerStatement * node )
{
    const CompilerValue::Data::CharArray & argCharArray = node->args()->lVal().m_data.m_array;
    std::string arg ( (char*) argCharArray.m_data, argCharArray.m_size );
    m_codeListing->setTitle(arg);
}

inline void AsmPicoBlazeTreeDecoder::dir_MESSG ( CompilerStatement * node )
{
    const CompilerValue::Data::CharArray & argCharArray = node->args()->lVal().m_data.m_array;
    std::string arg ( (char*) argCharArray.m_data, argCharArray.m_size );
    m_compilerCore->semanticMessage(node->location(), CompilerBase::MT_REMARK, arg, true);
}

inline void AsmPicoBlazeTreeDecoder::dir_WARNING ( CompilerStatement * node )
{
    const CompilerValue::Data::CharArray & argCharArray = node->args()->lVal().m_data.m_array;
    std::string arg ( (char*) argCharArray.m_data, argCharArray.m_size );
    m_compilerCore->semanticMessage(node->location(), CompilerBase::MT_WARNING, arg, true);
}

inline void AsmPicoBlazeTreeDecoder::dir_ERROR ( CompilerStatement * node )
{
    const CompilerValue::Data::CharArray & argCharArray = node->args()->lVal().m_data.m_array;
    std::string arg ( (char*) argCharArray.m_data, argCharArray.m_size );
    m_compilerCore->semanticMessage(node->location(), CompilerBase::MT_ERROR, arg, true);
}

inline void AsmPicoBlazeTreeDecoder::dir_LIST ( CompilerStatement * node )
{
    m_codeListing->setNoList(node->location(), false);
}

inline void AsmPicoBlazeTreeDecoder::dir_NOLIST ( CompilerStatement * node )
{
    m_codeListing->setNoList(node->location(), true);
}

inline void AsmPicoBlazeTreeDecoder::dir_EXPAND()
{
    m_macros->setExpEnabled(true);
}

inline void AsmPicoBlazeTreeDecoder::dir_NOEXPAND()
{
    m_macros->setExpEnabled(false);
}

inline AsmPicoBlazeTreeDecoder::CourseOfAction
       AsmPicoBlazeTreeDecoder::dir_EXITM ( CompilerStatement * node )
{
    if ( false == m_macros->isFromMacro(node) )
    {
        m_compilerCore->semanticMessage ( node->location(),
                                          CompilerBase::MT_ERROR,
                                          QObject::tr ( "directive `EXITM' cannot appear outside macro definition")
                                                      .toStdString(),
                                          true );
        return CA_NO_ACTION;
    }
    else if ( true == m_specialMacros->isFromSpecMacro(node) )
    {
        m_compilerCore->semanticMessage ( node->location(),
                                          CompilerBase::MT_ERROR,
                                          QObject::tr ( "directive `EXITM' cannot appear inside special macro" )
                                                      . toStdString(),
                                          true );
        return CA_NO_ACTION;
    }

    node->prev()->m_next = nullptr;
    node->m_prev = nullptr;
    node->completeDelete();
    return CA_RETURN_TRUE;
}

inline void AsmPicoBlazeTreeDecoder::dir_INCLUDE ( CompilerStatement * node )
{
    m_codeListing -> setInclusion ( node,
                                    (int) (node->args()->m_lValue.m_data.m_integer) );
}

inline void AsmPicoBlazeTreeDecoder::dir_LOCAL ( CompilerStatement * node,
                                                 const std::string * macroName,
                                                 std::vector<std::string> & localSymbols )
{
    if ( false == m_macros->isFromMacro(node) )
    {
        m_compilerCore->semanticMessage ( node->location(),
                                          CompilerBase::MT_ERROR,
                                          QObject::tr ( "directive `LOCAL' cannot appear outside macro definition" )
                                                      . toStdString(),
                                          true );
    }

    const std::string local = node->args()->lVal().m_data.m_symbol;
    if ( false == m_macros -> mangleName ( node->location(),
                                           &localSymbols,
                                           local,
                                           *macroName,
                                           node->next() ) )
    {
        m_compilerCore->semanticMessage ( node->location(),
                                          CompilerBase::MT_WARNING,
                                          QObject::tr ( "symbol `%1' declared as local but never used, "
                                                        "declaration ignored"  )
                                                       . arg ( local.c_str() )
                                                       . toStdString() );
    }
}

inline void AsmPicoBlazeTreeDecoder::dir_SET ( CompilerStatement * node )
{
    const CompilerValue * nameVal;
    for ( nameVal = &( node->args()->lVal() );
          CompilerValue::TYPE_EXPR == nameVal->m_type;
          nameVal = &( nameVal->m_data.m_expr->lVal() ) );

    std::string name = nameVal->m_data.m_symbol;
    int value;

    if ( true == m_symbolTable->isDefined(name) )
    {
        value = m_symbolTable -> assignValue ( name,
                                               node->args()->next(),
                                               &( node->location() ),
                                               AsmSymbolTable::STYPE_NUMBER,
                                               true );
    }
    else
    {
        value = m_symbolTable -> addSymbol ( name,
                                             node->args()->next(),
                                             &( node->location() ),
                                             AsmSymbolTable::STYPE_NUMBER,
                                             true,
                                             true );
    }

    m_codeListing->setValue(node->location(), value);
}

inline void AsmPicoBlazeTreeDecoder::dir_DEFINE ( CompilerStatement * node )
{
    int value = m_symbolTable -> addSymbol ( node->args()->lVal().m_data.m_symbol,
                                             node->args()->next(),
                                             &( node->location() ),
                                             AsmSymbolTable::STYPE_EXPRESSION,
                                             false );

    m_codeListing->setValue(node->location(), value);
}

inline void AsmPicoBlazeTreeDecoder::dir_UNDEFINE ( CompilerStatement * node )
{
    m_symbolTable -> removeSymbol ( node->args()->lVal().m_data.m_symbol,
                                    node->location() );
}

inline void AsmPicoBlazeTreeDecoder::label ( CompilerStatement * node )
{
    CompilerExpr * label = node->args();
    while ( CompilerValue::TYPE_EXPR == label->lVal().m_type )
    {
        label = label->lVal().m_data.m_expr;
    }

    CompilerExpr e(m_memoryPtr->m_code);
    int value = m_symbolTable -> addSymbol ( label->lVal().m_data.m_symbol,
                                             &e,
                                             &( node->location() ),
                                             AsmSymbolTable::STYPE_LABEL,
                                             true );

    m_codeListing->setValue(node->location(), value);
}

inline void AsmPicoBlazeTreeDecoder::dir_ORG ( CompilerStatement * node )
{
    m_memoryPtr->m_code = m_symbolTable->resolveExpr(node->args());
    m_codeListing->setValue(node->location(), m_memoryPtr->m_code);
    node->args()->completeDelete();
    node->m_args = new CompilerExpr(m_memoryPtr->m_code);
}

inline void AsmPicoBlazeTreeDecoder::dir_SKIP ( CompilerStatement * node )
{
    int skip = m_symbolTable->resolveExpr(node->args());
    m_memoryPtr->m_code += skip;
    node->args()->completeDelete();
    node->m_args = new CompilerExpr(skip);
}

inline void AsmPicoBlazeTreeDecoder::dir_FAILJMP ( CompilerStatement * node )
{
    if ( nullptr != m_failjmp )
    {
        m_compilerCore->semanticMessage ( node->location(),
                                          CompilerBase::MT_ERROR,
                                          QObject::tr("only one fail jump may be specified in the code").toStdString(),
                                          true );
    }

    m_failjmp = node->copyChainLink();
    m_failjmp->m_type = CompilerStatementTypes::ASMPICOBLAZE_INS_JUMP_AAA;

    AsmSymbolTable::SymbolType argType = m_symbolTable->getType(node->args());

    if ( ( AsmSymbolTable::STYPE_LABEL != argType )
             &&
         ( AsmSymbolTable::STYPE_UNSPECIFIED != argType ) )
    {
        int targetAddr = m_symbolTable->resolveExpr(node->args());

        node->args()->completeDelete();
        node->m_args = new CompilerExpr(targetAddr, node->location());
    }
}

inline void AsmPicoBlazeTreeDecoder::dir_LIMIT ( CompilerStatement * node )
{
    const char * limSel = node->args()->lVal().m_data.m_symbol;
    int limVal = m_symbolTable->resolveExpr ( node->args()->next() );

    if ( -1 == limVal )
    {
        m_compilerCore->semanticMessage ( node->location(),
                                          CompilerBase::MT_REMARK,
                                          QObject::tr("limit value -1 means unlimited").toStdString(),
                                          true );
    }
    else if ( -1 > limVal )
    {
        m_compilerCore->semanticMessage ( node->location(),
                                          CompilerBase::MT_ERROR,
                                          QObject::tr("limit value %1 is not valid").arg(limVal).toStdString(),
                                          true );
        return;
    }

    if ( 0 == strcmp("R", limSel) )
    {
        m_opts->m_processorlimits.m_regFileSize = limVal;
    }
    else if ( 0 == strcmp("D", limSel) )
    {
        m_opts->m_processorlimits.m_iDataMemSize = limVal;
    }
    else if ( 0 == strcmp("C", limSel) )
    {
        m_opts->m_processorlimits.m_iCodeMemSize = limVal;
    }
}

inline void AsmPicoBlazeTreeDecoder::dir_OUTPUTK_STR_P ( CompilerStatement * node )
{
    using namespace CompilerStatementTypes;

    const CompilerValue * val;
    for ( val = &( node->args()->lVal() );
          CompilerValue::TYPE_EXPR == val->m_type;
          val = &( val->m_data.m_expr->lVal() ) );

    const CompilerValue::Type type = val->m_type;
    std::string stringValue;

    if ( CompilerValue::TYPE_SYMBOL == type )
    {
        if ( false == m_stringTable->get(val->m_data.m_symbol, stringValue, &(node->location())) )
        {
            m_compilerCore->semanticMessage ( node->location(),
                                              CompilerBase::MT_ERROR,
                                              QObject::tr ( "string %1 not defined" )
                                                          . arg ( val->m_data.m_symbol )
                                                          . toStdString () );
            return;
        }
    }
    else if ( CompilerValue::TYPE_ARRAY == type )
    {
        stringValue = std::string ( (const char*) val->m_data.m_array.m_data,
                                    (size_t) val->m_data.m_array.m_size );
    }

    CompilerStatement * instructions = new CompilerStatement();
    for ( const char c : stringValue )
    {
        CompilerExpr * pp = new CompilerExpr(node->args()->next()->copyChainLink(), node->args()->next()->location());
        CompilerExpr * kk = new CompilerExpr(int(c), node->args()->location());
        instructions->appendLink ( new CompilerStatement ( node->location(),
                                                           ASMPICOBLAZE_INS_OUTPUTK_KK_P,
                                                           kk->appendLink(pp)));
    }

    node->insertLink(instructions);
}

inline void AsmPicoBlazeTreeDecoder::dir_LD_RET_SX_STR ( CompilerStatement * node )
{
    using namespace CompilerStatementTypes;

    const CompilerValue * val;
    for ( val = &( node->args()->next()->lVal() );
          CompilerValue::TYPE_EXPR == val->m_type;
          val = &( val->m_data.m_expr->lVal() ) );

    const CompilerValue::Type type = val->m_type;
    std::string stringValue;

    if ( CompilerValue::TYPE_SYMBOL == type )
    {
        if ( false == m_stringTable->get(val->m_data.m_symbol, stringValue, &(node->location())) )
        {
            m_compilerCore->semanticMessage ( node->location(),
                                              CompilerBase::MT_ERROR,
                                              QObject::tr ( "string %1 not defined" )
                                                          . arg ( val->m_data.m_symbol )
                                                          . toStdString () );
            return;
        }
    }
    else if ( CompilerValue::TYPE_ARRAY == type )
    {
        stringValue = std::string ( (const char *) val->m_data.m_array.m_data,
                                    (size_t) val->m_data.m_array.m_size );
    }

    CompilerStatement * instructions = new CompilerStatement();
    for ( const char c : stringValue )
    {
        CompilerExpr * sx = new CompilerExpr(node->args()->copyChainLink(), node->args()->location());
        CompilerExpr * kk = new CompilerExpr(int(c), node->args()->next()->location());
        instructions->appendLink ( new CompilerStatement ( node->location(),
                                                           ASMPICOBLAZE_INS_LD_RET_SX_KK,
                                                           sx->appendLink(kk)));
    }

    node->insertLink(instructions);
}

inline void AsmPicoBlazeTreeDecoder::dir_STRING ( CompilerStatement * node )
{
    const CompilerValue * val;

    for ( val = &( node->args()->lVal() );
          CompilerValue::TYPE_EXPR == val->m_type;
          val = &( val->m_data.m_expr->lVal() ) );
    std::string name ( val->m_data.m_symbol );

    for ( val = &( node->args()->next()->lVal() );
          CompilerValue::TYPE_EXPR == val->m_type;
          val = &( val->m_data.m_expr->lVal() ) );
    std::string value ( (const char *) val->m_data.m_array.m_data, val->m_data.m_array.m_size );

    if ( true == m_symbolTable->isDefined(name) )
    {
        m_compilerCore->semanticMessage ( node->location(),
                                          CompilerBase::MT_ERROR,
                                          QObject::tr("attempting to override symbol: %1")
                                                     .arg(name.c_str()).toStdString());
        return;
    }

    m_stringTable->add(name, value, &(node->location()));
}

inline void AsmPicoBlazeTreeDecoder::dir_EQU_etc ( CompilerStatement * node )
{
    using namespace CompilerStatementTypes;

    AsmSymbolTable::SymbolType symbolType;

    switch ( (int) node->type() )
    {
        case ASMPICOBLAZE_DIR_CODE:
            symbolType = AsmSymbolTable::STYPE_LABEL;
            break;
        case ASMPICOBLAZE_DIR_DATA:
            symbolType = AsmSymbolTable::STYPE_DATA;
            break;
        case ASMPICOBLAZE_DIR_PORT:
            symbolType = AsmSymbolTable::STYPE_PORT;
            break;
        case ASMPICOBLAZE_DIR_PORTIN:
            symbolType = AsmSymbolTable::STYPE_PORTIN;
            break;
        case ASMPICOBLAZE_DIR_PORTOUT:
            symbolType = AsmSymbolTable::STYPE_PORTOUT;
            break;
        case ASMPICOBLAZE_DIR_REG:
            symbolType = AsmSymbolTable::STYPE_REGISTER;
            break;
        case ASMPICOBLAZE_DIR_EQU:
            symbolType = AsmSymbolTable::STYPE_NUMBER;
            break;
        default:
            symbolType = AsmSymbolTable::STYPE_UNSPECIFIED;
            break;
    }

    const CompilerValue * nameVal;
    for ( nameVal = &( node->args()->lVal() );
          CompilerValue::TYPE_EXPR == nameVal->m_type;
          nameVal = &( nameVal->m_data.m_expr->lVal() ) );

    std::string name = nameVal->m_data.m_symbol;

    if ( true == m_stringTable->find(name) )
    {
        m_compilerCore->semanticMessage ( node->location(),
                                          CompilerBase::MT_ERROR,
                                          QObject::tr("attempting to override string: %1")
                                                     .arg(name.c_str()).toStdString());
        return;
    }

    int value = m_symbolTable -> addSymbol ( name,
                                             node->args()->next(),
                                             &( node->location() ),
                                             symbolType,
                                             true );

    m_codeListing->setValue(node->location(), value);

    if ( ( value >= 0 ) && ( -1 != node->location().m_fileNumber ) )
    {
        if ( node->type() == ASMPICOBLAZE_DIR_REG )
        {
            m_memoryPtr -> tryReserve ( node->location(), AsmMemoryPtr::MS_REG, (unsigned int) value );
        }
        else if ( node->type() == ASMPICOBLAZE_DIR_DATA )
        {
            m_memoryPtr -> tryReserve ( node->location(), AsmMemoryPtr::MS_DATA, (unsigned int) value );
        }
    }
}

inline AsmPicoBlazeTreeDecoder::CourseOfAction
       AsmPicoBlazeTreeDecoder::dir_RTCOND ( CompilerStatement * node )
{
    if ( false == checkKcpsm3AndHigher(node->location(), "run-time conditions") )
    {
        return CA_NO_ACTION;
    }

    CompilerStatement * body = new CompilerStatement();
    body->appendLink(m_specialMacros->runTimeCondition(node));

    if ( false == phase1(body) )
    {
        body->completeDelete();
        return CA_RETURN_FALSE;
    }

    m_forceNext = body->last();
    node->insertLink(body);

    return CA_NO_ACTION;
}

inline AsmPicoBlazeTreeDecoder::CourseOfAction
       AsmPicoBlazeTreeDecoder::dir_RTWHILE ( CompilerStatement * node )
{
    if ( false == checkKcpsm3AndHigher(node->location(), "run-time while loops") )
    {
        return CA_NO_ACTION;
    }

    CompilerStatement * body = new CompilerStatement();
    body->appendLink(m_specialMacros->runTimeWhile(node));

    if ( false == phase1(body) )
    {
        body->completeDelete();
        return CA_RETURN_FALSE;
    }

    m_forceNext = body->last();
    node->insertLink(body);

    return CA_NO_ACTION;
}

inline AsmPicoBlazeTreeDecoder::CourseOfAction
       AsmPicoBlazeTreeDecoder::dir_RTFOR ( CompilerStatement * node )
{
    if ( false == checkKcpsm3AndHigher(node->location(), "run-time for loops") )
    {
        return CA_NO_ACTION;
    }

    CompilerStatement * body = new CompilerStatement();
    body->appendLink(m_specialMacros->runTimeFor(node));

    if ( false == phase1(body) )
    {
        body->completeDelete();
        return CA_RETURN_FALSE;
    }

    m_specialMacros->runTimeForLeave();

    m_forceNext = body->last();
    node->insertLink(body);

    return CA_NO_ACTION;
}

inline AsmPicoBlazeTreeDecoder::CourseOfAction
       AsmPicoBlazeTreeDecoder::dir_DEVICE ( CompilerStatement * node )
{
    std::string deviceName = node->args()->lVal().m_data.m_symbol;
    std::transform(deviceName.begin(), deviceName.end(), deviceName.begin(), ::tolower);
    CompilerBase::DevSpecLoaderFlag loaderFlag;
    CompilerStatement * devSpecCode = m_compilerCore->loadDevSpecCode(deviceName, &loaderFlag);

    if ( nullptr == devSpecCode )
    {
        if ( CompilerBase::DSLF_DOES_NOT_EXIST == loaderFlag )
        {
            m_compilerCore->semanticMessage ( node->location(),
                                              CompilerBase::MT_ERROR,
                                              QObject::tr("device not supported: ").toStdString()
                                              + '"' + deviceName + '"' );
            return CA_RETURN_FALSE;
        }
    }
    else
    {
        node->insertLink(devSpecCode->next());
        delete devSpecCode;
    }

    return CA_NO_ACTION;
}

inline bool AsmPicoBlazeTreeDecoder::isInstWord3B() const
{
    switch ( m_device )
    {
        case AsmPicoBlazeSemanticAnalyzer::DEV_UNSPEC:
        case AsmPicoBlazeSemanticAnalyzer::DEV_KCPSM1CPLD:
        case AsmPicoBlazeSemanticAnalyzer::DEV_KCPSM1:
        case AsmPicoBlazeSemanticAnalyzer::DEV_KCPSM2:
            return false;

        case AsmPicoBlazeSemanticAnalyzer::DEV_KCPSM3:
        case AsmPicoBlazeSemanticAnalyzer::DEV_KCPSM6:
        default:
            return true;
    }
}

bool AsmPicoBlazeTreeDecoder::checkKcpsm3AndHigher ( const CompilerSourceLocation & location,
                                                     const char * functionality )
{
    switch ( m_device )
    {
        case AsmPicoBlazeSemanticAnalyzer::DEV_KCPSM3:
        case AsmPicoBlazeSemanticAnalyzer::DEV_KCPSM6:
            return true;

        case AsmPicoBlazeSemanticAnalyzer::DEV_UNSPEC:
        case AsmPicoBlazeSemanticAnalyzer::DEV_KCPSM1CPLD:
        case AsmPicoBlazeSemanticAnalyzer::DEV_KCPSM1:
        case AsmPicoBlazeSemanticAnalyzer::DEV_KCPSM2:
        default:
            m_compilerCore->semanticMessage(location,
                                            CompilerBase::MT_ERROR,
                                            QObject::tr("assembler feature '%1' is supported only on KCPSM3 and higher")
                                                       .arg(functionality).toStdString(),
                                            true );
            return false;
    }
}

bool AsmPicoBlazeTreeDecoder::isBlank ( const CompilerExpr * expr ) const
{
    if ( CompilerExpr::OPER_NONE != expr->oper() )
    {
        return false;
    }

    if ( CompilerValue::TYPE_EMPTY == expr->lVal().m_type )
    {
        return true;
    }
    else if ( CompilerValue::TYPE_EXPR == expr->lVal().m_type )
    {
        return isBlank(expr->lVal().m_data.m_expr);
    }
    else
    {
        return false;
    }
}

const std::string & AsmPicoBlazeTreeDecoder::getEntityName ( const std::string & defaultValue ) const
{
    if ( true == m_entityName.empty() )
    {
        return defaultValue;
    }

    return m_entityName;
}
