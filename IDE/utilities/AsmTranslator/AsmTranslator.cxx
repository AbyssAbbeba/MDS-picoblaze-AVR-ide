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

bool AsmTranslator::translate ( Variant variant,
                                std::ostream & output,
                                std::istream & input )
{
    bool success = true;
    AsmTranslatorBase * translator = NULL;

    switch ( variant )
    {
        case V_KCPSM_XILINX:
            translator = new AsmTranslatorKcpsmXil;
            break;
    }

    if ( NULL == translator )
    {
        m_messages.push_back ( QObject::tr("Error: input failure.").toStdString() );
        return false;
    }

    std::string line;
    while ( false == input.eof() )
    {
        std::getline(input, line);
        if ( true == input.bad() )
        {
            m_messages.push_back ( QObject::tr("Error: input failure.").toStdString() );
            success = false;
            break;
        }
        if ( false == ( success = translator->process(m_messages, line) ) )
        {
            break;
        }
        output << line << std::endl;
        if ( true == output.bad() )
        {
            m_messages.push_back ( QObject::tr("Error: output failure.").toStdString() );
            success = false;
            break;
        }
    }

    delete translator;
    return success;
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
