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
 * @ingroup Compiler
 * @file Compiler.cxx
 */
// =============================================================================

#include "Compiler.h"

// Compiler header files.
#include "core/CompilerCore.h"
#include "core/CompilerMsgInterface.h"
#include "modules/CompilerModules.h"

// Boost Filesystem library.
#define BOOST_FILESYSTEM_NO_DEPRECATED
#include "boost/filesystem.hpp"

Compiler::Compiler ( CompilerMsgInterface * msgInterface,
                     std::string baseIncludeDir )
                   : m_compilerCore ( new CompilerCore(msgInterface) )
{
    setBaseIncludeDir(baseIncludeDir);
}

Compiler::~Compiler()
{
    delete m_compilerCore;
}

void Compiler::setBaseIncludeDir ( const std::string & directory )
{
    m_compilerCore->setBaseIncludeDir(directory);
}

DbgFile * Compiler::getSimDbg()
{
    return m_compilerCore->getSimDbg();
}

DataFile * Compiler::getSimData()
{
    return m_compilerCore->getSimData();
}

bool Compiler::compile ( CompilerBase::LangId lang,
                         CompilerBase::TargetArch arch,
                         CompilerOptions * const opts,
                         bool genSimData )
{
    using namespace CompilerModules;

    try
    {
        if ( false == checkOptions(lang, arch, opts) )
        {
            return false;
        }

        m_compilerCore->setup(lang, arch, opts, genSimData);

        std::string errStr;
        ModEmplStatCode statusCode = employModule ( lang, arch, m_compilerCore, &errStr );

        switch ( statusCode )
        {
            case MESC_OK:
                return m_compilerCore->successful();
            case MESC_IO_ERROR:
                m_compilerCore->coreMessage ( CompilerBase::MT_ERROR,
                                              QObject::tr("unable to open file: ").toStdString() + errStr );
                return false;
            case MESC_ARCH_NOT_SUPPORTED:
                m_compilerCore->coreMessage ( CompilerBase::MT_ERROR,
                                              QObject::tr ( "architecture not supported for the selected language" )
                                                          . toStdString() );
                return false;
            case MESC_LANG_NOT_SUPPORTED:
                m_compilerCore->coreMessage ( CompilerBase::MT_ERROR,
                                              QObject::tr("programming language not supported").toStdString() );
                return false;
            case MESC_UNKNOWN_ERROR:
                return false;
        }
    }
    catch ( const boost::system::error_code & e )
    {
        m_compilerCore->coreMessage ( CompilerBase::MT_ERROR,
                                      QObject::tr("failure: %1").arg(e.message().c_str()).toStdString() );
        return false;
    }
    catch ( const boost::filesystem::filesystem_error & e )
    {
        m_compilerCore->coreMessage ( CompilerBase::MT_ERROR,
                                      QObject::tr("failure: %1").arg(e.what()).toStdString() );
        return false;
    }

    return false;
}

inline bool Compiler::checkOptions ( CompilerBase::LangId lang,
                                     CompilerBase::TargetArch arch,
                                     const CompilerOptions * opts )
{
    if ( CompilerBase::LI_INVALID == lang )
    {
        m_compilerCore->coreMessage ( CompilerBase::MT_ERROR,
                                      QObject::tr("programming language not specified").toStdString() );
        return false;
    }

    if ( CompilerBase::TA_INVALID == arch )
    {
        m_compilerCore->coreMessage ( CompilerBase::MT_ERROR,
                                      QObject::tr("target architecture not specified").toStdString() );
        return false;
    }

    if ( true == opts->m_sourceFiles.empty() )
    {
        m_compilerCore->coreMessage ( CompilerBase::MT_ERROR,
                                      QObject::tr("source code file not specified").toStdString() );
        return false;
    }
    else
    {
        for ( const auto & file : opts->m_sourceFiles )
        {
            if ( true == file.empty() )
            {
                m_compilerCore->coreMessage ( CompilerBase::MT_ERROR,
                                              QObject::tr("empty string used as source code file name").toStdString() );
                return false;
            }
        }
    }

    return true;
}
