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
 * @ingroup AsmTranslator
 * @file AsmTranslator.cxx
 */
// =============================================================================

#include "AsmTranslator.h"

// AsmTranslator header files.
#include "AsmTranslatorBase.h"
#include "AsmTranslatorKcpsmXil.h"
#include "AsmTranslatorKcpsmMed.h"
#include "AsmTranslatorKcpsmPBIDE.h"

// Standard header files.
#include <cstdio>
#include <fstream>

// Used for i18n only.
#include <QObject>

AsmTranslator::~AsmTranslator()
{
}

bool AsmTranslator::translate ( Variant variant,
                                std::ostream & output,
                                std::istream & input )
{
    AsmTranslatorBase * translator = nullptr;

    switch ( variant )
    {
        case V_INVALID:
            break;
        case V_KCPSM_XILINX:
            translator = new AsmTranslatorKcpsmXil;
            break;
        case V_KCPSM_MEDIATRONIX:
            translator = new AsmTranslatorKcpsmMed;
            break;
        case V_KCPSM_OPENPICIDE:
            translator = new AsmTranslatorKcpsmPBIDE;
            break;
    }

    if ( nullptr == translator )
    {
        m_messages.push_back ( { 0,
                                 QObject::tr ( "Error: specified assembler variant is not supported." )
                                            . toStdString() } );
        return false;
    }

    translator->m_config = &m_config;
    bool result = translate(translator, output, input);
    delete translator;
    return result;
}

inline bool AsmTranslator::translate ( AsmTranslatorBase * translator,
                                       std::ostream & output,
                                       std::istream & input )
{
    if ( ( true == output.bad() ) || ( true == input.bad() ) )
    {
            m_messages.push_back ( { 0, QObject::tr("Error: I/O failure.").toStdString() } );
        return false;
    }

    // Read the entire input.
    std::vector<std::string> buffer;
    while ( false == input.eof() )
    {
        buffer.push_back(std::string());
        std::getline(input, buffer.back());
        if ( true == input.bad() )
        {
            m_messages.push_back ( { 0, QObject::tr("Error: input failure.").toStdString() } );
            return false;
        }
    }

    // Process the input in two steps.
    {
        bool success = true;
        unsigned int lineNumber = 1;

        for ( auto & line : buffer )
        {
            if ( false == translator->process(m_messages, line, lineNumber) )
            {
                success = false;
            }
            lineNumber++;
        }

        lineNumber = 1;
        for ( auto & line : buffer )
        {
            if ( false == translator->process(m_messages, line, lineNumber, true) )
            {
                success = false;
            }
            lineNumber++;
        }

        if ( false == success )
        {
            return false;
        }
    }

    // Write output.
    const char * eol = "\n";
    switch ( m_config.m_eol )
    {
        case AsmTranslatorConfig::EOF_LF:   eol = "\n";   break;
        case AsmTranslatorConfig::EOF_CR:   eol = "\r";   break;
        case AsmTranslatorConfig::EOF_CRLF: eol = "\r\n"; break;
    }
    for ( const auto line : translator->m_prologue )
    {
        output << line << eol;
        if ( true == output.bad() )
        {
            m_messages.push_back ( { 0, QObject::tr("Error: output failure.").toStdString() } );
            return false;
        }
    }
    for ( const auto line : buffer )
    {
        output << line << eol;
        if ( true == output.bad() )
        {
            m_messages.push_back ( { 0, QObject::tr("Error: output failure.").toStdString() } );
            return false;
        }
    }

    return true;
}

bool AsmTranslator::translate ( Variant variant,
                                const std::string & output,
                                const std::string & input,
                                bool makeBackup )
{
    if ( true == makeBackup )
    {
        rename(output.c_str(), (output + "~").c_str());
    }

    std::ifstream iFile(input);
    std::ofstream oFile(output, std::ios_base::trunc);

    if ( ( false == iFile.is_open() ) || ( false == oFile.is_open() ) )
    {
        m_messages.push_back ( { 0, QObject::tr("Error: Cannot open file(s).").toStdString() } );
        return false;
    }

    return translate(variant, oFile, iFile);
}

const std::vector<std::pair<unsigned int, std::string> > & AsmTranslator::getMessages() const
{
    return m_messages;
}

void AsmTranslator::clear()
{
    m_messages.clear();
}
