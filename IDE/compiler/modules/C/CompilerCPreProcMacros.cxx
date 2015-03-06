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
 * @file CompilerCPreProcMacros.cxx
 */
// =============================================================================

#include "CompilerCPreProcMacros.h"

// Common compiler header files.
#include "CompilerOptions.h"
#include "CompilerParserInterface.h"

// OS compatibility.
#include "utilities/os/os.h"

// Standard headers.
#include <ctime>
#include <cstdio>
#include <cctype>
#include <cstdlib>
#include <cstring>
#include<iostream>//DEBUG

#define IS_IDENTIFIER(str, i)                                   \
(                                                               \
       ( ( 0 != i ) && ( str[i] >= '0' ) && ( str[i] <= '9' ) ) \
    || ( ( str[i] >= 'a' ) && ( str[i] <= 'z' ) )               \
    || ( ( str[i] >= 'A' ) && ( str[i] <= 'Z' ) )               \
    || ( '_' == str[i] )                                        \
)

#define IS_SPACE(i) ( ( ' '  == i ) || ( '\t' == i ) )

#define IS_BLANK(i)                                      \
(                                                        \
         IS_SPACE(i) || ( '\0' == i ) || ( '\f' == i )   \
    || ( '\n' == i ) || ( '\v' == i ) || ( '\r' == i )   \
)

const char * CompilerCPreProcMacros::STDC_VERSION = "201104L";

const std::set<std::string> CompilerCPreProcMacros::KEYWORDS =
{
    "defined",      "break",        "case",         "continue",     "default",
    "do",           "else",         "for",          "goto",         "if",
    "inline",       "return",       "restrict",     "sizeof",       "switch",
    "while",        "auto",         "char",         "const",        "double",
    "extern",       "float",        "enum",         "int",          "long",
    "register",     "short",        "signed",       "static",       "struct",
    "typedef",      "union",        "unsigned",     "void",         "volatile",
    "_Bool",        "_Complex",     "_Imaginary"
};

const std::map<std::string, std::string> CompilerCPreProcMacros::NAMED_OPERATORS =
{
    { "and",    "&&" },    { "and_eq", "&=" },    { "bitand", "&"  },
    { "bitor",  "|"  },    { "compl",  "~"  },    { "not",    "!"  },
    { "not_eq", "!=" },    { "or",     "||" },    { "or_eq",  "|=" },
    { "xor",    "^"  },    { "xor_eq", "^=" }
};

const std::map<std::string, std::pair<bool, CompilerCPreProcMacros::PredefinedMacro>>
CompilerCPreProcMacros::PREDEFINED_MACROS =
{
    // Mandatory
    { "__DATE__",                       { true, PRE_DEF__DATE } },
    { "__TIME__",                       { true, PRE_DEF__TIME } },
    { "__FILE__",                       { true, PRE_DEF__FILE } },
    { "__LINE__",                       { true, PRE_DEF__LINE } },
    { "__STDC_VERSION__",               { true, PRE_DEF__STDC_VERSION } },

    // Optional
    { "__STDC__",                       { true,  PRE_DEF__STDC } },
    { "__STDC_HOSTED__",                { false, PRE_DEF__STDC_HOSTED } },
    { "__STDC_IEC_559__",               { false, PRE_DEF__STDC_IEC_559 } },
    { "__STDC_IEC_559_COMPLEX__",       { false, PRE_DEF__STDC_IEC_559_COMPLEX } },
    { "__STDC_ISO_10646__",             { false, PRE_DEF__STDC_ISO_10646 } },
    { "__STDC_MB_MIGHT_NEQ_WC__",       { false, PRE_DEF__STDC_MB_MIGHT_NEQ_WC } },
    { "__STDC_UTF_16__",                { false, PRE_DEF__STDC_UTF_16 } },
    { "__STDC_UTF_32__",                { false, PRE_DEF__STDC_UTF_32 } },
    { "__STDC_ANALYZABLE__",            { false, PRE_DEF__STDC_ANALYZABLE } },
    { "__STDC_LIB_EXT1__",              { false, PRE_DEF__STDC_LIB_EXT1 } },
    { "__STDC_NO_ATOMICS__",            { true,  PRE_DEF__STDC_NO_ATOMICS } },
    { "__STDC_NO_COMPLEX__",            { true,  PRE_DEF__STDC_NO_COMPLEX } },
    { "__STDC_NO_THREADS__ ",           { true,  PRE_DEF__STDC_NO_THREADS } },
    { "__STDC_NO_VLA__",                { false, PRE_DEF__STDC_NO_VLA } },
};

CompilerCPreProcMacros::CompilerCPreProcMacros ( CompilerParserInterface * compilerCore,
                                                 const CompilerOptions * opts,
                                                 std::vector<CompilerSourceLocation> & locationStack )
                                               : m_compilerCore ( compilerCore ),
                                                 m_opts ( opts ),
                                                 m_locationStack ( locationStack )
{
}

bool CompilerCPreProcMacros::isDefined ( const std::string & name )
{
    if ( NAMED_OPERATORS.cend() != NAMED_OPERATORS.find(name) )
    {
        return true;
    }

    const auto macro = PREDEFINED_MACROS.find(name);
    if ( ( PREDEFINED_MACROS.cend() != macro ) && ( true == macro->second.first ) )
    {
        return true;
    }

    return ( m_table.cend() != m_table.find(name) );
}

void CompilerCPreProcMacros::define ( char * macro,
                                      const int length )
{
    char * paramList = nullptr;
    char * body;

    int i = 0;
    while ( ( ++i < length ) && IS_IDENTIFIER(macro, i) );

    if ( '(' == macro[i] )
    {
        macro[i] = '\0';
        paramList = macro + i + 1;

        while ( ( ++i < length ) && ( ')' != macro[i] ) );
        if ( length == i )
        {
            throw MacroException("unmatched ()");
        }
    }
    else if ( ! IS_BLANK(macro[i]) )
    {
        throw MacroException(std::string("invalid name of macro: ") + macro[i]);
    }
    macro[i] = '\0';
    while ( ( ++i < length ) && isblank(macro[i]) );
    body = macro + i;

    if ( '\0' == macro[0] )
    {
        throw MacroException("no name for macro provided");
    }

    if ( m_table.cend() != m_table.find(macro) )
    {
        m_compilerCore->preprocessorMessage ( locationCorrection(m_locationStack.back()),
                                              CompilerBase::MT_WARNING,
                                              std::string("redefining macro: ") + macro );
    }

    switch ( isReserved(macro) )
    {
        case -1:
            throw MacroException("this macro cannot be redefined");
        case 1:
            m_compilerCore->preprocessorMessage ( locationCorrection(m_locationStack.back()),
                                                  CompilerBase::MT_WARNING,
                                                  std::string("macro name is language keyword: ") + macro );
    }

    m_table[macro] = { body, std::vector<std::string>() };

    if ( nullptr == paramList )
    {
        return;
    }

    std::vector<std::string> & parameters = m_table[macro].m_parameters;
    char * param = paramList;
    int dots = 0;
    int size = strlen(paramList);

    for ( i = 0; i <= size; i++ )
    {
        if ( ( ',' != paramList[i] ) && ( i != size ) )
        {
            continue;
        }

        paramList[i] = '\0';

        for ( int pos = ( i - 1 ); IS_SPACE(paramList[pos]); pos-- )
        {
            paramList[pos] = '\0';
        }

        while ( IS_SPACE(param[0]) )
        {
            param++;
        }

        if ( 0 != dots )
        {
            throw MacroException("variadic specifier (`...') must appear last in the parameter list");
        }

        for ( int pos = 0; '\0' != param[pos]; pos++ )
        {
            if ( ( !IS_IDENTIFIER(param, pos) || ( 0 != dots ) )
                   &&
                 ( ( '.' != param[pos] ) || ( ++dots > 3 ) ) )
            {
                throw MacroException("macro parameter must be identifier");
            }
        }

        if ( ( 0 != dots ) && ( 3 != dots ) )
        {
            throw MacroException("macro parameter must be identifier, maybe you forgot a dot in `...' expression");
        }

        switch ( isReserved(param) )
        {
            case -1:
                throw MacroException(std::string("cannot use macro parameter with that name: ") + param);
            case 1:
                m_compilerCore->preprocessorMessage ( locationCorrection(m_locationStack.back()),
                                                      CompilerBase::MT_WARNING,
                                                      std::string("macro parameter is language keyword: ") + param );
        }

        parameters.push_back(param);
        param = paramList + i + 1;
    }

    for ( i = 0; i < (int) parameters.size(); i++ )
    {
        for ( int j = i + 1; j < (int) parameters.size(); j++ )
        {
            if ( parameters[i] == parameters[j] )
            {
                throw MacroException(std::string("duplicite parameter name: ") + parameters[i]);
            }
        }
    }
}

void CompilerCPreProcMacros::undef ( char * macro )
{
    if ( -1 == isReserved(macro) )
    {
        throw MacroException("this macro cannot be undefined");
    }

    const auto iter = m_table.find(macro);
    if ( m_table.cend() == iter )
    {
        throw MacroException ( std::string("macro not defined: ") + macro );
    }
    m_table.erase(iter);
}

inline int CompilerCPreProcMacros::isReserved ( const char * word ) const
{
    if (
           ( PREDEFINED_MACROS.cend() != PREDEFINED_MACROS.find(word) )
               ||
           (
               ( true == m_opts->m_enableNamedOperators )
                   &&
               ( NAMED_OPERATORS.cend() != NAMED_OPERATORS.find(word) )
           )
       )
    {
        return -1;
    }

    return ( ( KEYWORDS.cend() == KEYWORDS.find(word) ) ? 0 : 1 );
}

void CompilerCPreProcMacros::expand ( Buffer & out,
                                      const Buffer & in,
                                      ExpansionMode expMode )
{
    Buffer expansionBuffer;
    int outpos = 0;
    int start = -1;
    InMode mode = MODE_NORMAL;

    for ( int pos = 0; pos <= in.m_pos; pos++ )
    {
        char input = in.m_data[pos];
        bool backslash = ( ( 0 != pos ) && ( '\\' == in.m_data[pos - 1] ) );

        if ( ( ( MODE_STRING == mode ) && ( '"'  == input ) ) || ( ( MODE_CHAR   == mode ) && ( '\'' == input) ) )
        {
            if ( false == backslash )
            {
                mode = MODE_NORMAL;
            }
            continue;
        }
        else if ( MODE_NORMAL != mode )
        {
            continue;
        }

        if ( false == backslash )
        {
            if ( '"' == in.m_data[pos] )
            {
                mode = MODE_STRING;
                continue;
            }
            else if ( '\'' == in.m_data[pos] )
            {
                mode = MODE_CHAR;
                continue;
            }
        }

        if ( ( isalnum(input) ) || ( '_' == input ) )
        {
            if ( -1 == start )
            {
                start = pos;
            }
            continue;
        }

        if ( -1 == start )
        {
            continue;
        }

        std::string name(in.m_data + start, pos - start);

        if ( ( EXP_EXPRESSION & expMode ) && ( "defined" == name ) )
        {
            std::string argument;
            pos += getArgument ( argument, in.m_data + pos );

            if ( true == argument.empty() )
            {
                throw MacroException("missing argument for operator defined()");
            }

            out.append(Buffer(in.m_data+outpos, start-outpos));
            out.append(isDefined(argument) ? "1" : "0");

            outpos = pos;
            start = -1;
            continue;
        }

        if ( true == prefinedMacro(out, in, name, start, outpos) )
        {
            outpos = pos;
            start = -1;
            continue;
        }

        const auto macro = m_table.find(name);
        if ( ( m_table.cend() == macro ) || ( m_status.m_macros.cend() != m_status.m_macros.find(name) ) )
        {
            start = -1;
            continue;
        }

        std::vector<std::string> argVector;
        if ( false == macro->second.m_parameters.empty() )
        {
            int argLen = getArgVector(argVector, in.m_data + pos, expMode);

            if ( -1 == argLen )
            {
                start = -1;
                continue;
            }

            if ( macro->second.m_parameters.size() != argVector.size() )
            {
                const std::string & lastParam = macro->second.m_parameters.back();
                const size_t paramSize = lastParam.size();
                if ( ( paramSize >= 3 ) && ( "..." == lastParam.substr(paramSize - 3, 3) ) )
                {
                    if ( argVector.size() < ( macro->second.m_parameters.size() - 1) )
                    {
                        m_compilerCore->preprocessorMessage ( locationCorrection(m_locationStack.back()),
                                                              CompilerBase::MT_WARNING,
                                                              "possible variadic macro expansion, too little arguments "
                                                              "provided");
                        start = -1;
                        continue;
                    }
                }
                else
                {
                    m_compilerCore->preprocessorMessage ( locationCorrection(m_locationStack.back()),
                                                          CompilerBase::MT_WARNING,
                                                          "possible macro expansion, invalid number of argument(s)");
                    start = -1;
                    continue;
                }
            }

            pos += argLen;
        }

        if ( ++m_status.m_depth > m_opts->m_maxMacroExp )
        {
            throw MacroException("maximum expansion depth exceeded");
        }
        m_status.m_macros.insert(name);

        out.append(Buffer(in.m_data+outpos, start-outpos));
        expansionBuffer.m_pos = 0;
        substitute(expansionBuffer, macro->second, argVector);
        expand(out, expansionBuffer, ExpansionMode(expMode | EXP_RECURSIVE));

        m_status.m_macros.erase(name);
        m_status.m_depth--;

        outpos = pos;
        start = -1;
    }

    out.append(Buffer(in.m_data + outpos, in.m_pos - outpos));
}

inline unsigned int CompilerCPreProcMacros::getArgument ( std::string & argument,
                                                          const char * string )
{
    unsigned int start;
    for ( start = 0; ( '\0' != string[start] ) && ( '_' != string[start] ) && ( !isalnum(string[start]) ); start++ );

    unsigned int end;
    for ( end = start; ( '\0' != string[end] ) && ( ( '_' == string[end] ) || ( isalnum(string[end]) ) ); end++ );

    argument.append(string + start, end - start);
    return end;
}


inline bool CompilerCPreProcMacros::prefinedMacro ( Buffer & out,
                                                    const Buffer & in,
                                                    const std::string & name,
                                                    int start,
                                                    int outpos )
{
    if ( true == m_opts->m_enableNamedOperators )
    {
        const auto oper = NAMED_OPERATORS.find(name);
        if ( NAMED_OPERATORS.cend() != oper )
        {
            out.append(Buffer(in.m_data+outpos, start-outpos));
            out.append(oper->second);
            return true;
        }
    }

    const auto macro = PREDEFINED_MACROS.find(name);
    if ( PREDEFINED_MACROS.cend() != macro )
    {
        out.append(Buffer(in.m_data+outpos, start-outpos));
        switch ( macro->second.second )
        {
            case PRE_DEF__FILE:
                getFileOrLine(out, true);
                break;
            case PRE_DEF__LINE:
                getFileOrLine(out, false);
                break;
            case PRE_DEF__DATE:
                getTimeOrDate(out, true);
                break;
            case PRE_DEF__TIME:
                getTimeOrDate(out, false);
                break;
            case PRE_DEF__STDC_VERSION:
                out.append(STDC_VERSION);
                break;

            default:
                if ( false == macro->second.first )
                {
                    return false;
                }
                out.m_data[out.m_pos++] = '1';
        }
        return true;
    }

    return false;
}

inline void CompilerCPreProcMacros::getFileOrLine ( Buffer & out,
                                                    bool file )
{
    using namespace boost::filesystem;

    if ( true == file )
    {
        out.m_data[out.m_pos++] = '"';
        std::string filename = m_compilerCore->getFileName ( m_locationStack.back().m_fileNumber );
        if ( true == path(filename).is_absolute() )
        {
            out.append ( make_relative ( m_compilerCore->getBasePath(), filename ).string() );
        }
        else
        {
            out.append(filename);
        }
        out.m_data[out.m_pos++] = '"';
    }
    else
    {
        char buffer[16];
        sprintf(buffer, "%d", ( m_locationStack.back().m_lineStart - 1 ) );
        out.append(buffer);
    }
}

inline void CompilerCPreProcMacros::getTimeOrDate ( Buffer & out,
                                                    bool date )
{
    time_t rawtime;
    struct tm * timeinfo;

    time(&rawtime);
    timeinfo = localtime(&rawtime);

    char buffer[16];
    strftime(buffer, 16, ( ( true == date ) ? "\"%b %e %Y\"" : "\"%T\"" ), timeinfo );
    out.append(buffer);
}

inline int CompilerCPreProcMacros::getArgVector ( std::vector<std::string> & argVector,
                                                  const char * string,
                                                  ExpansionMode expMode )
{
    if ( '(' != string[0] )
    {
        m_compilerCore->preprocessorMessage ( locationCorrection(m_locationStack.back()),
                                              CompilerBase::MT_WARNING,
                                              "possible macro expansion, missing argument(s)");
        return -1;
    }

    int end = 1;
    int start = 0;
    int parentheses = 1;
    InMode mode = MODE_NORMAL;

    for ( char input = string[end]; 0 != parentheses; input = string[++end] )
    {
        if ( '\0' == input )
        {
            m_compilerCore->preprocessorMessage ( locationCorrection(m_locationStack.back()),
                                                  CompilerBase::MT_WARNING,
                                                  "possible macro expansion, unmatched `()'");
            return -1;
        }

        if ( ( 0 != end ) && ( '\\' == string[end - 1] ) )
        {
            continue;
        }

        if ( ( ( MODE_STRING == mode ) && ( '"' == input ) ) || ( ( MODE_CHAR == mode ) && ( '\'' == input ) ) )
        {
            mode = MODE_NORMAL;
            continue;
        }
        else if ( MODE_NORMAL != mode )
        {
            continue;
        }

        switch ( input )
        {
            case '"':  mode = MODE_STRING; break;
            case '\'': mode = MODE_CHAR;   break;
            case '(':  parentheses++;      break;
            case ')':  parentheses--;      break;
        }

        if ( ( 0 == parentheses ) || ( ( 1 == parentheses ) && ( ',' == input ) ) )
        {
            while ( IS_SPACE(string[++start]) );
            start--;

            size_t length = end - start;
            for ( int j = ( end - 1 ); ( j >= 0 ) && IS_SPACE(string[j]); j-- )
            {
                length--;
            }

            argVector.push_back({string + start, length});
            start = end;
        }
    }

    if ( EXP_RECURSIVE & expMode )
    {
        for ( unsigned int i = 0; i < argVector.size(); i++ )
        {
            Buffer out;
            expand(out, argVector[i], EXP_NORMAL);
            argVector[i] = out.m_data;
        }
    }

    return end;
}

inline void CompilerCPreProcMacros::substitute ( Buffer & out,
                                                 const Macro & macro,
                                                 const std::vector<std::string> & argVector ) const
{
    int outpos = 0;
    int start = -1;
    int strf = -1;
    InMode mode = MODE_NORMAL;

    const Buffer in(macro.m_body);
    bool concat = false;
    bool hasParameters = !macro.m_parameters.empty();

    std::string vaArgsString;
    if ( true == hasParameters )
    {
        const std::string & lastParam = macro.m_parameters.back();
        const size_t paramSize = lastParam.size();
        if ( ( paramSize >= 3 ) && ( "..." == lastParam.substr(paramSize - 3, 3) ) )
        {
            if ( paramSize > 3 )
            {
                vaArgsString = lastParam.substr(0, paramSize - 3);
            }
            else
            {
                vaArgsString = "__VA_ARGS__";
            }
        }
    }

    for ( int pos = 0; pos <= in.m_pos; pos++ )
    {
        bool backslash = ( ( 0 != pos ) && ( '\\' == in.m_data[pos - 1] ) );

        if ( ( ( MODE_STRING == mode ) && ( '"'  == in.m_data[pos] ) )
                      ||
             ( ( MODE_CHAR   == mode ) && ( '\'' == in.m_data[pos]) ) )
        {
            if ( false == backslash )
            {
                start = -1;
                strf = -1;
                concat = false;
                mode = MODE_NORMAL;
            }
            continue;
        }

        if ( MODE_NORMAL != mode )
        {
            continue;
        }

        if ( false == backslash )
        {
            if ( '"' == in.m_data[pos] )
            {
                mode = MODE_STRING;
                continue;
            }
            else if ( '\'' == in.m_data[pos] )
            {
                mode = MODE_CHAR;
                continue;
            }
        }

        if ( ( isalnum(in.m_data[pos]) ) || ( '_' == in.m_data[pos] ) )
        {
            if ( -1 == start )
            {
                start = pos;
            }
            continue;
        }

        if ( ( true == hasParameters ) && ( -1 != start ) )
        {
            std::string word(in.m_data + start, pos - start);
            int idx = ( macro.m_parameters.size() - 1 );
            bool varArg = ( vaArgsString == word );

            if ( false == varArg )
            {
                for ( ; ( idx >= 0 ) && ( macro.m_parameters[idx] != word ); idx-- );
            }

            if ( -1 == idx )
            {
                start = pos;
            }
            else if ( -1 != strf )
            {
                start = strf;
            }

            out.append(Buffer(in.m_data+outpos, start-outpos));

            if ( -1 != idx )
            {
                substArg(out, argVector, idx, varArg, ( -1 != strf ), concat);
            }

            outpos = pos;
            start = -1;
            strf = -1;
        }

        concat = false;

        if ( '#' == in.m_data[pos] )
        {
            strf = pos;

            if ( '#' == in.m_data[ pos + 1 ] )
            {
                if ( ( 0 == pos ) || ( ( in.m_pos - 2 ) == pos ) )
                {
                    throw MacroException("invalid use of preprocessing operator `##'");
                }

                strf = -1;
                concat = true;

                int i = pos;
                while ( ( --i >= outpos ) && IS_SPACE(in.m_data[i]) );
                out.append(Buffer(in.m_data + outpos, 1 + i - outpos));

                for ( pos += 2; IS_SPACE(in.m_data[pos]); pos++ );
                outpos = pos--;
            }
        }
        else if ( !IS_SPACE(in.m_data[pos]) )
        {
            strf = -1;
        }
    }

    out.append(Buffer(in.m_data + outpos, in.m_pos - outpos));
}

inline void CompilerCPreProcMacros::substArg ( Buffer & out,
                                               const std::vector<std::string> & argVector,
                                               int idx,
                                               bool varArg,
                                               bool strf,
                                               bool concat ) const
{
    Buffer tmpBuffer;
    Buffer * target;

    if ( true == strf )
    {
        target = &tmpBuffer;
    }
    else
    {
        target = &out;
    }

    if ( true == varArg )
    {
        if ( ( true == concat ) && ( ',' == target->m_data[target->m_pos-1] ) )
        {
            target->m_pos--;
        }

        // Variable argument.
        bool first = true;
        for ( ; idx < (int) argVector.size(); idx++ )
        {
            if ( false == first )
            {
                target->append(", ");
            }
            target->append(argVector[idx]);
            first = false;
        }
    }
    else
    {
        target->append(argVector[idx]);
    }

    if ( true == strf )
    {
        // Stringification.
        stringify(out, tmpBuffer);
    }
}

inline void CompilerCPreProcMacros::stringify ( Buffer & out,
                                                const Buffer & in ) const
{
    unsigned int start = 0;
    unsigned int end = 0;
    InMode mode = MODE_NORMAL;

    out.m_data[out.m_pos++] = '"';

    for ( ; end < in.m_pos; end++ )
    {
        bool backslash = ( ( 0 != end ) && ( '\\' == in.m_data[end - 1] ) );

        if ( MODE_NORMAL == mode )
        {
            if ( false == backslash )
            {
                switch ( in.m_data[end] )
                {
                    case '"':
                        mode = MODE_STRING;
                        strEscape(out, in, start, end);
                        break;
                    case '\'':
                        mode = MODE_CHAR;
                        break;
                }
            }
        }
        else if ( MODE_STRING == mode )
        {
            switch ( in.m_data[end] )
            {
                case '"':
                    if ( false == backslash )
                    {
                        mode = MODE_NORMAL;
                    }
                case '\\':
                    strEscape(out, in, start, end);
                    break;
            }
        }
        else if ( MODE_CHAR == mode )
        {
            switch ( in.m_data[end] )
            {
                case '\'':
                    if ( false == backslash )
                    {
                        mode = MODE_NORMAL;
                        break;
                    }
                case '\\':
                    strEscape(out, in, start, end);
                    break;
            }
        }
    }

    out.append(Buffer(in.m_data + start, end - start));
    out.m_data[out.m_pos++] = '"';
}

inline void CompilerCPreProcMacros::strEscape ( Buffer & out,
                                                const Buffer & in,
                                                unsigned int & start,
                                                unsigned int & end ) const
{
    out.append(Buffer(in.m_data + start, end - start));
    out.m_data[out.m_pos++] = '\\';
    start = end;
}
