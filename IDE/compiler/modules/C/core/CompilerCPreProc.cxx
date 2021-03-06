// =============================================================================
/**
 * @brief
 * C++ Implementation: ...
 *
 * ...
 *
 * (C) copyright 2014, 2015 Moravia Microsystems, s.r.o.
 *
 * @ingroup CompilerC
 * @file CompilerCPreProc.cxx
 */
// =============================================================================

#include "CompilerCPreProc.h"

/*
 * MISSING FEATURES:
 * - location map involvement for digraphs, trigraphs, and macros
 * - _Pragma operator
 * - macro expansion exactly as specified in the standard
 */

// Common compiler header files.
#include "CompilerOptions.h"
#include "CompilerParserInterface.h"

// Standard headers.
#include <cctype>
#include <cstdlib>
#include <climits>
#include <cstring>
#include<iostream>//DEBUG

// getline() function.
#include "utilities/os/getline.h"

#if ! defined ( YYSTYPE ) && ! defined ( YYSTYPE_IS_DECLARED )
    typedef union YYSTYPE {} YYSTYPE;
#endif
#include "CompilerCPreProcCalcLex.h"
int CompilerCPreProcCalcPar_parse ( yyscan_t yyscanner,
                                    CompilerCPreProcInterface * preprocessor );

const std::map<std::string, CompilerCPreProc::Directive> CompilerCPreProc::s_directives =
{
    { "include", DIR_INCLUDE },    { "line",    DIR_LINE    },
    { "pragma",  DIR_PRAGMA  },    { "define",  DIR_DEFINE  },
    { "undef",   DIR_UNDEF   },    { "if",      DIR_IF      },
    { "elif",    DIR_ELIF    },    { "ifdef",   DIR_IFDEF   },
    { "ifndef",  DIR_IFNDEF  },    { "else",    DIR_ELSE    },
    { "endif",   DIR_ENDIF   },    { "warning", DIR_WARNING },
    { "error",   DIR_ERROR   },    { "",        DIR_NULL    }
};

CompilerCPreProc::CompilerCPreProc ( CompilerParserInterface * compilerCore,
                                     const CompilerOptions * opts,
                                     CompilerCBackend * backend )
                                   :
                                     CompilerCPreProcInterface ( compilerCore, opts, backend ),
                                     m_macroTable ( compilerCore, opts, backend, m_locationStack )
{
}

char * CompilerCPreProc::processFiles ( const std::vector<FILE*> & inputFiles )
{
    Buffer inBuffer;    // Input buffer for reading the source file.
    Buffer outBuffer;   // Output buffer for storing the preprocessor output.
    Buffer mergeBuffer; // Auxiliary buffer for merging lines using `\' (backslash) or the "backslash" trigraph.
    Buffer mlineBuffer; // Auxiliary buffer for merging lines containing C multi-line comment: /* ... <new-line> ... */.

    // Start in the Normal mode.
    m_inmode = MODE_NORMAL;

    m_lineMerges = 0;
    m_location = CompilerSourceLocation(0, 1, 0, 1, 0);

    // Iterate over all given input files.
    std::vector<FILE*> fileStack;
    for ( unsigned int fileNumber = 0; fileNumber < inputFiles.size(); fileNumber++ )
    {
        m_locationStack.push_back(CompilerSourceLocation(fileNumber, 1, 0, 1, 0));

        for ( fileStack.push_back(inputFiles[fileNumber]); false == fileStack.empty(); fileStack.pop_back() )
        {
            // Iterate over the lines in the source file.
            while ( -1 != ( inBuffer.m_pos = getline(&inBuffer.m_data, &inBuffer.m_size, fileStack.back()) ) )
            {
                m_compilerCore->locationMap().addMark(m_locationStack.back(), m_location);

                bool result = false;

                try
                {
                    result = inputProcessing(inBuffer, outBuffer, mergeBuffer, mlineBuffer);
                }
                catch ( const CompilerCPreProcMacros::MacroException & e )
                {
                    m_compilerCore->preprocessorMessage ( locationCorrection(m_locationStack.back(), m_lineMerges),
                                                          CompilerBase::MT_ERROR,
                                                          e.m_message );
                }

                if ( false == result )
                {
                    // Critical error.
                    return nullptr;
                }

                if ( false == m_include.m_file.empty() )
                {
                    fileStack.push_back(m_compilerCore->fileOpen(m_include.m_file, nullptr, false, m_include.m_system));
                    m_locationStack.push_back(CompilerSourceLocation(m_compilerCore->getFileNumber(), 1, 0, 1, 0));
                    m_include.m_file.clear();

                    if ( nullptr == fileStack.back() )
                    {
                        // Critical error.
                        return nullptr;
                    }

                    continue;
                }
            }

            m_compilerCore->locationMap().addMark(m_locationStack.back(), m_location);
            m_locationStack.pop_back();

            // Check for error condition on the source file.
            if ( 0 != ferror(fileStack.back()) )
            {
                m_compilerCore->preprocessorMessage ( CompilerSourceLocation(),
                                                      CompilerBase::MT_ERROR,
                                                      QObject::tr ( "I/O error, cannot read the source file properly" )
                                                                  . toStdString() );

                // Critical error.
                return nullptr;
            }
        }
    }

    if ( false == m_conditional.empty() )
    {
        CompilerSourceLocation location;
        m_conditional.get(&location);
        m_compilerCore->preprocessorMessage ( locationCorrection(location),
                                              CompilerBase::MT_ERROR,
                                              QObject::tr("unterminated preprocessor condition").toStdString() );
        return nullptr;
    }

    // Save the output from preprocessor into a file specified by CompilerOptions.
    if ( false == m_opts->m_cunit.empty() )
    {
        FILE * file = fopen(m_opts->m_cunit.c_str(), "w+");
        if ( nullptr != file )
        {
            fwrite(outBuffer.m_data, sizeof(char), outBuffer.m_pos, file);
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

    //
    m_compilerCore->locationMap().sortMap();

    // Done.
    outBuffer.m_persistent = true;
    if ( nullptr == outBuffer.m_data )
    {
        outBuffer.append(" ");
    }
    return outBuffer.m_data;
}

inline bool CompilerCPreProc::inputProcessing ( Buffer & inBuffer,
                                                Buffer & outBuffer,
                                                Buffer & mergeBuffer,
                                                Buffer & mlineBuffer )
{
    m_locationStack.back().m_lineStart++;
    m_locationStack.back().m_colStart = 1;

    // Handle line merging - trailing `\' (backslash)
    if ( true == lineMerging(inBuffer, mergeBuffer) )
    {
        m_location.m_colStart += inBuffer.m_pos;
        m_lineMerges++;
        return true;
    }

    // When true, indicates requests to merge lines containing C multi-line comment: /*...<new-line>...*/.
    bool lineMerge = false;

    // Perform the initial stage of preprocessing: comments, trigrahs, and digraphs.
    if ( false == initialProcessing(inBuffer, lineMerge) )
    {
        // Critical error.
        return false;
    }

    if ( true == lineMerge )
    {
        // Append contents of the input buffer to the mline buffer.
        mlineBuffer.append(inBuffer);
        m_location.m_colStart = mlineBuffer.m_pos + 2;
        m_lineMerges++;
        return true;
    }
    else if ( 0 != mlineBuffer.m_pos )
    {
        // Append contents of the input buffer to the mline buffer.
        mlineBuffer.append(inBuffer);

        // Copy the entire mline buffer to the input buffer, and clear the mline buffer.
        inBuffer.move(mlineBuffer);
    }

    if ( false == directivesProcessing(inBuffer) )
    {
        // Critical error.
        return false;
    }

    // Continue only is output is not temporarily disabled by #if, #else, etc.
    if ( true == m_conditional.get() )
    {
        // Append contents of the input buffer to the output buffer and expand macros in one step.
        m_macroTable.expand(outBuffer, inBuffer);
    }
    else
    {
        m_compilerCore->locationMap().removeMarks(m_location);
        m_location.m_lineStart--;
    }

    m_location.m_lineStart++;
    m_location.m_colStart = 1;
    m_lineMerges = 0;

    return true;
}

inline bool CompilerCPreProc::lineMerging ( Buffer & in,
                                            Buffer & mergeBuffer )
{
    int lineMerge = -1;  // Position of the line merge mark - "\" (backslash) or the "? ? /" trigraph.
    int whiteSpace = -1; // Position of the first white space character following the line merge mark (warning cond.).
    bool endOfLine = false;

    // Search the line backwards for certain characters.
    for ( ssize_t pos = ( in.m_pos - 1 ); pos >= 0; pos-- )
    {
        switch ( in.m_data[pos] )
        {
            // Skip white space.
            case '\t': case ' ':
                whiteSpace = (int) pos;
                continue;

            // Skip EOL characters.
            case '\r': case '\n':
                endOfLine = true;
                continue;

            // Backslash - line merge mark.
            case '\\':
                lineMerge = (int) pos;
                break;
            // Trigraph "? ? /" - line merge mark.
            case '/':
                if ( true == m_opts->m_enableTrigraphs )
                {
                    // Check if the two preceding characters are question marks.
                    if ( ( pos >= 2 ) && ( '?' == in.m_data[pos-1] ) && ( '?' == in.m_data[pos-2] ) )
                    {
                        lineMerge = (int) (pos - 2);
                    }
                }
                break;
        }

        break;
    }

    // Line merge found, copy the input buffer into the merge buffer.
    if ( -1 != lineMerge )
    {
        if ( -1 != whiteSpace )
        {
            m_compilerCore->preprocessorMessage ( locationCorrection(m_locationStack.back()),
                                                  CompilerBase::MT_WARNING,
                                                  QObject::tr("space between backslash and new line").toStdString() );
        }

        // Append contents of the input buffer to the merge buffer.
        in.m_pos = lineMerge;
        mergeBuffer.append(in);

        // The input buffer contents was appended to the merge buffer, and therefore should be ignored for further
        //+ processing until the merge is entirely complete.
        return true;
    }
    // Line merge not found but the merge buffer is not empty, merge the input buffer with the merge buffer.
    else if ( 0 != mergeBuffer.m_pos )
    {
        // Append contents of the input buffer to the merge buffer.
        mergeBuffer.append(in);

        // Copy the entire merge buffer to the input buffer, and clear the merge buffer.
        in.move(mergeBuffer);
    }

    if ( false == endOfLine )
    {
        char newLine = '\n';
        in.append(Buffer(&newLine, 1));
    }

    return false;
}

inline void CompilerCPreProc::cutLine ( ssize_t & length,
                                        char * line,
                                        unsigned int pos )
{
    // Determinate EOL.
    bool lf = ( '\n' == line[length-1] ); // LF (\n) is present.
    bool cr = !lf;                        // CR (\r) is present.

    // Determinate whether CRLF sequence is present.
    if ( ( length > 1 ) && ( '\r' == line[length-2] ) )
    {
        cr = true;
    }

    // Cut the line with respect to the original end of line (EOL) character sequence.
    if ( ( true == cr ) && ( true == lf ) )
    {
        // CRLF sequence.
        length = pos + 2;
        line[pos] = '\r';
        line[++pos] = '\n';
    }
    else
    {
        // Single CR or LF character.
        length = pos + 1;
        line[pos] = ( true == lf ) ? '\n' : '\r';
    }

    // Insert proper C string terminator.
    line[++pos] = '\0';
}

inline bool CompilerCPreProc::initialProcessing ( Buffer & buffer,
                                                  bool & lineMerge )
{
    unsigned int out = 0;
    bool copy = true;

    for ( unsigned int in = 0; in < buffer.m_pos; in++ )
    {
        const char inchar   = buffer.m_data[in];
        const char nextchar = buffer.m_data[1+in];
        bool backslash = ( ( 0 != in ) && ( '\\' == buffer.m_data[in - 1] ) );

        switch ( inchar )
        {
            case '\n':
            case '\r':
                if ( false == copy )
                {
                    lineMerge = true;
                    buffer.m_pos = out;
                    buffer.m_data[out] = '\0';
                }
                else
                {
                    m_include.m_detection.reset();
                    cutLine(buffer.m_pos, buffer.m_data, out);
                }
                return true;
            case '?':
                if ( ( true == m_opts->m_enableTrigraphs ) && ( '?' == nextchar ) )
                {
                    char replacent = '\0';
                    switch ( buffer.m_data[2+in] )
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
                        CompilerSourceLocation loc = locationCorrection(m_locationStack.back());
                        loc.m_colStart += in + 1;
                        loc.m_colEnd = loc.m_colStart + 3;
                        m_compilerCore->preprocessorMessage ( loc,
                                                              CompilerBase::MT_WARNING,
                                                              QObject::tr("replacing trigraph with: ").toStdString()
                                                              + replacent );

                        in++;
                        buffer.m_data[1+in] = replacent;
                        continue;
                    }
                }
                break;
        }

        m_include.m_detection.input(inchar);

        switch ( m_inmode )
        {
            case MODE_NORMAL:
                switch ( inchar )
                {
                    case '"':
                        if ( false == backslash )
                        {
                            m_inmode = MODE_STRING;
                        }
                        break;
                    case '\'':
                        if ( false == backslash )
                        {
                            m_inmode = MODE_CHAR;
                        }
                        break;
                    case '/':
                        switch ( nextchar )
                        {
                            case '/':
                                buffer.m_data[out++] = ' ';
                                cutLine(buffer.m_pos, buffer.m_data, out);
                                return true;
                            case '*':
                                m_inmode = MODE_COMMENT;
                                copy = false;
                                in++;
                                break;
                        }
                        break;
                    case '<':
                        if (
                               ( true == m_include.m_detection.detected() )
                                   &&
                               (
                                   ( false == m_opts->m_enableDigraphs )
                                       ||
                                   ( ( '%' != nextchar ) && ( ':' != nextchar ) )
                               )
                           )
                        {
                            m_inmode = MODE_ANG_BR;
                            break;
                        }
                    case '%':
                    case ':':
                        if ( true == m_opts->m_enableDigraphs )
                        {
                            char replacent = '\0';
                            switch ( inchar )
                            {
                                case '<':
                                    switch ( nextchar )
                                    {
                                        case '%': replacent = '{';  break;
                                        case ':': replacent = '[';  break;
                                    }
                                    break;
                                case '%':
                                    switch ( nextchar )
                                    {
                                        case '>': replacent = '}';  break;
                                        case ':': replacent = '#';  break;
                                    }
                                    break;
                                case ':':
                                    if ( '>' == nextchar )
                                    {
                                        replacent = ']';
                                    }
                                    break;
                            }
                            if ( '\0' != replacent )
                            {
                                buffer.m_data[1+in] = replacent;
                                continue;
                            }
                        }
                        break;
                }
                break;
            case MODE_STRING:
                if ( ( '"' == inchar ) && ( false == backslash ) )
                {
                    m_inmode = MODE_NORMAL;
                }
                break;
            case MODE_CHAR:
                if ( ( '\'' == inchar ) && ( false == backslash ) )
                {
                    m_inmode = MODE_NORMAL;
                }
                break;
            case MODE_ANG_BR:
                if ( '>' == inchar )
                {
                    m_inmode = MODE_NORMAL;
                }
                break;
            case MODE_COMMENT:
                if ( ( '*' == inchar ) && ( '/' == nextchar ) )
                {
                    buffer.m_data[++in] = ' ';
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
            buffer.m_data[out++] = buffer.m_data[in];
        }
    }

    buffer.m_data[out] = '\0';
    buffer.m_pos = out;
    return true;
}

inline bool CompilerCPreProc::directivesProcessing ( Buffer & buffer )
{
    for ( int in = 0; in < buffer.m_pos; in++ )
    {
        if ( '#' == buffer.m_data[in] )
        {
            while ( ( ++in < buffer.m_pos ) && isblank(buffer.m_data[in]) );
            int start = in;

            while ( isalpha(buffer.m_data[in]) && ( ++in < buffer.m_pos ) );
            char directive [ in - start + 1 ];
            memcpy(directive, buffer.m_data + start, in - start);
            directive[in - start] = '\0';

            while ( isblank(buffer.m_data[in]) && ( ++in < buffer.m_pos ) );
            char * arguments = buffer.m_data + in;
            in = ( buffer.m_pos - in );

            while ( ( --in >= 0 ) && isspace(arguments[in]) );
            arguments[++in] = '\0';

            buffer.m_pos = 0;
            buffer.m_data[0] = '\0';

            m_compilerCore->locationMap().removeMarks(m_location);
            m_location.m_lineStart--;

            const auto iter = s_directives.find(directive);
            if ( s_directives.cend() == iter )
            {
                m_compilerCore->preprocessorMessage ( locationCorrection(m_locationStack.back(), m_lineMerges),
                                                      CompilerBase::MT_ERROR,
                                                      QObject::tr("invalid preprocessor directive: ")
                                                                 .toStdString() + directive );
                return false;
            }

            return handleDirective(arguments, iter->second);
        }
        else if ( ( ' '  != buffer.m_data[in] ) && ( '\t' != buffer.m_data[in] ) )
        {
            return true;
        }
    }

    return true;
}

inline bool CompilerCPreProc::handleDirective ( char * arguments,
                                                Directive directive )
{
    if ( ( false == m_conditional.get() ) &&
         ( DIR_ELIF != directive ) && ( DIR_ELSE != directive ) && ( DIR_ENDIF != directive ) )
    {
        return true;
    }

    unsigned int argLength = strlen(arguments);

    switch ( directive )
    {
        case DIR_INCLUDE:
            handleInclude(arguments, argLength);
            break;
        case DIR_LINE:
        {
            Buffer buf;
            m_macroTable.expand(buf, Buffer(arguments, argLength));
            handleLine(buf.m_data, buf.m_pos);
            break;
        }
        case DIR_PRAGMA:
            handlePragma(arguments, argLength);
            break;
        case DIR_DEFINE:
            m_macroTable.define(arguments, argLength);
            break;
        case DIR_UNDEF:
            m_macroTable.undef(arguments);
            break;
        case DIR_IF:
            m_conditional.dirIf(m_locationStack.back(), evaluateExpr(arguments, argLength));
            break;
        case DIR_IFDEF:
            if ( true == expectSigleId(arguments, argLength) )
            {
                m_conditional.dirIf(m_locationStack.back(), m_macroTable.isDefined(arguments));
            }
            else
            {
                m_conditional.dirIf(m_locationStack.back(), false);
            }
            break;
        case DIR_IFNDEF:
            if ( true == expectSigleId(arguments, argLength) )
            {
                m_conditional.dirIf(m_locationStack.back(), !m_macroTable.isDefined(arguments));
            }
            else
            {
                m_conditional.dirIf(m_locationStack.back(), false);
            }
            break;
        case DIR_ELIF:
            if ( false == m_conditional.dirElif(m_locationStack.back(), evaluateExpr(arguments, argLength)) )
            {
                m_compilerCore->preprocessorMessage ( locationCorrection(m_locationStack.back(), m_lineMerges),
                                                      CompilerBase::MT_ERROR,
                                                      QObject::tr("#elif without an #if").toStdString() );
            }
            break;
        case DIR_ELSE:
            if ( 0 != argLength )
            {
                m_compilerCore->preprocessorMessage ( locationCorrection(m_locationStack.back(), m_lineMerges),
                                                      CompilerBase::MT_WARNING,
                                                      QObject::tr("ignoring characters at the end of #else directive: ")
                                                                 .toStdString() + arguments );
            }
            if ( false == m_conditional.dirElse(m_locationStack.back()) )
            {
                m_compilerCore->preprocessorMessage ( locationCorrection(m_locationStack.back(), m_lineMerges),
                                                      CompilerBase::MT_ERROR,
                                                      QObject::tr("#else without an #if").toStdString() );
            }
            break;
        case DIR_ENDIF:
            if ( 0 != argLength )
            {
                m_compilerCore->preprocessorMessage (locationCorrection(m_locationStack.back(), m_lineMerges),
                                                     CompilerBase::MT_WARNING,
                                                     QObject::tr("ignoring characters at the end of #endif directive: ")
                                                                .toStdString() + arguments );
            }
            if ( false == m_conditional.dirEndif() )
            {
                m_compilerCore->preprocessorMessage ( locationCorrection(m_locationStack.back(), m_lineMerges),
                                                      CompilerBase::MT_ERROR,
                                                      QObject::tr("#endif without an #if").toStdString() );
            }
            break;
        case DIR_ERROR:
            m_compilerCore->preprocessorMessage ( locationCorrection(m_locationStack.back(), m_lineMerges),
                                                  CompilerBase::MT_ERROR,
                                                  arguments );
            break;
        case DIR_WARNING:
            m_compilerCore->preprocessorMessage ( locationCorrection(m_locationStack.back(), m_lineMerges),
                                                  CompilerBase::MT_WARNING,
                                                  arguments );
            break;
        case DIR_NULL:
            break;
    }

    return true;
}

inline void CompilerCPreProc::handleInclude ( char * arguments,
                                              unsigned int length,
                                              bool recursion )
{
    if ( '<' == arguments[0] )
    {
        if ( '>' != arguments[length-1] )
        {
            m_compilerCore->preprocessorMessage ( locationCorrection(m_locationStack.back()),
                                                  CompilerBase::MT_ERROR,
                                                  QObject::tr("missing terminating `>' character").toStdString() );
            return;
        }

        arguments[length-1] = '\0';
        m_include.m_file = ( arguments + 1 );
        m_include.m_system = true;
    }
    else if ( '"' == arguments[0] )
    {
        if ( '"' != arguments[length-1] )
        {
            m_compilerCore->preprocessorMessage ( locationCorrection(m_locationStack.back()),
                                                  CompilerBase::MT_ERROR,
                                                  QObject::tr("missing terminating `\"' character").toStdString() );
            return;
        }

        arguments[length-1] = '\0';
        m_include.m_file = ( arguments + 1 );
        m_include.m_system = false;
    }
    else if ( false == recursion )
    {
        Buffer out;
        m_macroTable.expand(out, Buffer(arguments, length));
        handleInclude(out.m_data, out.m_pos, true);
    }
    else
    {
        m_include.m_file = arguments;
        m_include.m_system = false;
    }
}

inline void CompilerCPreProc::tokenize ( std::vector<char*> & tokens,
                                         char * arguments,
                                         unsigned int length )
{
    for ( unsigned int i = 0; i < length; i++ )
    {
        if ( ( ' ' == arguments[i] ) || ( '\t' == arguments[i] ) )
        {
            arguments[i] = '\0';
        }
        else
        {
            if ( ( 0 == i ) || ( '\0' == arguments[i - 1] ) )
            {
                tokens.push_back(arguments + i);
            }
        }
    }
}

bool CompilerCPreProc::expectSigleId ( char * expr,
                                       unsigned int length )
{
    bool valid = ( ( 0 != length ) && ( ( '_' == expr[0] ) || isalpha(expr[0]) ) );

    for ( unsigned int i = 1; i < length; i++ )
    {
        if ( ( '_' != expr[i] ) && !isalnum(expr[i]) )
        {
            valid = false;
            break;
        }
    }

    if ( false == valid )
    {
        m_compilerCore->preprocessorMessage ( locationCorrection(m_locationStack.back(), m_lineMerges),
                                              CompilerBase::MT_ERROR,
                                              QObject::tr ( "expecting single identifier but got: " )
                                                          .toStdString() + expr );
    }

    return valid;
}

bool CompilerCPreProc::evaluateExpr ( char * expr,
                                      unsigned int length )
{
    // Local variables.
    yyscan_t yyscanner;                 // Pointer to the lexer context.
    Buffer out;                         //
    const Buffer in(expr, length);      //

    // Expand macros and operators defined() and sizeof().
    m_macroTable.expand(out, in, CompilerCPreProcMacros::EXP_EXPRESSION);

    // Initialize lexical analyzer for the arithmetic expression calculator.
    CompilerCPreProcCalcLex_lex_init_extra ( this, &yyscanner );
    YY_BUFFER_STATE bufferState = CompilerCPreProcCalcLex__scan_string ( out.m_data, yyscanner );

    // Initialize syntax analyzer for the arithmetic expression calculator.
    m_exprLocation = locationCorrection(m_locationStack.back(), m_lineMerges);
    CompilerCPreProcCalcPar_parse ( yyscanner, this );

    // Clean up.
    CompilerCPreProcCalcLex__delete_buffer ( bufferState, yyscanner );
    CompilerCPreProcCalcLex_lex_destroy ( yyscanner );

    // Done.
    return m_exprResult;
}

inline void CompilerCPreProc::handlePragma ( char * arguments,
                                             const unsigned int length )
{
    std::vector<char*> tokens;
    tokenize(tokens, arguments, length);

    if ( true == tokens.empty() )
    {
        m_compilerCore->preprocessorMessage ( locationCorrection(m_locationStack.back(), m_lineMerges),
                                              CompilerBase::MT_WARNING,
                                              QObject::tr("empty #pragma directive").toStdString() );
        return;
    }

    if ( 0 == strcmp(tokens[0], "STDC") )
    {
        if ( tokens.size() != 3 )
        {
            m_compilerCore->preprocessorMessage ( locationCorrection(m_locationStack.back(), m_lineMerges),
                                                  CompilerBase::MT_WARNING,
                                                  QObject::tr("invalid #pragma STDC directive").toStdString() );
            return;
        }

        enum {
            OPT_ON,
            OPT_OFF,
            OPT_DEFAULT
        } option;

        if ( 0 == strcmp(tokens[2], "ON") )
        {
            option = OPT_ON;
        }
        else if ( 0 == strcmp(tokens[2], "OFF") )
        {
            option = OPT_OFF;
        }
        else if ( 0 == strcmp(tokens[2], "DEFAULT") )
        {
            option = OPT_DEFAULT;
        }
        else
        {
            m_compilerCore->preprocessorMessage ( locationCorrection(m_locationStack.back()),
                                                  CompilerBase::MT_WARNING,
                                                  QObject::tr ( "this option for #pragma STDC directive is not "
                                                                "supported: ").toStdString() + tokens[2] );
            return;
        }

        if ( 0 == strcmp(tokens[1], "FP_CONTRACT") )
        {
            std::cout << ">>> #pragma STDC: FP_CONTRACT=" << option << '\n';
        }
        else if ( 0 == strcmp(tokens[1], "FENV_ACCESS") )
        {
            std::cout << ">>> #pragma STDC: FENV_ACCESS=" << option << '\n';
        }
        else if ( 0 == strcmp(tokens[1], "CX_LIMITED_RANGE") )
        {
            std::cout << ">>> #pragma STDC: CX_LIMITED_RANGE=" << option << '\n';
        }
        else
        {
            m_compilerCore->preprocessorMessage ( locationCorrection(m_locationStack.back()),
                                                  CompilerBase::MT_WARNING,
                                                  QObject::tr ( "this #pragma STDC directive is not supported: ")
                                                              .toStdString() + tokens[1] );
            return;
        }
    }
    else
    {
        m_compilerCore->preprocessorMessage ( locationCorrection(m_locationStack.back()),
                                              CompilerBase::MT_WARNING,
                                              QObject::tr ( "this #pragma directive is not supported: ")
                                                          .toStdString() + tokens[0] );
    }
}

inline void CompilerCPreProc::handleLine ( char * arguments,
                                           const unsigned int length )
{
    std::vector<char*> tokens;
    tokenize(tokens, arguments, length);

    // Validate input data size.
    if ( ( tokens.size() < 1 ) || ( tokens.size() > 2 ) )
    {
        m_compilerCore->preprocessorMessage ( locationCorrection(m_locationStack.back()),
                                              CompilerBase::MT_ERROR,
                                              QObject::tr ( "invalid syntax for #line directive").toStdString() );
        return;
    }

    // Read line number.
    long int linenum = strtol(tokens[0], nullptr, 0);
    if ( ( linenum <= 0 ) || ( linenum >= INT_MAX ) )
    {
        m_compilerCore->preprocessorMessage ( locationCorrection(m_locationStack.back()),
                                              CompilerBase::MT_ERROR,
                                              QObject::tr ( "invalid line number: ").toStdString() + tokens[0] );
        return;
    }

    // Read file name.
    if ( 2 == tokens.size() )
    {
        unsigned int len = strlen(tokens[1]);
        if ( len >= 2 )
        {
            if ( ( '"' == tokens[1][0] ) && ( '"' == tokens[1][len - 1] ) )
            {
                tokens[1][len - 1] = '\0';
                tokens[1]++;
            }
        }

        m_compilerCore->setVirtualFileName(tokens[1]);
    }

    // Alter current location accordingly.
    m_locationStack.back().m_lineStart = (int) linenum;
    m_locationStack.back().m_fileNumber = m_compilerCore->getFileNumber();
}
