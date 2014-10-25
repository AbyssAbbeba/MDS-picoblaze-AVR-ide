// =============================================================================
/**
 * @brief
 * C++ Implementation: ...
 *
 * ...
 *
 * (C) copyright 2014 Moravia Microsystems, s.r.o.
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
#include "AdjSimProcDefParser.h"

// OS compatibility.
#include "utilities/os/os.h"

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
    m_treeDecoder    = new AsmAdaptableTreeDecoder ( this );

    m_deviceSet = false;
    m_memoryPtr->clear();
    m_memoryPtr->m_hardLimits.m_reg  = 0;
    m_memoryPtr->m_hardLimits.m_data = 0;
    m_memoryPtr->m_hardLimits.m_code = 0;
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

void AsmAdaptableSemanticAnalyzer::setDevice ( const std::string & deviceDefFile )
{
    using namespace boost::filesystem;


    path filenamePath = path(makeHomeSafe(deviceDefFile)).make_preferred();
    if ( false == filenamePath.is_absolute() )
    {
        path basePath = path(m_compilerCore->getFileName()).parent_path();
        filenamePath = ( basePath / filenamePath );
    }

    if ( true == m_deviceSet )
    {
        m_compilerCore -> semanticMessage ( CompilerSourceLocation(),
                                            CompilerBase::MT_ERROR,
                                            QObject::tr ( "cannot set device to %1, device definition already set")
                                                        . arg(filenamePath.string().c_str()).toStdString() );
        return;
    }
    m_deviceSet = true;

    std::ifstream file ( filenamePath.string(), (std::ios_base::in | std::ios_base::binary) );

    if ( false == file.is_open() )
    {
        m_compilerCore -> semanticMessage ( CompilerSourceLocation(),
                                            CompilerBase::MT_ERROR,
                                            QObject::tr ( "unable to open file " ).toStdString()
                                                        + '`' + filenamePath.string()  + '\'' );
        return;
    }

    static const long long int MAX_SIZE = 102400;
    char data [ MAX_SIZE ];
    size_t len = (size_t) file.readsome (data, MAX_SIZE);

    if ( true == file.bad() )
    {
        m_compilerCore -> semanticMessage ( CompilerSourceLocation(),
                                            CompilerBase::MT_ERROR,
                                            QObject::tr ( "unable to read file " ).toStdString()
                                                        + '`' + filenamePath.string()  + '\'' );
        return;
    }

    AdjSimProcDefParser parser(std::string(data, len));

    if ( false == parser.isValid() )
    {
        m_compilerCore -> semanticMessage ( CompilerSourceLocation(),
                                            CompilerBase::MT_ERROR,
                                            QObject::tr ( "invalid data in " ).toStdString()
                                                        + '`' + filenamePath.string()  + '\'' );
        return;
    }

    m_device = parser.data();

    m_memoryPtr->m_hardLimits.m_reg  = m_device.m_memory.m_register.m_size;
    m_memoryPtr->m_hardLimits.m_data = m_device.m_memory.m_data.m_size;
    m_memoryPtr->m_hardLimits.m_code = m_device.m_memory.m_program.m_size;
}

void AsmAdaptableSemanticAnalyzer::process ( CompilerStatement * codeTree )
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

    if ( true == m_compilerCore->successful() )
    {
        genMachineCode();
        m_dgbFile->output(m_compilerCore, m_opts);
    }
}

inline void AsmAdaptableSemanticAnalyzer::printCodeTree ( const CompilerStatement * codeTree )
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

inline void AsmAdaptableSemanticAnalyzer::saveHDL ( AsmMachineCodeGen::WordSize wordSize,
                                                    DataFile & dataFile,
                                                    const std::string & fileName )
{
    AsmMachineCodeGen::Endianness endianness = (
        ( AdjSimProcDef::Memory::Program::END_BIG == m_device.m_memory.m_program.m_endian )
            ?
        AsmMachineCodeGen::E_BIG_ENDIAN
            :
        AsmMachineCodeGen::E_LITTLE_ENDIAN
    );

    try
    {
        m_machineCode->output(wordSize, endianness, &dataFile);
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

inline void AsmAdaptableSemanticAnalyzer::genMachineCode()
{
    unsigned int arrSize = 0;
    unsigned int bytesPerRecord = 0;
    unsigned int linesInTotal = ( m_device.m_memory.m_program.m_size / 16 );
    AsmMachineCodeGen::WordSize wordSize = AsmMachineCodeGen::WORD_2B;
    XilHDLFile::OPCodeSize opCodeSize = XilHDLFile::SIZE_16b;
    std::string vTemplate = "default.v";
    std::string vhdlTemplate = "default.vhd";
    std::string name = m_compilerCore->getBaseName();

    AsmMachineCodeGen::Endianness endianness = (
        ( AdjSimProcDef::Memory::Program::END_BIG == m_device.m_memory.m_program.m_endian )
            ?
        AsmMachineCodeGen::E_BIG_ENDIAN
            :
        AsmMachineCodeGen::E_LITTLE_ENDIAN
    );

    switch ( m_device.m_memory.m_program.m_word )
    {
        case AdjSimProcDef::Memory::Program::WORD_1B:
            wordSize = AsmMachineCodeGen::WORD_1B;
            bytesPerRecord = 1;
            break;
        case AdjSimProcDef::Memory::Program::WORD_2B:
            wordSize = AsmMachineCodeGen::WORD_2B;
            opCodeSize = XilHDLFile::SIZE_16b;
            bytesPerRecord = 2;
            break;
        case AdjSimProcDef::Memory::Program::WORD_3B:
            wordSize = AsmMachineCodeGen::WORD_3B;
            opCodeSize = XilHDLFile::SIZE_18b;
            bytesPerRecord = 3;
            break;
    }

    arrSize = ( m_device.m_memory.m_program.m_size * bytesPerRecord );

    m_machineCode -> output ( wordSize, endianness, m_compilerCore, m_opts );

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
