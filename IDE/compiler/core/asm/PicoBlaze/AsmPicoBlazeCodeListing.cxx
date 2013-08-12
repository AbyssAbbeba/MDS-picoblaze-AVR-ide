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
 * @file AsmPicoBlazeCodeListing.cxx
 */
// =============================================================================

#include "AsmPicoBlazeCodeListing.h"

// Standard headers.
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
}

AsmPicoBlazeCodeListing::Message::Message ( CompilerBase::MessageType type,
                                            const std::string & text )
{
    m_type = type;
    m_text = text;
}

AsmPicoBlazeCodeListing::AsmPicoBlazeCodeListing ( CompilerSemanticInterface * compilerCore,
                                                   CompilerOptions * opts )
                                                 : m_compilerCore ( compilerCore ),
                                                   m_opts ( opts )
{
    m_messageLimit = 0;
    m_compilerCore->registerMsgObserver(this);

    clear();
}

void AsmPicoBlazeCodeListing::clear()
{
    m_msgCounter = 0;
    m_numberOfFiles = 0;
    m_numberOfMacros = 0;
    m_listing.clear();
    m_messages.clear();
    m_messageQueue.clear();
    m_title.clear();
}

AsmPicoBlazeCodeListing::~AsmPicoBlazeCodeListing()
{
}

void AsmPicoBlazeCodeListing::loadSourceFiles()
{
    std::fstream file;
    int fileNumber = -1;
    char line [ MAX_LINE_LENGTH ];

    m_numberOfFiles = m_compilerCore->listSourceFiles().size();
    m_listing.resize(m_numberOfFiles);

    for ( std::vector<std::string>::const_iterator it = m_compilerCore->listSourceFiles().begin();
          it != m_compilerCore->listSourceFiles().end();
          it++ )
    {
        fileNumber++;
        file.open ( *it, std::fstream::in );

        if ( true == file.bad())
        {
            m_compilerCore -> compilerMessage ( CompilerBase::MT_ERROR,
                                                QObject::tr("unable to open ").toStdString() + "\"" + *it  + "\"" );
            return;
        }

        // Iterate over lines in the file
        while ( false == file.eof() )
        {
            file.getline ( line, MAX_LINE_LENGTH );
            m_listing[fileNumber].push_back ( LstLine ( line ) );
        }

        file.close();
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
    char line [ 16 ];
    std::string output;

    for ( std::vector<LstLine>::const_iterator i = m_listing[fileNumber].cbegin();
          i != m_listing[fileNumber].cend();
          i++ )
    {
        lineNumber++;

        if ( 1 == i->m_noList )
        {
            outputEnabled = true;
        }

        if ( true == outputEnabled )
        {
            output.clear();

            if ( -1 != i->m_value )
            {
                sprintf ( line, "  %05X   ", i->m_value );
                output += line;
            }
            else
            {
                if ( -1 != i->m_address )
                {
                    sprintf ( line, "%03X ", i->m_address );
                    output += line;
                }
                else
                {
                    output += "    ";
                }

                if ( 0 != i->m_code.size() )
                {
                    sprintf ( line, "%05X ", i->m_code[0] );
                    output += line;
                }
                else
                {
                    output += "      ";
                }
            }

            if ( 0 != inclusionLevel )
            {
                sprintf ( line, "=%d ", inclusionLevel );
                output += line;
                if ( inclusionLevel < 10 )
                {
                    output += " ";
                }
            }
            else
            {
                output += "    ";
            }

            sprintf ( line, "%6d ", lineNumber);
            output += line;

            if ( 0 != macroLevel )
            {
                sprintf ( line, "+%d ", macroLevel );
                output += line;
                if ( macroLevel < 10 )
                {
                    output += " ";
                }
            }
            else
            {
                output += "    ";
            }

            output += i->m_line;
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

            int addr = i->m_address + 1;
            for ( size_t j = 1; j < i->m_code.size(); j++ )
            {
                char tmp[11];
                sprintf ( tmp, "%04X %05X", addr, i->m_code[j] );
                out << tmp << std::endl;

                addr++;
            }

            if ( -1 != i->m_message )
            {
                for ( std::vector<Message>::const_iterator msg = m_messages[i->m_message].cbegin();
                    msg != m_messages[i->m_message].cend();
                    msg++ )
                {
                    if ( true == msg->m_text.empty() || CompilerBase::MT_INVALID == msg->m_type )
                    {
                        continue;
                    }

                    switch ( msg->m_type )
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

                    out << msg->m_text << std::endl;;
                }
            }
        }

        if ( -1 == i->m_noList )
        {
            outputEnabled = false;
        }

        for ( std::vector<int>::const_iterator mark = i->m_macro.cbegin();
              mark != i->m_macro.cend();
              mark ++ )
        {
            if ( 0 < *mark )
            {
                printCodeListing ( out, outputEnabled, lineNumber, ( *mark - 1 ), inclusionLevel, ( 1 + macroLevel ) );
            }
            else if ( 0 > *mark )
            {
                printCodeListing ( out, outputEnabled, lineNumber, -( *mark + 1 ), inclusionLevel, macroLevel );
            }
        }

        if ( -1 != i->m_inclusion )
        {
            printCodeListing ( out, outputEnabled, lineNumber, i->m_inclusion, ( 1 + inclusionLevel ), macroLevel );
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
        m_compilerCore -> compilerMessage ( CompilerBase::MT_ERROR,
                                            QObject::tr("unable to open ").toStdString() + "\""
                                            + m_opts -> m_lstFile  + "\"" );
        return;
    }

    file << this;

    if ( true == file.bad() )
    {
        m_compilerCore -> compilerMessage ( CompilerBase::MT_ERROR,
                                            QObject::tr("unable to write to ").toStdString() + "\""
                                            + m_opts -> m_lstFile  + "\"" );
        return;
    }
}

inline bool AsmPicoBlazeCodeListing::checkLocation ( const CompilerSourceLocation & location,
                                                  bool silent )
{
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
        m_compilerCore -> compilerMessage ( CompilerBase::MT_ERROR,
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

    m_numberOfMacros++;

    location.m_lineStart--;
    m_listing[location.m_fileNumber][location.m_lineStart].m_macro.push_back( m_numberOfFiles + m_numberOfMacros );

    unsigned int lineCounter = 0;
    m_listing.resize(m_numberOfFiles + m_numberOfMacros);
    copyMacroBody(&lineCounter, definition);
    lineCounter = 0;
    rewriteMacroLoc(&lineCounter, expansion);
}

void AsmPicoBlazeCodeListing::copyMacroBody ( unsigned int * lastLine,
                                              const CompilerStatement * macro )
{
    for ( const CompilerStatement * node = macro;
          node != NULL;
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
                                                CompilerStatement * macro )
{
    for ( CompilerStatement * node = macro;
          node != NULL;
          node = node->next() )
    {
        if ( true == node->location().isSet() )
        {
            if ( 0 == *lineDiff )
            {
                *lineDiff = node->m_location.m_lineStart - 1;
            }
            node->m_location.m_fileNumber = ( m_numberOfFiles + m_numberOfMacros - 1 );
            node->m_location.m_lineStart -= *lineDiff;
            node->m_location.m_lineEnd   -= *lineDiff;
        }

        rewriteMacroLoc ( lineDiff, node->branch() );
    }
}

void AsmPicoBlazeCodeListing::rewriteRepeatLoc ( unsigned int * lineDiff,
                                                 CompilerStatement * code )
{
    for ( CompilerStatement * node = code;
          node != NULL;
          node = node->next() )
    {
        if ( true == node->location().isSet() )
        {
            if ( 0 == *lineDiff )
            {
                *lineDiff = node->m_location.m_lineStart - 1;
            }
            node->m_location.m_fileNumber = ( m_numberOfFiles + m_numberOfMacros - 1 );
            node->m_location.m_lineStart -= *lineDiff;
            node->m_location.m_lineEnd   -= *lineDiff;
        }

        rewriteRepeatLoc ( lineDiff, node->branch() );
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
    location.m_lineStart--;
    m_listing[location.m_fileNumber][location.m_lineStart].m_macro.push_back(macroMark);

    unsigned int lineCounter = 0;
    m_listing.resize(m_numberOfFiles + m_numberOfMacros);
    copyMacroBody(&lineCounter, code);
    lineCounter = 0;
    rewriteRepeatLoc(&lineCounter, code);
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
          node != NULL;
          node = node->next() )
    {
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
    for ( std::vector<std::pair<CompilerSourceLocation,Message>>::const_iterator i = m_messageQueue.cbegin();
          i != m_messageQueue.cend();
          i++ )
    {
        message(i->first, i->second.m_type, i->second.m_text);
    }

    m_messageQueue.clear();
}

void AsmPicoBlazeCodeListing::message ( const CompilerSourceLocation & location,
                                        CompilerBase::MessageType type,
                                        const std::string & text )
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
        m_messageQueue.push_back ( std::pair<CompilerSourceLocation,Message>(location, Message(type, text)) );
        return;
    }

    if ( false == checkLocation(location, true) )
    {
        return;
    }

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
