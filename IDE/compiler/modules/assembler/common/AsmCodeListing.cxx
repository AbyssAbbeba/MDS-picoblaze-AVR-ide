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
 * @ingroup Assembler
 * @file AsmCodeListing.cxx
 */
// =============================================================================

#include "AsmCodeListing.h"

//  assembler semantic analyzer header files.
#include "AsmSymbolTable.h"

// Standard headers.
#include <cstdio>
#include <cstdlib>
#include <fstream>

// getline() function.
#include "os/getline.h"

// Used for i18n only
#include <QObject>

AsmCodeListing::LstLine::LstLine()
{
    m_address    = -1;
    m_inclusion  = -1;
    m_value      = -1;
    m_message    = -1;
    m_noList     = 0;
}

AsmCodeListing::LstLine::LstLine ( const char * line )
{
    m_address    = -1;
    m_inclusion  = -1;
    m_value      = -1;
    m_message    = -1;
    m_noList     = 0;
    m_line       = line;
}

AsmCodeListing::Message::Message()
{
    m_type = CompilerBase::MT_INVALID;
    m_subsequent = false;
}

AsmCodeListing::Message::Message ( CompilerBase::MessageType type,
                                   const std::string & text,
                                   bool subsequent )
{
    m_type = type;
    m_text = text;
    m_subsequent = subsequent;
}

AsmCodeListing::AsmCodeListing ( CompilerSemanticInterface * compilerCore,
                                 const CompilerOptions * opts,
                                 AsmSymbolTable * symbolTable,
                                 AsmCodeGenerator * codeGenerator )
                               :
                                 m_compilerCore ( compilerCore ),
                                 m_opts ( opts ),
                                 m_symbolTable ( symbolTable ),
                                 m_codeGenerator ( codeGenerator )
{
    m_messageLimit = 0;
    m_compilerCore->registerMsgObserver(this);

    clear();
}

void AsmCodeListing::clear()
{
    m_msgCounter     = 0;
    m_numberOfFiles  = 0;
    m_numberOfMacros = 0;

    m_title.clear();
    m_listing.clear();
    m_messages.clear();
    m_files2skip.clear();
    m_messageQueue.clear();
    m_lastMsgPrefix.clear();
    m_locationTransMap.clear();
}

AsmCodeListing::~AsmCodeListing()
{
}

void AsmCodeListing::loadSourceFiles()
{
    char * line = nullptr;
    size_t bufSize = 0;
    ssize_t lineLen = -1;
    int fileNumber = -1;

    m_numberOfFiles = m_compilerCore->listSourceFiles().size();
    m_listing.resize(m_numberOfFiles);

    for ( const auto & file : m_compilerCore->listSourceFiles() )
    {
        fileNumber++;
        if ( nullptr == file.second )
        {
            m_files2skip.insert(fileNumber);
            continue;
        }

        rewind(file.second);

        if ( 0 != ferror(file.second) )
        {
            m_compilerCore -> semanticMessage ( CompilerSourceLocation(),
                                                CompilerBase::MT_ERROR,
                                                QObject::tr("unable to read file: ").toStdString()
                                                + "`" + file.first  + "'" );
            break;
        }

        // Iterate over lines in the file.
        while ( 0 < ( lineLen = getline(&line, &bufSize, file.second) ) )
        {
            // Dispose of the terminating EOL character sequence.
            if ( ( lineLen > 1 ) && ( '\r' == line[lineLen-2] ) )
            {
                line[lineLen-2] = '\0';
            }
            else if ( ( '\n' == line[lineLen-1] ) || ( '\r' == line[lineLen-1] ) )
            {
                line[lineLen-1] = '\0';
            }

            m_listing[fileNumber].push_back ( LstLine ( line ) );
        }
    }

    if ( nullptr != line )
    {
        free(line);
    }

    processMsgQueue();
}

void AsmCodeListing::printCodeListing ( std::ostream & out,
                                        bool & outputEnabled,
                                        unsigned int & lineNumber,
                                        unsigned int fileNumber,
                                        unsigned int inclusionLevel,
                                        unsigned int macroLevel ) const
{
    char buffer [ 16 ];
    std::string output;

    for ( const auto & lstLine : m_listing[fileNumber] )
    {
        lineNumber++;

        if ( 1 == lstLine.m_noList )
        {
            outputEnabled = true;
        }

        if ( true == outputEnabled )
        {
            output.clear();

            if ( ( -1 != lstLine.m_address ) || ( false == lstLine.m_code.empty() ) )
            {
                if ( -1 != lstLine.m_address )
                {
                    sprintf ( buffer, "%03X ", lstLine.m_address );
                    output += buffer;
                }
                else
                {
                    output += std::string(4, ' ');
                }

                if ( false == lstLine.m_code.empty() )
                {
                    sprintf ( buffer, "%05X ", lstLine.m_code[0] );
                    output += buffer;
                }
                else
                {
                    output += std::string(6, ' ');
                }
            }
            else if ( -1 != lstLine.m_value )
            {
                sprintf ( buffer, "  %05X   ", lstLine.m_value );
                output += buffer;
            }
            else
            {
                output += std::string(10, ' ');
            }

            if ( 0 != inclusionLevel )
            {
                sprintf ( buffer, "=%d ", inclusionLevel );
                output += buffer;
                if ( inclusionLevel < 10 )
                {
                    output += ' ';
                }
            }
            else
            {
                output += std::string(4, ' ');
            }

            sprintf ( buffer, "%6d ", lineNumber);
            output += buffer;

            if ( 0 != macroLevel )
            {
                sprintf ( buffer, "+%d ", macroLevel );
                output += buffer;
                if ( macroLevel < 10 )
                {
                    output += ' ';
                }
            }
            else
            {
                output += std::string(4, ' ');
            }

            output += lstLine.m_line;
            int newSize;
            for ( newSize = (int)output.size() - 1; newSize >= 0; newSize-- )
            {
                if ( ' ' != output[newSize] )
                {
                    break;
                }
            }
            output.resize( newSize + 1 );
            out << output << std::endl;

            int addr = lstLine.m_address + 1;
            for ( size_t j = 1; j < lstLine.m_code.size(); j++ )
            {
                char tmp[11];
                sprintf ( tmp, "%03X %05X", addr, lstLine.m_code[j] );
                out << tmp << std::endl;

                addr++;
            }

            if ( -1 != lstLine.m_message )
            {
                for ( const auto & msg : m_messages[lstLine.m_message] )
                {
                    if ( true == msg.m_text.empty() || CompilerBase::MT_INVALID == msg.m_type )
                    {
                        continue;
                    }

                    switch ( msg.m_type )
                    {
                        case CompilerBase::MT_INVALID:
                            break;
                        case CompilerBase::MT_GENERAL:
                            out << "G: ";
                            break;
                        case CompilerBase::MT_ERROR:
                            out << "E: ";
                            break;
                        case CompilerBase::MT_WARNING:
                            out << "W: ";
                            break;
                        case CompilerBase::MT_REMARK:
                            out << "R: ";
                            break;
                    }

                    out << msg.m_text << '.' << std::endl;;
                }
            }
        }

        if ( -1 == lstLine.m_noList )
        {
            outputEnabled = false;
        }

        for ( int i = 0; i < (int) lstLine.m_macro.size(); i++ )
        {
            int macro = lstLine.m_macro[i];

            if ( macro > 0 )
            {
                macro--;
            }
            else if ( macro < 0 )
            {
                macro = -( macro + 1 );
            }

            printCodeListing ( out, outputEnabled, lineNumber, macro, inclusionLevel, ( 1 + macroLevel ) );
        }

        if ( -1 != lstLine.m_inclusion )
        {
            printCodeListing(out, outputEnabled, lineNumber, lstLine.m_inclusion, ( 1 + inclusionLevel ), macroLevel);
        }
    }
}

void AsmCodeListing::output()
{
    if ( m_opts->m_lstFile.empty() )
    {
        return;
    }

    std::ofstream file ( m_opts->m_lstFile, ( std::fstream::out | std::fstream::trunc ) );

    if ( false == file.is_open() )
    {
        m_compilerCore -> semanticMessage ( CompilerSourceLocation(),
                                            CompilerBase::MT_ERROR,
                                            QObject::tr("unable to open ").toStdString() + '"'
                                            + m_opts -> m_lstFile  + '"' );
        return;
    }

    file << this;

    if ( true == file.bad() )
    {
        m_compilerCore -> semanticMessage ( CompilerSourceLocation(),
                                            CompilerBase::MT_ERROR,
                                            QObject::tr("unable to write to ").toStdString() + '"'
                                            + m_opts -> m_lstFile  + '"' );
        return;
    }
}

inline bool AsmCodeListing::checkLocation ( const CompilerSourceLocation & location,
                                            bool silent )
{
    return checkLocation(location.m_fileNumber, location.m_lineStart, silent);
}

inline bool AsmCodeListing::checkLocation ( int file,
                                            int line,
                                            bool silent )
{
    if ( m_files2skip.cend() != m_files2skip.find(file) )
    {
        return false;
    }

    if (
           ( -1 != file )
               &&
           ( line >= 0 )
               &&
           ( file <= (int) ( m_numberOfFiles + m_numberOfMacros - 1 ) )
               &&
           ( line <= int( m_listing[file].size() ) )
       )
    {
        // Location is valid.
        return true;
    }

    // Location is NOT valid.
    if ( ( false == silent ) && ( -1 != file ) )
    {
        m_compilerCore -> semanticMessage ( CompilerSourceLocation(),
                                            CompilerBase::MT_ERROR,
                                            QObject::tr ( "some of the source code files were aparently changed"
                                                          " during compilation" ).toStdString() );
    }

    return false;
}

inline bool AsmCodeListing::translateLocation ( const CompilerSourceLocation & location,
                                                int & file,
                                                int & line)
{
    if ( false == checkLocation(location) )
    {
        return false;
    }

    const auto translation = m_locationTransMap.find(location.m_origin);
    if ( m_locationTransMap.cend() != translation )
    {
        file = translation->second.first;
        line = location.m_lineStart - translation->second.second;
        return checkLocation(file, line);
    }
    else
    {
        file = location.m_fileNumber;
        line = location.m_lineStart - 1;
        return true;
    }
}

void AsmCodeListing::setTitle ( const std::string & title )
{
    m_title = title;
}

void AsmCodeListing::setNoList ( CompilerSourceLocation location,
                                 bool flag )
{
    int file, line;
    if ( true == translateLocation(location, file, line) )
    {
        m_listing[file][line].m_noList = ( ( true == flag ) ? -1 : 1 );
    }
}

void AsmCodeListing::setInclusion ( CompilerStatement * node,
                                    int fileNumber )
{
    if ( -1 != fileNumber )
    {
        int file, line;
        const CompilerSourceLocation & location = node->location();

        if ( true == translateLocation(location, file, line) )
        {
            m_listing[file][line].m_inclusion = fileNumber;

            int origin = m_compilerCore->locationTrack().add(location);
            m_locationTransMap[origin] = {fileNumber, 1};
            rewriteIncludeLoc(node->next(), fileNumber, origin);
        }
    }
}

void AsmCodeListing::rewriteIncludeLoc ( CompilerStatement * codeTree,
                                         const int fileNumber,
                                         const int origin )
{
    for ( CompilerStatement * node = codeTree;
          node != nullptr;
          node = node->next() )
    {
        if ( fileNumber != node->m_location.m_fileNumber )
        {
            break;
        }

        if ( true == node->location().isSet() )
        {
            node->m_location.m_origin = origin;
            m_symbolTable->rewriteExprLoc ( node->args(), origin );
        }

        rewriteIncludeLoc ( node->branch(), fileNumber, origin );
    }
}

void AsmCodeListing::setCode ( const CompilerSourceLocation & location,
                               int code,
                               int address )
{
    int file, line;
    if ( true == translateLocation(location, file, line) )
    {
        std::vector<int> & codeVector = m_listing[file][line].m_code;
        if ( true == codeVector.empty() )
        {
            m_listing[file][line].m_address = address;
        }
        codeVector.push_back(code);
    }
}

void AsmCodeListing::setValue ( CompilerSourceLocation location,
                                int value )
{
    int file, line;
    if ( true == translateLocation(location, file, line) )
    {
        m_listing[file][line].m_value = value;
    }
}

void AsmCodeListing::expandMacro ( const CompilerSourceLocation & location,
                                   CompilerStatement * expansion )
{
    int file, line;
    if ( true == translateLocation(location, file, line) )
    {
        m_numberOfMacros++;
        m_listing.resize(m_numberOfFiles + m_numberOfMacros);

        int origin = m_compilerCore->locationTrack().add(location);
        m_locationTransMap[origin] = {m_numberOfFiles + m_numberOfMacros - 1, expansion->location().m_lineStart};
        copyMacroBody(expansion, origin);
        m_listing[file][line].m_macro.push_back(m_numberOfFiles+m_numberOfMacros);
    }
}

void AsmCodeListing::repeatCode ( CompilerSourceLocation location,
                                  CompilerStatement * code,
                                  bool first )
{
    int file, line;
    if ( true == translateLocation(location, file, line) )
    {
        m_numberOfMacros++;
        int macroMark = ( m_numberOfFiles + m_numberOfMacros );
        if ( false == first )
        {
            macroMark = -macroMark;
        }

        int origin = m_compilerCore->locationTrack().add(location);
        m_listing.resize(m_numberOfFiles + m_numberOfMacros);
        m_locationTransMap[origin] = {m_numberOfFiles + m_numberOfMacros - 1, code->location().m_lineStart};
        copyMacroBody(code, origin);
        m_listing[file][line].m_macro.push_back(macroMark);
    }
}

void AsmCodeListing::copyMacroBody ( CompilerStatement * macro,
                                     int origin,
                                     int * lastLine )
{
    int ll = -1;
    if ( nullptr == lastLine )
    {
        lastLine = &ll;
    }

    const unsigned int fileNo = m_numberOfFiles + m_numberOfMacros - 1;

    for ( CompilerStatement * node = macro;
          nullptr != node;
          node = node->next() )
    {
        if ( true == node->location().isSet() )
        {
            if ( node->location().m_lineStart > (*lastLine) )
            {
                if ( -1 == (*lastLine) )
                {
                    (*lastLine) = node->location().m_lineStart;
                }
                else
                {
                    // Copy empty lines between the last LstLine and the one which is about to be inserted now.
                    while ( ++(*lastLine) < node->location().m_lineStart )
                    {
                        m_listing[fileNo].push_back(LstLine());
                    }
                }

                m_listing[fileNo].push_back(LstLine());
                LstLine & lstLine = m_listing[fileNo].back();

                lstLine = m_listing[node->location().m_fileNumber][(*lastLine) - 1];
                lstLine.m_message = -1;
            }

            node->m_location.m_origin = origin;
            m_symbolTable->rewriteExprLoc(node->args(), origin);
        }

        copyMacroBody(node->branch(), origin, lastLine);
    }
}

void AsmCodeListing::generatedCode ( CompilerSourceLocation location,
                                     CompilerStatement * code )
{
    int file, line;
    if ( false == translateLocation(location, file, line) )
    {
        return;
    }

    m_numberOfMacros++;
    m_listing.resize(m_numberOfFiles + m_numberOfMacros);
    m_listing[file][line].m_macro.push_back ( m_numberOfFiles + m_numberOfMacros );

    int lineNumber = 0;
    int origin = m_compilerCore->locationTrack().add(location);
    const unsigned int index = ( m_numberOfFiles + m_numberOfMacros - 1 );

    m_locationTransMap[origin] = {index, 0};

    for ( CompilerStatement * node = code;
          node != nullptr;
          node = node->next() )
    {
        if ( CompilerStatementTypes::EMPTY_STATEMENT == node->type() )
        {
            continue;
        }

        m_listing[index].push_back(LstLine());
        m_codeGenerator->toSourceLine(m_listing[index].back().m_line, node);
        node->m_location.m_fileNumber = index;
        node->m_location.m_lineStart  = lineNumber;
        node->m_location.m_lineEnd    = lineNumber;
        node->m_location.m_origin     = origin;
        lineNumber++;
    }
}

inline void AsmCodeListing::processMsgQueue()
{
    for ( const auto & msg : m_messageQueue )
    {
        message(msg.first, msg.second.m_type, msg.second.m_text, msg.second.m_subsequent);
    }

    m_messageQueue.clear();
}

void AsmCodeListing::message ( const CompilerSourceLocation & location,
                               CompilerBase::MessageType type,
                               const std::string & text,
                               bool subsequent )
{
    if ( 0 != m_messageLimit )
    {
        if ( m_msgCounter == m_messageLimit )
        {
            m_msgCounter = 0;
            message ( location,
                      CompilerBase::MT_WARNING,
                      QObject::tr ( "maximum number of messages reached, suppressing compiler message generation" )
                                  . toStdString() );
            m_msgCounter = m_messageLimit + 1;
            return;
        }
        else if ( m_msgCounter > m_messageLimit )
        {
            return;
        }
        m_msgCounter++;
    }

    if ( 0 == m_numberOfFiles )
    {
        m_messageQueue.push_back(std::pair<CompilerSourceLocation,Message>(location, Message(type, text, subsequent)));
        return;
    }

    if ( false == checkLocation(location, true) )
    {
        return;
    }

    if ( true == subsequent )
    {
        m_lastMsgPrefix += "==> ";
    }
    else
    {
        m_lastMsgPrefix.clear();
    }

    insertMessage(location, type, m_lastMsgPrefix + text);
}

inline void AsmCodeListing::insertMessage ( const CompilerSourceLocation & location,
                                            CompilerBase::MessageType type,
                                            const std::string & text )
{
    int file, line;
    if ( true == translateLocation(location, file, line) )
    {
        LstLine & lstLine = m_listing[file][line];

        int msgIdx = lstLine.m_message;
        if ( -1 == msgIdx )
        {
            msgIdx = m_messages.size();
            lstLine.m_message = msgIdx;
            m_messages.push_back(std::vector<Message>());
        }
        m_messages[msgIdx].push_back(Message(type, text));
    }
}

void AsmCodeListing::setMaxNumberOfMessages ( unsigned int limit )
{
    m_messageLimit = limit;
}

void AsmCodeListing::reset()
{
    m_msgCounter = 0;
}

std::ostream & operator << ( std::ostream & out,
                             const AsmCodeListing * codeListing )
{
    if ( false == codeListing->m_title.empty() )
    {
        out << codeListing->m_title << std::endl << std::endl;
    }

    bool outputEnabled = true;
    unsigned int lineNumber = 0;
    codeListing->printCodeListing(out, outputEnabled, lineNumber);

    return out;
}
