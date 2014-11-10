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
 * @file AsmPicoBlazeSemanticAnalyzer.cxx
 */
// =============================================================================

#include "AsmPicoBlazeSemanticAnalyzer.h"

// Common compiler header files.
#include "AsmMacros.h"
#include "AsmMemoryPtr.h"
#include "AsmDgbFileGen.h"
#include "AsmStringTable.h"
#include "AsmSymbolTable.h"
#include "AsmCodeListing.h"
#include "CompilerStatementTypes.h"

// PicoBlaze assembler semantic analyzer header files.
#include "AsmPicoBlazeCodeGenerator.h"
#include "AsmPicoBlazeTreeDecoder.h"
#include "AsmPicoBlazeSpecialMacros.h"
#include "AsmPicoBlazeInstructionSet.h"
#include "AsmPicoBlazeInstructionSet6.h"
#include "AsmPicoBlazeInstructionSet3.h"
#include "AsmPicoBlazeInstructionSet2.h"
#include "AsmPicoBlazeInstructionSet1.h"
#include "AsmPicoBlazeInstructionSet1CPLD.h"

// Header files of libMCUDataFiles.
#include "XilMemFile.h"
#include "XilVHDLFile.h"
#include "XilVerilogFile.h"
#include "RawHexDumpFile.h"

// Standard headers.
#include <string>
#include <vector>
#include <cctype>
#include <cstdint>
#include <fstream>
#include <cstring>
#include <iostream>

AsmPicoBlazeSemanticAnalyzer::AsmPicoBlazeSemanticAnalyzer ( CompilerSemanticInterface * compilerCore,
                                                             CompilerOptions * opts )
                                                           : CompilerSemanticAnalyzer ( compilerCore, opts )
{
    m_dgbFile        = new AsmDgbFileGen();
    m_machineCode    = new AsmMachineCodeGen();
    m_memoryPtr      = new AsmMemoryPtr ( compilerCore );
    m_symbolTable    = new AsmSymbolTable ( compilerCore, opts );
    m_stringTable    = new AsmStringTable ( compilerCore, opts );
    m_codeGenerator  = new AsmPicoBlazeCodeGenerator();
    m_codeListing    = new AsmCodeListing ( compilerCore, opts, m_symbolTable, m_codeGenerator );
    m_macros         = new AsmMacros ( compilerCore, opts, m_symbolTable, m_codeListing );
    m_specialMacros  = new AsmPicoBlazeSpecialMacros ( compilerCore, m_symbolTable, m_codeListing );
    m_instructionSet = new AsmPicoBlazeInstructionSet ( compilerCore, opts, m_symbolTable, &m_device );
    m_treeDecoder    = new AsmPicoBlazeTreeDecoder ( m_compilerCore,    m_opts,         m_dgbFile,
                                                     m_machineCode,     m_macros,       m_memoryPtr,
                                                     m_symbolTable,     m_codeListing,  m_specialMacros,
                                                     m_instructionSet,  m_stringTable,  m_device );

    m_device = DEV_UNSPEC;

    m_memoryPtr->clear();
    m_memoryPtr->m_hardLimits.m_reg  = 32;
    m_memoryPtr->m_hardLimits.m_data = 64;
    m_memoryPtr->m_hardLimits.m_code = 4096;
}

AsmPicoBlazeSemanticAnalyzer::~AsmPicoBlazeSemanticAnalyzer()
{
    delete m_macros;
    delete m_dgbFile;
    delete m_memoryPtr;
    delete m_treeDecoder;
    delete m_symbolTable;
    delete m_machineCode;
    delete m_codeListing;
    delete m_specialMacros;
    delete m_codeGenerator;
    delete m_instructionSet;
}

inline void AsmPicoBlazeSemanticAnalyzer::printCodeTree ( const CompilerStatement * codeTree )
{
    if ( true == m_opts->m_codeTree.empty() )
    {
        return;
    }

    std::ofstream file ( m_opts->m_codeTree, ( std::fstream::out | std::fstream::trunc ) );

    if ( false == file.is_open() )
    {
        m_compilerCore -> semanticMessage ( CompilerSourceLocation(),
                                            CompilerBase::MT_ERROR,
                                            QObject::tr ( "Unable to open " )
                                                        . toStdString()
                                                        + '"' + m_opts->m_codeTree  + '"' );
        return;
    }

    file << codeTree;

    if ( true == file.bad() )
    {
        m_compilerCore -> semanticMessage ( CompilerSourceLocation(),
                                            CompilerBase::MT_ERROR,
                                            QObject::tr ( "Unable to write to " ).toStdString()
                                                        + '"' + m_opts->m_codeTree  + '"' );
        return;
    }
}

void AsmPicoBlazeSemanticAnalyzer::setDevice ( const std::string & deviceName )
{
    if ( "kcpsm1cpld" == deviceName )
    {
        m_device = DEV_KCPSM1CPLD;
        m_memoryPtr->m_hardLimits.m_reg  = 8;
        m_memoryPtr->m_hardLimits.m_data = 0;
        m_memoryPtr->m_hardLimits.m_code = 256;
        m_instructionSet->setStrategy ( new AsmPicoBlazeInstructionSet1CPLD() );
    }
    else if ( "kcpsm1" == deviceName )
    {
        m_device = DEV_KCPSM1;
        m_memoryPtr->m_hardLimits.m_reg  = 16;
        m_memoryPtr->m_hardLimits.m_data = 0;
        m_memoryPtr->m_hardLimits.m_code = 256;
        m_instructionSet->setStrategy ( new AsmPicoBlazeInstructionSet1() );
    }
    else if ( "kcpsm2" == deviceName )
    {
        m_device = DEV_KCPSM2;
        m_memoryPtr->m_hardLimits.m_reg  = 32;
        m_memoryPtr->m_hardLimits.m_data = 0;
        m_memoryPtr->m_hardLimits.m_code = 1024;
        m_instructionSet->setStrategy ( new AsmPicoBlazeInstructionSet2() );
    }
    else if ( "kcpsm3" == deviceName )
    {
        m_device = DEV_KCPSM3;
        m_memoryPtr->m_hardLimits.m_reg  = 16;
        m_memoryPtr->m_hardLimits.m_data = 64;
        m_memoryPtr->m_hardLimits.m_code = 1024;
        m_instructionSet->setStrategy ( new AsmPicoBlazeInstructionSet3() );
    }
    else if ( "kcpsm6" == deviceName )
    {
        m_device = DEV_KCPSM6;
        m_memoryPtr->m_hardLimits.m_reg  = 32;
        m_memoryPtr->m_hardLimits.m_data = 256;
        m_memoryPtr->m_hardLimits.m_code = 4096;
        m_instructionSet->setStrategy ( new AsmPicoBlazeInstructionSet6() );
    }
}

void AsmPicoBlazeSemanticAnalyzer::process ( CompilerStatement * codeTree )
{
    m_memoryPtr->clear();

    m_codeListing->loadSourceFiles();
    printCodeTree(codeTree);

    if ( false == m_treeDecoder->phase1(codeTree) )
    {
        m_compilerCore->semanticMessage(CompilerSourceLocation(),
                                        CompilerBase::MT_ERROR,
                                        QObject::tr("the last error was critical, compilation aborted").toStdString());

        m_codeListing->output();
        m_macros->output();
        m_symbolTable->output();

        return;
    }

    m_treeDecoder->phase2(codeTree);
    m_treeDecoder->phase3();

    m_codeListing->output();
    m_macros->output();
    m_symbolTable->output();
    m_stringTable->output();

    if ( true == m_compilerCore->successful() )
    {
        genMachineCode();
        m_dgbFile->output(m_compilerCore, m_opts);
    }
}

inline void AsmPicoBlazeSemanticAnalyzer::genMachineCode()
{
    unsigned int arrSize = 0;
    unsigned int bytesPerRecord = 0;
    unsigned int linesInTotal = 0;
    AsmMachineCodeGen::WordSize wordSize = AsmMachineCodeGen::WORD_3B;
    XilHDLFile::OPCodeSize opCodeSize = XilHDLFile::SIZE_18b;
    std::string vTemplate;
    std::string vhdlTemplate;
    std::string name = m_compilerCore->getBaseName();

    switch ( m_device )
    {
        case DEV_UNSPEC:
            return;

        case DEV_KCPSM1CPLD:
            bytesPerRecord = 2;
            arrSize = ( 256 * bytesPerRecord );
            linesInTotal = 16;
            wordSize = AsmMachineCodeGen::WORD_2B;
            opCodeSize = XilHDLFile::SIZE_16b;
            vTemplate = "kcpsm1cpld.v";
            vhdlTemplate = "kcpsm1cpld.vhd";
            break;

        case DEV_KCPSM1:
            bytesPerRecord = 2;
            arrSize = ( 256 * bytesPerRecord );
            linesInTotal = 16;
            wordSize = AsmMachineCodeGen::WORD_2B;
            opCodeSize = XilHDLFile::SIZE_16b;
            vTemplate = "kcpsm1.v";
            vhdlTemplate = "kcpsm1.vhd";
            break;

        case DEV_KCPSM2:
            bytesPerRecord = 3;
            arrSize = ( 1024 * bytesPerRecord );
            linesInTotal = 64;
            wordSize = AsmMachineCodeGen::WORD_3B;
            opCodeSize = XilHDLFile::SIZE_18b;
            vTemplate = "kcpsm2.v";
            vhdlTemplate = "kcpsm2.vhd";
            break;

        case DEV_KCPSM3:
            bytesPerRecord = 3;
            arrSize = ( 1024 * bytesPerRecord );
            linesInTotal = 64;
            wordSize = AsmMachineCodeGen::WORD_3B;
            opCodeSize = XilHDLFile::SIZE_18b;
            vTemplate = "kcpsm3.v";
            vhdlTemplate = "kcpsm3.vhd";
            break;

        case DEV_KCPSM6:
            bytesPerRecord = 3;
            arrSize = ( 4096 * bytesPerRecord );
            linesInTotal = 256;
            wordSize = AsmMachineCodeGen::WORD_3B;
            opCodeSize = XilHDLFile::SIZE_18b;
            vTemplate = "kcpsm6.v";
            vhdlTemplate = "kcpsm6.vhd";
            break;
    }

    m_machineCode -> output ( wordSize,
                              AsmMachineCodeGen::E_BIG_ENDIAN,
                              m_compilerCore,
                              m_opts );

    if ( false == m_opts->m_verilogTemplate.empty() )
    {
        vTemplate = m_opts->m_verilogTemplate;
    }
    else
    {
        vTemplate.insert(0, m_compilerCore->getBaseIncludeDir());
    }
    if ( false == m_opts->m_vhdlTemplate.empty() )
    {
        vhdlTemplate = m_opts->m_vhdlTemplate;
    }
    else
    {
        vhdlTemplate.insert(0, m_compilerCore->getBaseIncludeDir());
    }

    if ( false == m_opts->m_memFile.empty() )
    {
        XilMemFile dataFile(bytesPerRecord, arrSize, linesInTotal);
        saveHDL(wordSize, dataFile, m_opts->m_memFile);
    }
    if ( false == m_opts->m_verilogFile.empty() )
    {
        XilVerilogFile dataFile(name, vTemplate, opCodeSize, arrSize);
        saveHDL(wordSize, dataFile, m_opts->m_verilogFile);
    }
    if ( false == m_opts->m_vhdlFile.empty() )
    {
        XilVHDLFile dataFile(name, vhdlTemplate, opCodeSize, arrSize);
        saveHDL(wordSize, dataFile, m_opts->m_vhdlFile);
    }
    if ( false == m_opts->m_rawHexDumpFile.empty() )
    {
        RawHexDumpFile dataFile(RawHexDumpFile::OPCodeSize(opCodeSize), arrSize);
        saveHDL(wordSize, dataFile, m_opts->m_rawHexDumpFile);
    }
}

inline void AsmPicoBlazeSemanticAnalyzer::saveHDL ( AsmMachineCodeGen::WordSize wordSize,
                                                    DataFile & dataFile,
                                                    const std::string & fileName )
{
    try
    {
        m_machineCode->output(wordSize, AsmMachineCodeGen::E_BIG_ENDIAN, &dataFile);
        dataFile.save(fileName, false);
    }
    catch ( const DataFileException & e )
    {
        std::cerr << e.toString() << std::endl;
        m_compilerCore -> semanticMessage ( CompilerSourceLocation(),
                                            CompilerBase::MT_ERROR,
                                            QObject::tr ( "unable to save file " ).toStdString()
                                                        + '`' + fileName  + '\'' );
    }
}
