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
#include <cctype>
#include <cstdlib>
#include <cstring>
#include<iostream>//DEBUG

// getline() function.
#include "../../../utilities/os/getline.h"

#if ! defined ( YYSTYPE ) && ! defined ( YYSTYPE_IS_DECLARED )
    typedef union YYSTYPE {} YYSTYPE;
#endif
#include "CompilerCPreProcCalcLex.h"
int CompilerCPreProcCalcPar_parse ( yyscan_t yyscanner,
                                    CompilerCPreprocessorIntr * preprocessor );

#define IS_IDENTIFIER(str, i)                                   \
(                                                               \
       ( ( 0 != i ) && ( str[i] >= '0' ) && ( str[i] <= '9' ) ) \
    || ( ( str[i] >= 'a' ) && ( str[i] <= 'z' ) )               \
    || ( ( str[i] >= 'A' ) && ( str[i] <= 'Z' ) )               \
    || ( '_' == str[i] )                                        \
)

#define IS_BLANK(str, i)                                                \
(                                                                       \
       ( ' '  == str[i] ) || ( '\t' == str[i] ) || ( '\0' == str[i] )   \
    || ( '\n' == str[i] ) || ( '\v' == str[i] ) || ( '\r' == str[i] )   \
    || ( '\f' == str[i] )                                               \
)

const std::map<std::string, CompilerCPreprocessor::Directive> CompilerCPreprocessor::s_directives =
{
    { "include", DIR_INCLUDE },    { "line",    DIR_LINE    },
    { "pragma",  DIR_PRAGMA  },    { "define",  DIR_DEFINE  },
    { "undef",   DIR_UNDEF   },    { "if",      DIR_IF      },
    { "elif",    DIR_ELIF    },    { "ifdef",   DIR_IFDEF   },
    { "ifndef",  DIR_IFNDEF  },    { "else",    DIR_ELSE    },
    { "endif",   DIR_ENDIF   },    { "warning", DIR_WARNING },
    { "error",   DIR_ERROR   }

};

const std::map<std::string, std::string> CompilerCPreprocessor::MacroTable::s_namedOperators =
{
    { "and",    "&&" },    { "and_eq", "&=" },    { "bitand", "&"  },
    { "bitor",  "|"  },    { "compl",  "~"  },    { "not",    "!"  },
    { "not_eq", "!=" },    { "or",     "||" },    { "or_eq",  "|=" },
    { "xor",    "^"  },    { "xor_eq", "^=" }
};

const std::set<std::string> CompilerCPreprocessor::MacroTable::s_keywords =
{
    "defined",      "break",        "case",         "continue",     "default",
    "do",           "else",         "for",          "goto",         "if",
    "inline",       "return",       "restrict",     "sizeof",       "switch",
    "while",        "auto",         "char",         "const",        "double",
    "extern",       "float",        "enum",         "int",          "long",
    "register",     "short",        "signed",       "static",       "struct",
    "typedef",      "union",        "unsigned",     "void",         "volatile"
};

CompilerCPreprocessor::CompilerCPreprocessor ( CompilerParserInterface * compilerCore,
                                               CompilerOptions * opts )
                                             :
                                               CompilerCPreprocessorIntr(compilerCore, opts),
                                               m_macroTable ( opts )
{
}

char * CompilerCPreprocessor::processFiles ( const std::vector<FILE*> & inputFiles )
{
    Buffer inBuffer;    // Input buffer for reading the source file.
    Buffer outBuffer;   // Output buffer for storing the preprocessor output.
    Buffer mergeBuffer; // Auxiliary buffer for merging lines using `\' (backslash) or the "backslash" trigraph.
    Buffer mlineBuffer; // Auxiliary buffer for merging lines containing C multi-line comment: /* ... <new-line> ... */.

    // Start in the Normal mode.
    m_inmode = MODE_NORMAL;

    // Iterate over all given input files.
    std::vector<FILE*> fileStack;
    for ( auto sourceFile : inputFiles )
    {
        for ( fileStack.push_back(sourceFile); false == fileStack.empty(); fileStack.pop_back() )
        {
            // Iterate over the lines in the source file.
            while ( -1 != ( inBuffer.m_pos = getline(&inBuffer.m_data, &inBuffer.m_size, fileStack.back()) ) )
            {
                if ( false == inputProcessing(inBuffer, outBuffer, mergeBuffer, mlineBuffer) )
                {
                    // Critical error.
                    return nullptr;
                }

                if ( false == m_include.m_file.empty() )
                {
                    fileStack.push_back(m_compilerCore->fileOpen(m_include.m_file, nullptr, false, m_include.m_system));
                    m_include.m_file.clear();

                    if ( nullptr == fileStack.back() )
                    {
                        // Critical error.
                        return nullptr;
                    }
                }
            }

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
        // TODO: error: unterminated condition
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

    // Done.
    outBuffer.m_persistent = true;
    return outBuffer.m_data;
}

inline bool CompilerCPreprocessor::inputProcessing ( Buffer & inBuffer,
                                                     Buffer & outBuffer,
                                                     Buffer & mergeBuffer,
                                                     Buffer & mlineBuffer )
{
    // Handle line merging - trailing `\' (backslash)
    if ( true == lineMerging(inBuffer, mergeBuffer) )
    {
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

    return true;
}

inline bool CompilerCPreprocessor::lineMerging ( Buffer & inBuffer,
                                                 Buffer & mergeBuffer )
{
    int lineMerge = -1;  // Position of the line merge mark - "\" (backslash) or the "? ? /" trigraph.
    int whiteSpace = -1; // Position of the first white space character following the line merge mark (warning cond.).

    // Search the line backwards for certain characters.
    for ( ssize_t pos = ( inBuffer.m_pos - 1 ); pos >= 0; pos-- )
    {
        switch ( inBuffer.m_data[pos] )
        {
            // Skip white space.
            case '\t': case ' ':
                whiteSpace = (int) pos;
            // Skip EOL characters.
            case '\r': case '\n':
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
                    if ( ( pos >= 2 ) && ( '?' == inBuffer.m_data[pos-1] ) && ( '?' == inBuffer.m_data[pos-2] ) )
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
            // TODO: Warning.
        }

        // Append contents of the input buffer to the merge buffer.
        inBuffer.m_pos = lineMerge;
        mergeBuffer.append(inBuffer);

        // The input buffer contents was appended to the merge buffer, and therefore should be ignored for further
        //+ processing until the merge is entirely complete.
        return true;
    }
    // Line merge not found but the merge buffer is not empty, merge the input buffer with the merge buffer.
    else if ( 0 != mergeBuffer.m_pos )
    {
        // Append contents of the input buffer to the merge buffer.
        mergeBuffer.append(inBuffer);

        // Copy the entire merge buffer to the input buffer, and clear the merge buffer.
        inBuffer.move(mergeBuffer);
    }

    return false;
}

inline void CompilerCPreprocessor::cutLine ( ssize_t & length,
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

inline void CompilerCPreprocessor::Buffer::append ( const Buffer & sourceBuffer )
{
    // Check if there is enough space left in the target buffer; and if not, enlarge the buffer.
    const size_t requiredMinSize = ( sourceBuffer.m_pos + m_pos + 1 );
    if ( requiredMinSize > m_size )
    {
        m_size = requiredMinSize;
        m_data = (char*) realloc ( m_data, m_size );
    }

    // Append contents of the source buffer to the target buffer.
    memcpy((m_data + m_pos), sourceBuffer.m_data, sourceBuffer.m_pos);
    m_pos += sourceBuffer.m_pos;
    m_data[m_pos] = '\0';
}

inline void CompilerCPreprocessor::Buffer::move ( Buffer & sourceBuffer )
{
    char * auxBuf = m_data;
    m_data = sourceBuffer.m_data;
    sourceBuffer.m_data = auxBuf;

    size_t auxSize = m_size;
    m_size = sourceBuffer.m_size;
    sourceBuffer.m_size = auxSize;

    m_pos = sourceBuffer.m_pos;
    sourceBuffer.m_pos = 0;
}

CompilerCPreprocessor::Buffer::Buffer()
{
    m_pos = 0;
    m_size = 0;
    m_data = nullptr;
    m_persistent = false;
}

// CompilerCPreprocessor::Buffer::Buffer ( const std::string & data )
// {
//     m_pos = data.size();
//     m_size = data.size() + 1;
//     m_data = (char*) malloc(m_size);
//     m_persistent = false;
//
//     memcpy(m_data, data.c_str(), m_size);
//     m_data[m_pos] = '\0';
// }

CompilerCPreprocessor::Buffer::Buffer ( char * data,
                                        unsigned int length )
{
    m_pos = length;
    m_size = length + 1;
    m_data = data;
    m_persistent = true;
}

CompilerCPreprocessor::Buffer::~Buffer()
{
    if ( ( false == m_persistent ) && ( nullptr != m_data ) )
    {
        free(m_data);
    }
}

CompilerCPreprocessor::Include::Detection::Detection()
{
    reset();
}

inline void CompilerCPreprocessor::Include::Detection::reset()
{
    m_state = STATE_WHITE_SPACE_0;
}

inline void CompilerCPreprocessor::Include::Detection::input ( char in )
{
    switch ( m_state )
    {
        case STATE_NEGATIVE:
        case STATE_POSITIVE:
            break;
        case STATE_WHITE_SPACE_0:
            switch ( in )
            {
                case ' ':
                case '\t':
                    break;
                case '#':
                    m_state = STATE_WHITE_SPACE_1;
                    break;
                default:
                    m_state = STATE_NEGATIVE;
            }
            break;
        case STATE_WHITE_SPACE_1:
            switch ( in )
            {
                case ' ':
                case '\t':
                    break;
                case 'i':
                    m_state = STATE_I;
                    break;
                default:
                    m_state = STATE_NEGATIVE;
            }
            break;
        case STATE_I:    m_state = ( ( 'n' == in ) ? STATE_N             : STATE_NEGATIVE ); break;
        case STATE_N:    m_state = ( ( 'c' == in ) ? STATE_C             : STATE_NEGATIVE ); break;
        case STATE_C:    m_state = ( ( 'l' == in ) ? STATE_L             : STATE_NEGATIVE ); break;
        case STATE_L:    m_state = ( ( 'u' == in ) ? STATE_U             : STATE_NEGATIVE ); break;
        case STATE_U:    m_state = ( ( 'd' == in ) ? STATE_D             : STATE_NEGATIVE ); break;
        case STATE_D:    m_state = ( ( 'e' == in ) ? STATE_POSITIVE      : STATE_NEGATIVE ); break;
    }
}

inline bool CompilerCPreprocessor::Include::Detection::detected() const
{
    return ( STATE_POSITIVE == m_state );
}

inline bool CompilerCPreprocessor::initialProcessing ( Buffer & buffer,
                                                       bool & lineMerge )
{
    unsigned int out = 0;
    bool copy = true;

    for ( unsigned int in = 0; in < buffer.m_pos; in++ )
    {
        const char inchar   = buffer.m_data[in];
        const char nextchar = buffer.m_data[1+in];

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
                        m_inmode = MODE_STRING;
                        break;
                    case '\'':
                        m_inmode = MODE_CHAR;
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
                                break;
                        }
                        break;
                    case '<':
                        if ( ( true == m_include.m_detection.detected() ) && ( ( false == m_opts->m_enableDigraphs ) || ( ( '%' != nextchar ) && ( ':' != nextchar ) ) ) )
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
                if ( '"' == inchar )
                {
                    m_inmode = MODE_NORMAL;
                }
                break;
            case MODE_CHAR:
                if ( '\'' == inchar )
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

inline bool CompilerCPreprocessor::directivesProcessing ( Buffer & buffer )
{
    for ( int in = 0; in < buffer.m_pos; in++ )
    {
        if ( '#' == buffer.m_data[in] )
        {
            while ( ( ++in < buffer.m_pos ) && isblank(buffer.m_data[in]) );
            int start = in;
            while ( ( ++in < buffer.m_pos ) && isalpha(buffer.m_data[in]) );

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
            const auto iter = s_directives.find(directive);
            if ( s_directives.cend() == iter )
            {
                // TODO: Error: "Invalid preprocessing directive"
                return false;
            }

            return handleDirective(arguments, iter->second);
        }
        else if ( ( ' ' != buffer.m_data[in] ) && ( '\t' != buffer.m_data[in] ) )
        {
            return true;
        }
    }

    return true;
}

inline bool CompilerCPreprocessor::handleDirective ( char * arguments,
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
            handleLine(arguments, argLength);
            break;
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
            m_conditional.dirIf(evaluateExpr(arguments, argLength));
            break;
        case DIR_ELIF:
            m_conditional.dirElif(evaluateExpr(arguments, argLength));
            break;
        case DIR_IFDEF:
            m_conditional.dirIf(m_macroTable.isDefined(arguments));
            break;
        case DIR_IFNDEF:
            m_conditional.dirIf(!m_macroTable.isDefined(arguments));
            break;
        case DIR_ELSE:
//         TODO: warning if arguments present
            m_conditional.dirElse();
            break;
        case DIR_ENDIF:
//         TODO: warning if arguments present
            m_conditional.dirEndif();
            break;
        case DIR_ERROR:
//         throw ...;
            break;
        case DIR_WARNING:
//         TODO: warning
            break;
    }

    return true;
}

inline void CompilerCPreprocessor::handleInclude ( char * arguments,
                                                   unsigned int length )
{
    if ( '<' == arguments[0] )
    {
        if ( '>' != arguments[length-1] )
        {
//         throw ...;
        }

        m_include.m_system = true;
    }
    else if ( '"' == arguments[0] )
    {
        if ( '"' != arguments[length-1] )
        {
//         throw ...;
        }

        m_include.m_system = false;
    }
    else
    {
//         throw ...;
    }

    arguments[length-1] = '\0';
    m_include.m_file = ( arguments + 1 );
}

bool CompilerCPreprocessor::evaluateExpr ( char * expr,
                                           unsigned int length )
{
    // Local variables.
    yyscan_t yyscanner;                 // Pointer to the lexer context.
    Buffer out;                         //
    const Buffer in(expr, length);      //

    // Expand macros and operators defined() and sizeof().
    m_macroTable.expand(out, in, true);

    // Initialize lexical analyzer for the arithmetic expression calculator.
    CompilerCPreProcCalcLex_lex_init_extra ( this, &yyscanner );
    YY_BUFFER_STATE bufferState = CompilerCPreProcCalcLex__scan_string ( out.m_data, yyscanner );

    // Initialize syntax analyzer for the arithmetic expression calculator.
    CompilerCPreProcCalcPar_parse ( yyscanner, this );

    // Clean up.
    CompilerCPreProcCalcLex__delete_buffer ( bufferState, yyscanner );
    CompilerCPreProcCalcLex_lex_destroy ( yyscanner );

    // Done.
    return m_exprResult;
}

inline void CompilerCPreprocessor::handleLine ( char * /*arguments*/,
                                                unsigned int /*length*/ )
{
}

inline void CompilerCPreprocessor::handlePragma ( char * /*arguments*/,
                                                  unsigned int /*length*/ )
{
}

inline bool CompilerCPreprocessor::Conditional::get()
{
    if ( true == m_stack.empty() )
    {
        return true;
    }
    return ( STATE_POSITIVE == m_stack.back() );
}

inline void CompilerCPreprocessor::Conditional::dirIf ( bool condition )
{
    m_stack.push_back ( condition ? STATE_POSITIVE : STATE_NEGATIVE );
}

inline void CompilerCPreprocessor::Conditional::dirElif ( bool condition )
{
    if ( true == m_stack.empty() )
    {
//         throw ...;
    }

    switch ( m_stack.back() )
    {
        case STATE_POSITIVE:
            m_stack.back() = STATE_CLOSED;
            break;
        case STATE_NEGATIVE:
            m_stack.back() = ( condition ? STATE_POSITIVE : STATE_NEGATIVE );
            break;
        case STATE_CLOSED:
            break;
    }
}

inline void CompilerCPreprocessor::Conditional::dirEndif()
{
    if ( true == m_stack.empty() )
    {
//         throw ...;
    }
    m_stack.pop_back();
}

inline void CompilerCPreprocessor::Conditional::dirElse()
{
    if ( true == m_stack.empty() )
    {
//         throw ...;
    }

    switch ( m_stack.back() )
    {
        case STATE_POSITIVE:
            m_stack.back() = STATE_CLOSED;
            break;
        case STATE_NEGATIVE:
            m_stack.back() = STATE_POSITIVE;
            break;
        case STATE_CLOSED:
            break;
    }
}

inline bool CompilerCPreprocessor::Conditional::empty()
{
    return m_stack.empty();
}

inline bool CompilerCPreprocessor::MacroTable::isDefined ( const char * name )
{
    return ( m_table.cend() != m_table.find(name) );
}

inline void CompilerCPreprocessor::MacroTable::define ( char * macro,
                                                        const int length )
{
    char * paramList;
    char * body;

    int i = 0;
    while ( ( ++i < length ) && IS_IDENTIFIER(macro, i) );
    paramList = macro + i;
    if ( '(' == macro[i] )
    {
        macro[i] = '\0';
        paramList = macro + i + 1;

        while ( ( ++i < length ) && ( ')' != macro[i] ) );
        if ( length == i )
        {
            std::cout<<"!!! UNMATCHED ()\n";
            // throw ...; unmatched ()
        }
    }
    else if ( ! IS_BLANK(macro, i) )
    {
        std::cout<<"!!! INVALID NAME OF MACRO ('"<<macro[i]<<"')\n";
        // throw ...; invalid name of macro
    }
    macro[i] = '\0';
    while ( ( ++i < length ) && isblank(macro[i]) );
    body = macro + i;

    if ( '\0' == macro[0] )
    {
        std::cout<<"!!! NO NAME PROVIDED\n";
        // throw ...; no name provided
    }
    if ( m_table.cend() != m_table.find(macro) )
    {
        std::cout<<"[W] REDEFINING MACRO\n";
    }

    m_table[macro] = { body, std::vector<std::string>() };

    std::vector<std::string> & parameters = m_table[macro].m_parameters;
    char * param = paramList;
    int size = strlen(paramList);
    for ( i = 0; i <= size; i++ )
    {
        if ( ( ',' != paramList[i] ) && ( i != size ) )
        {
            continue;
        }

        paramList[i] = '\0';

        for ( int pos = ( i - 1 ); ( ' ' == paramList[pos] ) || ( '\t' == paramList[pos] ); pos-- )
        {
            paramList[pos] = '\0';
        }

        while ( ( ' ' == param[0] ) || ( '\t' == param[0] ) )
        {
            param++;
        }

        int dots = 0;
        for ( int pos = 0; '\0' != param[pos]; pos++ )
        {
            if ( ( !IS_IDENTIFIER(param, pos) || ( 0 != dots ) )
                   &&
                 ( ( '.' != param[pos] ) || ( ++dots > 3 ) ) )
            {
                std::cout<<"!!! MACRO PARAMETER MUST BE IDENTIFIER\n";
                // throw ...;
            }
        }

        if ( '\0' != param[0] )
        {
            if ( true == isReserved(param) )
            {
                std::cout<<"[W] MACRO PARAMETER IS RESERVED KEYWORD\n";
            }

            parameters.push_back(param);
        }

        param = paramList + i + 1;
    }

    if ( true == isReserved(macro) )
    {
        std::cout<<"[W] MACRO NAME IS RESERVED KEYWORD\n";
        // throw ...;
    }
}

inline void CompilerCPreprocessor::MacroTable::undef ( char * macro )
{
    const auto iter = m_table.find(macro);
    if ( m_table.cend() == iter )
    {
        // throw ...;
    }
    m_table.erase(iter);
}

inline bool CompilerCPreprocessor::MacroTable::isReserved ( const char * word ) const
{
    if ( ( true == m_opts->m_enableNamedOperators ) && ( s_namedOperators.cend() != s_namedOperators.find(word) ) )
    {
        return true;
    }

    return ( s_keywords.cend() != s_keywords.find(word) );
}

void CompilerCPreprocessor::MacroTable::expand ( Buffer & out,
                                                 const Buffer & in,
                                                 bool inExpression )
{
    int outpos = 0;
    int start = -1;
    InMode mode = MODE_NORMAL;

    for ( int pos = 0; pos <= in.m_pos; pos++ )
    {
        if ( MODE_NORMAL == mode)
        {
            if ( '"' == in.m_data[pos] )
            {
                mode = MODE_STRING;
            }
            else if ( '\'' == in.m_data[pos] )
            {
                mode = MODE_CHAR;
            }
            else if ( ( isalnum(in.m_data[pos]) ) || ( '_' == in.m_data[pos] ) )
            {
                if ( -1 == start )
                {
                    start = pos;
                }
            }
            else
            {
                if ( -1 == start )
                {
                    continue;
                }

                int length = pos - start;
                char name [ length + 1 ];
                memcpy(name, in.m_data + start, length);
                name[length] = '\0';

                if ( ( true == inExpression ) && ( 0 == strcmp(name, "defined") ) )
                {
                    std::vector<std::string> argVector;
                    int argLen = getArgVector(argVector, in.m_data + pos);
                    if ( -1 == argLen )
                    {
                        start = -1;
                        continue;
                    }
                    pos += argLen;

                    if ( 1 != argVector.size() )
                    {
                        std::cout << "!!! INVALID NUMBER OF ARGUMENTS\n";
                        throw 0;
                    }

                    char value[2];
                    value[0] = ( ( true == isDefined(argVector[0].c_str()) ) ? '1' : '0' );
                    value[1] = '\0';

                    out.append(Buffer(in.m_data+outpos, start-outpos));
                    out.append(Buffer(value, 1));
                    outpos = pos;
                }
                else
                {
                    const auto macro = m_table.find(name);
                    if ( ( m_table.cend() == macro ) || ( m_status.m_macros.cend() != m_status.m_macros.find(name) ) )
                    {
                        start = -1;
                        continue;
                    }

                    std::vector<std::string> argVector;
                    if ( false == macro->second.m_parameters.empty() )
                    {
                        int argLen = getArgVector(argVector, in.m_data + pos);
                        if ( -1 == argLen )
                        {
                            start = -1;
                            continue;
                        }
                        pos += argLen;
                    }

                    if ( ++m_status.m_depth > m_opts->m_maxMacroExp )
                    {
                        std::cout << "!!! EXPANSION DEPTH EXCEEDED\n";
                        throw 0;
                    }

                    out.append(Buffer(in.m_data+outpos, start-outpos));
                    outpos = pos;

                    m_status.m_macros.insert(name);

                    Buffer macroBody;
                    substitute(macroBody, macro->second, argVector);
                    expand(out, macroBody, inExpression);

                    m_status.m_macros.erase(name);
                    m_status.m_depth--;
                }

                start = -1;
            }
        }
        else if ( ( MODE_STRING == mode ) && ( '"' == in.m_data[pos] ) )
        {
            mode = MODE_NORMAL;
        }
        else if ( ( MODE_CHAR == mode ) && ( '\'' == in.m_data[pos] ) )
        {
            mode = MODE_NORMAL;
        }
    }

    out.append(Buffer(in.m_data + outpos, in.m_pos - outpos));
}

int CompilerCPreprocessor::MacroTable::getArgVector ( std::vector<std::string> & argVector,
                                                      char * string )
{
    if ( '(' != string[0] )
    {
        std::cout << "[W] POSSIBLE MACRO EXPANSION, MISSING ARGUMENT(S)\n";
        return -1;
    }

    int i = 0;
    while ( ( '\0' != string[++i] ) && ( ')' != string[i] ) );
    if ( '\0' == string[i] )
    {
        std::cout << "[W] POSSIBLE MACRO EXPANSION, MISSING `)'\n";
        return -1;
    }

    int length = i - 1;
    char arguments [ length + 1 ];
    memcpy(arguments, string + 1, length);
    arguments[length] = '\0';

    char * argStart = arguments;
    for ( i = 0; i <= length; i++ )
    {
        if ( ( ',' != arguments[i] ) && ( i != length ) )
        {
            continue;
        }

        arguments[i] = '\0';

        for ( int j = ( i - 1 ); ( ' ' == arguments[j] ) || ( '\t' == arguments[j] ); j-- )
        {
            arguments[j] = '\0';
        }

        while ( ( ' ' == argStart[0] ) || ( '\t' == argStart[0] ) )
        {
            argStart++;
        }

        if ( '\0' != argStart[0] )
        {
            argVector.push_back(argStart);
        }

        argStart = arguments + i + 1;
    }

    return ( length + 2 );
}

inline void CompilerCPreprocessor::MacroTable::substitute ( Buffer & out,
                                                            const Macro & macro,
                                                            const std::vector<std::string> & argVector ) const
{
    for ( auto x : macro.m_parameters ) std::cout << "#### P='"<<x<<"'\n";
    for ( auto x : argVector ) std::cout << "#### A='"<<x<<"'\n";
    out.append(Buffer(const_cast<char*>(macro.m_body.c_str()), macro.m_body.size()));
}
