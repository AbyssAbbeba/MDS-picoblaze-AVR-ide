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
 * @ingroup AsmTranslator
 * @file AsmTranslator.cxx
 */
// =============================================================================

#include "AsmTranslator.h"

// AsmTranslator header files.
#include "AsmTranslatorBase.h"
#include "AsmTranslatorKcpsmXil.h"

// Standard header files.
#include <cstdio>
#include <fstream>

// Used for i18n only.
#include <QObject>

AsmTranslator::~AsmTranslator()
{
}

inline bool AsmTranslator::translate ( AsmTranslatorBase * translator,
                                       std::ostream & output,
                                       std::istream & input )
{
    // Read the entire input.
    std::vector<std::string> buffer;
    while ( false == input.eof() )
    {
        buffer.push_back(std::string());
        std::getline(input, buffer.back());
        if ( true == input.bad() )
        {
            m_messages.push_back ( QObject::tr("Error: input failure.").toStdString() );
            return false;
        }
    }

    // Process the input in two steps.
    {
        bool success = true;
        for ( std::vector<std::string>::iterator line = buffer.begin();
              buffer.end() != line;
              line++ )
        {
            if ( false == translator->process(m_messages, *line) )
            {
                success = false;
            }
        }
        for ( std::vector<std::string>::iterator line = buffer.begin();
              buffer.end() != line;
              line++ )
        {
            if ( false == translator->process(m_messages, *line, true) )
            {
                success = false;
            }
        }
        if ( false == success )
        {
            return false;
        }
    }

    // Write output.
    for ( auto line : translator->m_prologue )
    {
        output << line << std::endl;
        if ( true == output.bad() )
        {
            m_messages.push_back ( QObject::tr("Error: output failure.").toStdString() );
            return false;
        }
    }
    for ( auto line : buffer )
    {
        output << line << std::endl;
        if ( true == output.bad() )
        {
            m_messages.push_back ( QObject::tr("Error: output failure.").toStdString() );
            return false;
        }
    }

    return true;
}

bool AsmTranslator::translate ( Variant variant,
                                std::ostream & output,
                                std::istream & input )
{
    AsmTranslatorBase * translator = NULL;

    switch ( variant )
    {
        case V_KCPSM_XILINX:
            translator = new AsmTranslatorKcpsmXil;
            break;
    }

    if ( NULL == translator )
    {
        m_messages.push_back ( QObject::tr("Error: unsupported assembler variant.").toStdString() );
        return false;
    }

    translator->m_config = &m_config;
    bool result = translate(translator, output, input);
    delete translator;
    return result;
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

    return translate(variant, oFile, iFile);
}

const std::vector<std::string> & AsmTranslator::getMessages() const
{
    return m_messages;
}

void AsmTranslator::clear()
{
    m_messages.clear();
}
