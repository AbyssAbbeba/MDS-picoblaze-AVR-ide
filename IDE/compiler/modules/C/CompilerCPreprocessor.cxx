// =============================================================================
/**
 * @brief
 * C++ Implementation: ...
 *
 * ...
 *
 * (C) copyright 2013, 2014 Moravia Microsystems, s.r.o.
 *
 * @ingroup Compiler
 * @file CompilerCPreprocessor.cxx
 */
// =============================================================================

#include "CompilerCPreprocessor.h"

// Common compiler header files.
#include "CompilerOptions.h"
#include "CompilerParserInterface.h"

// Standard headers.
#include <cstdlib>
#include <cstring>
#include<iostream>//DEBUG

// getline() function.
#include "../../../utilities/os/getline.h"

CompilerCPreprocessor::CompilerCPreprocessor ( CompilerParserInterface * compilerCore,
                                               CompilerOptions * opts )
                                             :
                                               m_compilerCore ( compilerCore ),
                                               m_opts ( opts )
{
}

char * CompilerCPreprocessor::processFiles ( const std::vector<FILE *> & inputFiles )
{
    // Input buffer for reading the sourceFile.
    ssize_t lineLen;                // Number of characters loaded in the buffer; including EOLs, without NULL(s).
    size_t  inBufferSize = 0;       // Overall size of the input buffer.
    char *  inBuffer     = nullptr; // Buffer pointer, used for single line only.

    // Output buffer for stroring preprocessor output.
    size_t  outBufferCurP = 0;                             // Current position in the buffer.
    size_t  outBufferSize = 1024;                          // Overall size of the buffer.
    ssize_t outBufferCap  = ( outBufferSize - 1 );         // Number of free bytes in the buffer.
    char *  outBuffer     = (char*) malloc(outBufferSize); // Buffer pointer, used to store the entire output at once.

    // Auxiliary buffer for merging lines.
    size_t mergeBufferCurP = 0;                            // Current position in the buffer.
    size_t mergeBufferSize = 0;                            // Overall size of the buffer.
    char * mergeBuffer     = nullptr;                      // Buffer pointer, cannot contain more that one line.

    // Start in the Normal mode.
    m_inmode = MODE_NORMAL;

    // Start with valid empty output buffer.
    outBuffer[outBufferCurP] = '\0';

    // Iterate over all given input files.
    for ( auto sourceFile : inputFiles )
    {
        // Iterate over lines in the source file.
        while ( -1 != ( lineLen = getline(&inBuffer, &inBufferSize, sourceFile) ) )
        {
            // Handle line merging - trailing `\' (backslash)
            if ( true == lineMerge(inBuffer, mergeBuffer, lineLen, inBufferSize, mergeBufferSize, mergeBufferCurP) )
            {
                continue;
            }

            // Process input and generate output.
            if ( false == processLine ( lineLen, inBuffer, inBufferSize ) )
            {
                // Error -> terminate preprocessor.
                free(inBuffer);
                free(outBuffer);
                free(mergeBuffer);
                return nullptr;
            }

            // Safely copy contents of the input buffer to the output buffer.
            outBufferCap -= lineLen;
            if ( outBufferCap < 0 )
            {
                size_t incr = 1024 + ( 10 * -outBufferCap );
                outBufferSize += incr;
                outBufferCap  += incr;

                outBuffer = (char*) realloc ( outBuffer, outBufferSize );
            }
            strcpy((outBuffer + outBufferCurP), inBuffer);
            outBufferCurP += lineLen;
        }

        // Check for error condition on the sourceFile.
        if ( 0 != ferror(sourceFile) )
        {
            m_compilerCore->preprocessorMessage ( CompilerSourceLocation(),
                                                  CompilerBase::MT_ERROR,
                                                  QObject::tr ( "I/O error, cannot read the source file properly" )
                                                              . toStdString() );

            // Error -> terminate preprocessor.;
            free(inBuffer);
            free(outBuffer);
            free(mergeBuffer);
            return nullptr;
        }
    }

    // Save the output from preprocessor into a file specified by CompilerOptions.
    if ( false == m_opts->m_cunit.empty() )
    {
        FILE * file = fopen(m_opts->m_cunit.c_str(), "w+");
        if ( nullptr != file )
        {
            fwrite(outBuffer, sizeof(char), outBufferCurP, file);
            if ( 0 != ferror(file) )
            {
                m_compilerCore->preprocessorMessage ( CompilerSourceLocation(),
                                                      CompilerBase::MT_ERROR,
                                                      QObject::tr ( "unable to save file: `%1'" )
                                                                  . arg ( m_opts->m_cunit.c_str() )
                                                                  . toStdString() );
            }
            fclose(file);
        }
        else
        {
            m_compilerCore->preprocessorMessage ( CompilerSourceLocation(),
                                                  CompilerBase::MT_ERROR,
                                                  QObject::tr ( "unable to open file: `%1'" )
                                                              . arg ( m_opts->m_cunit.c_str() )
                                                              . toStdString() );
        }
    }

    // Done.
    free(inBuffer);
    free(mergeBuffer);
    return outBuffer;
}

inline bool CompilerCPreprocessor::lineMerge ( char * & inBuffer,
                                               char * & mergeBuffer,
                                               ssize_t & lineLen,
                                               size_t  & inBufferSize,
                                               size_t  & mergeBufferSize,
                                               size_t  & mergeBufferCurP )
{
    bool abort = false;
    int lineMerge = -1;
    int whiteSpace = -1;

    for ( ssize_t pos = ( lineLen - 1 ); pos >= 0; pos-- )
    {
        switch ( inBuffer[pos] )
        {
            case '\t': case ' ':
                whiteSpace = (int) pos;
            case '\r': case '\n':
                break;
            case '\\':
                lineMerge = (int) pos;
                break;
            case '/': // Handle trigraph "? ? /"
                if ( true == m_opts->m_enableTrigraphs )
                {
                    if ( ( pos >= 2 ) && ( '?' == inBuffer[pos-1] ) && ( '?' == inBuffer[pos-2] ) )
                    {
                        lineMerge = (int) (pos - 2);
                        break;
                    }
                }
            default:
                abort = true;
                break;
        }

        if ( true == abort )
        {
            break;
        }
    }

    if ( -1 != whiteSpace )
    {
        // TODO: Warning.
    }

    if ( -1 != lineMerge )
    {
        const size_t requiredSize = ( lineMerge + mergeBufferCurP + 1 );
        if ( requiredSize > mergeBufferSize )
        {
            mergeBufferSize = requiredSize;
            mergeBuffer = (char*) realloc ( mergeBuffer, mergeBufferSize );
        }

        strncpy((mergeBuffer + mergeBufferCurP), inBuffer, lineMerge);
        mergeBufferCurP += lineMerge;
        return true;
    }

    if ( 0 != mergeBufferCurP )
    {
        const size_t requiredSize = ( mergeBufferCurP + lineLen + 1 );
        if ( requiredSize > inBufferSize )
        {
            inBufferSize = requiredSize;
            inBuffer = (char*) realloc ( inBuffer, inBufferSize );
        }
        if ( requiredSize > mergeBufferSize )
        {
            mergeBufferSize = requiredSize;
            mergeBuffer = (char*) realloc ( mergeBuffer, mergeBufferSize );
        }
        strncpy((mergeBuffer + mergeBufferCurP), inBuffer, lineLen);

        lineLen += mergeBufferCurP;
        strncpy(inBuffer, mergeBuffer, lineLen);
        inBuffer[lineLen] = '\0';
        mergeBufferCurP = 0;
    }

    return false;
}

void CompilerCPreprocessor::cutLine ( ssize_t & length,
                                      char * line,
                                      unsigned int pos )
{
    // Determinate EOL.
    bool lf = ( '\n' == line[length-1] );
    bool cr = !lf;

    if ( ( length > 1 ) && ( '\r' == line[length-2] ) )
    {
        cr = true;
    }

    if ( ( true == cr ) && ( true == lf ) )
    {
        length = pos + 2;
        line[pos] = '\r';
        line[++pos] = '\n';
    }
    else
    {
        length = pos + 1;
        line[pos] = ( true == lf ) ? '\n' : '\r';
    }
    line[++pos] = '\0';
}

inline bool CompilerCPreprocessor::processLine ( ssize_t & length,
                                                 char * & line,
                                                 size_t & /*bufferSize*/ )
{
    unsigned int out = 0;
    bool copy = true;

    for ( unsigned int in = 0; in < length; in++ )
    {
        if ( ( '\n' == line[in] ) || ( '\r' == line[in] ) )
        {
            if ( false == copy )
            {
                length = out;
                line[out] = '\0';
            }
            else
            {
                cutLine(length, line, out);
            }
            return true;
        }

        switch ( m_inmode )
        {
            case MODE_NORMAL:
                switch ( line[in] )
                {
                    case '"':
                        m_inmode = MODE_STRING;
                        break;
                    case '\'':
                        m_inmode = MODE_CHAR;
                        break;
                    case '/':
                        switch ( line[1+in] )
                        {
                            case '/':
                                line[out++] = ' ';
                                cutLine(length, line, out);
                                return true;
                            case '*':
                                m_inmode = MODE_COMMENT;
                                copy = false;
                                break;
                        }
                        break;
                    case '?':
                        if ( ( true == m_opts->m_enableTrigraphs ) && ( '?' == line[1+in] ) )
                        {
                            char replacent = '\0';
                            switch ( line[2+in] )
                            {
                                case '(':  replacent = '[';  break;
                                case ')':  replacent = ']';  break;
                                case '<':  replacent = '{';  break;
                                case '>':  replacent = '}';  break;
                                case '=':  replacent = '#';  break;
                                case '/':  replacent = '\\'; break;
                                case '\'': replacent = '^';  break;
                                case '!':  replacent = '|';  break;
                                case '-':  replacent = '~';  break;
                            }
                            if ( '\0' != replacent )
                            {
                                in++;
                                line[1+in] = replacent;
                                continue;
                            }
                        }
                        break;
                    case '<':
                    case '%':
                    case ':':
                        if ( true == m_opts->m_enableDigraphs )
                        {
                            char replacent = '\0';
                            switch ( line[in] )
                            {
                                case '<':
                                    switch ( line[1+in] )
                                    {
                                        case '%': replacent = '{';  break;
                                        case ':': replacent = '[';  break;
                                    }
                                    break;
                                case '%':
                                    switch ( line[1+in] )
                                    {
                                        case '>': replacent = '}';  break;
                                        case ':': replacent = '#';  break;
                                    }
                                    break;
                                case ':':
                                    if ( '>' == line[1+in] )
                                    {
                                        replacent = ']';
                                    }
                                    break;
                            }
                            if ( '\0' != replacent )
                            {
                                line[1+in] = replacent;
                                continue;
                            }
                        }
                }
                break;
            case MODE_STRING:
                switch ( line[in] )
                {
                    case '"':
                        m_inmode = MODE_NORMAL;
                        break;
                }
                break;
            case MODE_CHAR:
                switch ( line[in] )
                {
                    case '\'':
                        m_inmode = MODE_NORMAL;
                        break;
                }
                break;
            case MODE_COMMENT:
                if ( ( '*' == line[in] ) && ( '/' == line[1+in] ) )
                {
                    line[++in] = ' ';
                    copy = true;
                    m_inmode = MODE_NORMAL;
                }
                else
                {
                    copy = false;
                }
                break;
        }

        if ( true == copy )
        {
            line[out++] = line[in];
        }
    }

    line[out] = '\0';
    length = out;
    return true;
}

