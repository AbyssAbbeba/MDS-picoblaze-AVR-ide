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
 * @file AsmPicoBlazeCodeListing.cxx
 */
// =============================================================================

#include "AsmPicoBlazeCodeListing.h"

// PicoBlaze assembler semantic analyzer header files.
#include "AsmPicoBlazeSymbolTable.h"

// Standard headers.
#include <cstdio>
#include <cstdlib>
#include <fstream>

// Used for i18n only
#include <QObject>

AsmPicoBlazeCodeListing::LstLine::LstLine()
{
    m_address    = -1;
    m_inclusion  = -1;
    m_value      = -1;
    m_message    = -1;
    m_noList     = 0;
}

AsmPicoBlazeCodeListing::LstLine::LstLine ( const char * line )
{
    m_address    = -1;
    m_inclusion  = -1;
    m_value      = -1;
    m_message    = -1;
    m_noList     = 0;
    m_line       = line;
}

AsmPicoBlazeCodeListing::Message::Message()
{
    m_type = CompilerBase::MT_INVALID;
    m_subsequent = false;
}

AsmPicoBlazeCodeListing::Message::Message ( CompilerBase::MessageType type,
                                            const std::string & text,
                                            bool subsequent )
{
    m_type = type;
    m_text = text;
    m_subsequent = subsequent;
}

AsmPicoBlazeCodeListing::AsmPicoBlazeCodeListing ( CompilerSemanticInterface * compilerCore,
                                                   CompilerOptions * opts,
                                                   AsmPicoBlazeSymbolTable * symbolTable )
                                                 :
                                                   m_compilerCore ( compilerCore ),
                                                   m_opts ( opts ),
                                                   m_symbolTable ( symbolTable )
{
    m_messageLimit = 0;
    m_compilerCore->registerMsgObserver(this);

    clear();
}

void AsmPicoBlazeCodeListing::clear()
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

    m_lastMsgSubsequent = false;
    m_lastMsgLocation = CompilerSourceLocation();
}

AsmPicoBlazeCodeListing::~AsmPicoBlazeCodeListing()
{
}

void AsmPicoBlazeCodeListing::loadSourceFiles()
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

void AsmPicoBlazeCodeListing::printCodeListing ( std::ostream & out,
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

            if ( -1 != lstLine.m_value )
            {
                sprintf ( buffer, "  %05X   ", lstLine.m_value );
                output += buffer;
            }
            else
            {
                if ( -1 != lstLine.m_address )
                {
                    sprintf ( buffer, "%03X ", lstLine.m_address );
                    output += buffer;
                }
                else
                {
                    output += "    ";
                }

                if ( 0 != lstLine.m_code.size() )
                {
                    sprintf ( buffer, "%05X ", lstLine.m_code[0] );
                    output += buffer;
                }
                else
                {
                    output += "      ";
                }
            }

            if ( 0 != inclusionLevel )
            {
                sprintf ( buffer, "=%d ", inclusionLevel );
                output += buffer;
                if ( inclusionLevel < 10 )
                {
                    output += " ";
                }
            }
            else
            {
                output += "    ";
            }

            sprintf ( buffer, "%6d ", lineNumber);
            output += buffer;

            if ( 0 != macroLevel )
            {
                sprintf ( buffer, "+%d ", macroLevel );
                output += buffer;
                if ( macroLevel < 10 )
                {
                    output += " ";
                }
            }
            else
            {
                output += "    ";
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

                    out << msg.m_text << "." << std::endl;;
                }
            }
        }

        if ( -1 == lstLine.m_noList )
        {
            outputEnabled = false;
        }

        for ( const auto macro : lstLine.m_macro )
        {
            if ( 0 < macro )
            {
                printCodeListing ( out, outputEnabled, lineNumber, ( macro - 1 ), inclusionLevel, ( 1 + macroLevel ) );
            }
            else if ( 0 > macro )
            {
                printCodeListing ( out, outputEnabled, lineNumber, -( macro + 1 ), inclusionLevel, macroLevel );
            }
        }

        if ( -1 != lstLine.m_inclusion )
        {
            printCodeListing ( out, outputEnabled, lineNumber, lstLine.m_inclusion, ( 1 + inclusionLevel ), macroLevel );
        }
    }
}

void AsmPicoBlazeCodeListing::output()
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
                                            QObject::tr("unable to open ").toStdString() + "\""
                                            + m_opts -> m_lstFile  + "\"" );
        return;
    }

    file << this;

    if ( true == file.bad() )
    {
        m_compilerCore -> semanticMessage ( CompilerSourceLocation(),
                                            CompilerBase::MT_ERROR,
                                            QObject::tr("unable to write to ").toStdString() + "\""
                                            + m_opts -> m_lstFile  + "\"" );
        return;
    }
}

bool AsmPicoBlazeCodeListing::checkLocation ( const CompilerSourceLocation & location,
                                              bool silent )
{
    if ( m_files2skip.end() != m_files2skip.find(location.m_fileNumber) )
    {
        return false;
    }

    if ( -1 != location.m_fileNumber && (size_t)location.m_fileNumber < (m_numberOfFiles + m_numberOfMacros) )
    {
        if ( 0 < location.m_lineStart && (size_t)location.m_lineStart <= m_listing[location.m_fileNumber].size() )
        {
            // Location is valid.
            return true;
        }
    }

    // Location is NOT valid.
    if ( ( false == silent ) && ( -1 != location.m_fileNumber ) )
    {
        m_compilerCore -> semanticMessage ( CompilerSourceLocation(),
                                            CompilerBase::MT_ERROR,
                                            QObject::tr ( "some of the source code files were aparently changed"
                                                          " during compilation" ).toStdString() );
    }
    return false;
}

void AsmPicoBlazeCodeListing::setTitle ( const std::string & title )
{
    m_title = title;
}

void AsmPicoBlazeCodeListing::setNoList ( CompilerSourceLocation location,
                                          bool flag )
{
    if ( true == checkLocation ( location ) )
    {
        location.m_lineStart--;
        m_listing[location.m_fileNumber][location.m_lineStart].m_noList = ( ( true == flag ) ? -1 : 1 );
    }
}

void AsmPicoBlazeCodeListing::setInclusion ( CompilerSourceLocation location,
                                             int fileNumber )
{
    if ( ( true == checkLocation ( location ) ) && ( -1 != fileNumber ) )
    {
        location.m_lineStart--;
        m_listing[location.m_fileNumber][location.m_lineStart].m_inclusion = fileNumber;
    }
}

void AsmPicoBlazeCodeListing::setCode ( CompilerSourceLocation location,
                                        int code,
                                        int address )
{
    if ( true == checkLocation ( location ) )
    {
        location.m_lineStart--;
        std::vector<int> & codeVector = m_listing[location.m_fileNumber][location.m_lineStart].m_code;
        if ( 0 == codeVector.size() )
        {
            m_listing[location.m_fileNumber][location.m_lineStart].m_address = address;
        }
        codeVector.push_back(code);
    }
}

void AsmPicoBlazeCodeListing::setValue ( CompilerSourceLocation location,
                                         int value )
{
    if ( true == checkLocation ( location ) )
    {
        location.m_lineStart--;
        m_listing[location.m_fileNumber][location.m_lineStart].m_value = value;
    }
}

void AsmPicoBlazeCodeListing::expandMacro ( CompilerSourceLocation location,
                                            const CompilerStatement * definition,
                                            CompilerStatement * expansion )
{
    if ( false == checkLocation ( location ) )
    {
        return;
    }

    unsigned int lineCounter = 0;
    int formerOrigin = m_compilerCore->locationTrack().add(location);

    m_numberOfMacros++;

    location.m_lineStart--;
    m_listing[location.m_fileNumber][location.m_lineStart].m_macro.push_back( m_numberOfFiles + m_numberOfMacros );

    m_listing.resize(m_numberOfFiles + m_numberOfMacros);
    copyMacroBody(&lineCounter, definition);
    lineCounter = 0;
    rewriteMacroLoc(&lineCounter, expansion, formerOrigin);
}

void AsmPicoBlazeCodeListing::copyMacroBody ( unsigned int * lastLine,
                                              const CompilerStatement * macro )
{
    for ( const CompilerStatement * node = macro;
          node != nullptr;
          node = node->next() )
    {
        if ( true == node->location().isSet() )
        {
            // Copy empty lines between the last LstLine and the one which is about to be inserted now.
            if ( 0 != *lastLine )
            {
                for ( int i = ( node->location().m_lineStart - *lastLine ); i > 1 ; i-- )
                {
                    m_listing[m_numberOfFiles + m_numberOfMacros - 1].push_back(LstLine());
                }
            }
            *lastLine = node->location().m_lineStart;

            LstLine line = m_listing[node->location().m_fileNumber][node->location().m_lineStart - 1];
            line.m_message = -1;
            m_listing[m_numberOfFiles + m_numberOfMacros - 1].push_back(line);
        }

        copyMacroBody ( lastLine, node->branch() );
    }
}

void AsmPicoBlazeCodeListing::rewriteMacroLoc ( unsigned int * lineDiff,
                                                CompilerStatement * macro,
                                                int origin )
{
    for ( CompilerStatement * node = macro;
          node != nullptr;
          node = node->next() )
    {
        if ( true == node->location().isSet() )
        {
            if ( 0 == *lineDiff )
            {
                *lineDiff = node->m_location.m_lineStart - 1;
            }
            node->m_location.m_origin     = m_compilerCore->locationTrack().add(node->m_location, origin);
            node->m_location.m_fileNumber = ( m_numberOfFiles + m_numberOfMacros - 1 );
            node->m_location.m_lineStart -= *lineDiff;
            node->m_location.m_lineEnd   -= *lineDiff;

            m_symbolTable->rewriteExprLoc ( node->args(), node->location(), origin, true );
        }

        rewriteMacroLoc ( lineDiff, node->branch(), origin );
    }
}

void AsmPicoBlazeCodeListing::rewriteRepeatLoc ( unsigned int * lineDiff,
                                                 CompilerStatement * code,
                                                 int origin )
{
    for ( CompilerStatement * node = code;
          node != nullptr;
          node = node->next() )
    {
        if ( true == node->location().isSet() )
        {
            if ( 0 == *lineDiff )
            {
                *lineDiff = node->m_location.m_lineStart - 1;
            }
            node->m_location.m_origin     = m_compilerCore->locationTrack().add(node->m_location, origin);
            node->m_location.m_fileNumber = ( m_numberOfFiles + m_numberOfMacros - 1 );
            node->m_location.m_lineStart -= *lineDiff;
            node->m_location.m_lineEnd   -= *lineDiff;

            m_symbolTable->rewriteExprLoc ( node->args(), node->location(), origin, true );
        }

        rewriteRepeatLoc ( lineDiff, node->branch(), origin );
    }
}

void AsmPicoBlazeCodeListing::repeatCode ( CompilerSourceLocation location,
                                           CompilerStatement * code,
                                           bool first )
{
    if ( false == checkLocation ( location ) )
    {
        return;
    }

    m_numberOfMacros++;

    int macroMark = ( m_numberOfFiles + m_numberOfMacros );
    if ( false == first )
    {
        macroMark = -macroMark;
    }

    unsigned int lineCounter = 0;
    int formerOrigin = m_compilerCore->locationTrack().add(location);

    location.m_lineStart--;
    m_listing[location.m_fileNumber][location.m_lineStart].m_macro.push_back(macroMark);

    m_listing.resize(m_numberOfFiles + m_numberOfMacros);
    copyMacroBody(&lineCounter, code);
    lineCounter = 0;
    rewriteRepeatLoc(&lineCounter, code, formerOrigin);
}

void AsmPicoBlazeCodeListing::generatedCode ( CompilerSourceLocation location,
                                              CompilerStatement * code )
{
    if ( false == checkLocation ( location ) )
    {
        return;
    }

    m_numberOfMacros++;

    location.m_lineStart--;
    m_listing[location.m_fileNumber][location.m_lineStart].m_macro.push_back( m_numberOfFiles + m_numberOfMacros );
    m_listing.resize(m_numberOfFiles + m_numberOfMacros);

    int lineNumber = 0;
    const unsigned int index = ( m_numberOfFiles + m_numberOfMacros - 1 );
    for ( CompilerStatement * node = code;
          node != nullptr;
          node = node->next() )
    {
        if ( CompilerStatementTypes::EMPTY_STATEMENT == node->type() )
        {
            continue;
        }

        m_listing[index].push_back(LstLine());
        m_codeGenerator.toSourceLine(m_listing[index].back().m_line, node);

        lineNumber++;
        node->m_location.m_fileNumber = index;
        node->m_location.m_lineStart = lineNumber;
        node->m_location.m_lineEnd   = lineNumber;
    }
}

inline void AsmPicoBlazeCodeListing::processMsgQueue()
{
    for ( const auto & msg : m_messageQueue )
    {
        message(msg.first, msg.second.m_type, msg.second.m_text, msg.second.m_subsequent);
    }

    m_messageQueue.clear();
}

void AsmPicoBlazeCodeListing::message ( const CompilerSourceLocation & location,
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
        if ( ( false == m_lastMsgSubsequent ) || ( location.m_fileNumber >= (int) m_numberOfFiles ) )
        {
            if ( location.m_fileNumber > m_lastMsgLocation.m_fileNumber )
            {
                m_lastMsgLocation = location;
                m_lastMsgSubsequent = true;
            }
            return;
        }
        else
        {
            m_lastMsgPrefix += "==> ";
        }
    }
    else if ( true == m_lastMsgSubsequent )
    {
        insertMessage(m_lastMsgLocation, type, text);

        m_lastMsgPrefix += "==> ";
        m_lastMsgSubsequent = false;
        m_lastMsgLocation = CompilerSourceLocation();
    }

    insertMessage(location, type, m_lastMsgPrefix + text);

    if ( false == subsequent )
    {
        m_lastMsgPrefix.clear();
    }
}

inline void AsmPicoBlazeCodeListing::insertMessage ( const CompilerSourceLocation & location,
                                                     CompilerBase::MessageType type,
                                                     const std::string & text )
{
    LstLine & lstLine = m_listing[location.m_fileNumber][location.m_lineStart - 1];

    int msgIdx = lstLine.m_message;
    if ( -1 == msgIdx )
    {
        msgIdx = m_messages.size();
        lstLine.m_message = msgIdx;
        m_messages.push_back(std::vector<Message>());
    }
    m_messages[msgIdx].push_back(Message(type, text));
}

void AsmPicoBlazeCodeListing::setMaxNumberOfMessages ( unsigned int limit )
{
    m_messageLimit = limit;
}

void AsmPicoBlazeCodeListing::reset()
{
    m_msgCounter = 0;
}

std::ostream & operator << ( std::ostream & out,
                             const AsmPicoBlazeCodeListing * codeListing )
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
