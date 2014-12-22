// =============================================================================
/**
 * @brief
 * C++ Implementation: ...
 *
 * ...
 *
 * (C) copyright 2014 Moravia Microsystems, s.r.o.
 *
 * @ingroup CompilerC
 * @file CompilerCPreProcMacros.cxx
 */
// =============================================================================

#include "CompilerCPreProcMacros.h"

// Common compiler header files.
#include "CompilerOptions.h"
#include "CompilerParserInterface.h"

// Standard headers.
#include <ctime>
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

// const char * CompilerCPreProcMacros::STDC_VERSION = "199901L";

const std::set<std::string> CompilerCPreProcMacros::KEYWORDS =
{
    "defined",      "break",        "case",         "continue",     "default",
    "do",           "else",         "for",          "goto",         "if",
    "inline",       "return",       "restrict",     "sizeof",       "switch",
    "while",        "auto",         "char",         "const",        "double",
    "extern",       "float",        "enum",         "int",          "long",
    "register",     "short",        "signed",       "static",       "struct",
    "typedef",      "union",        "unsigned",     "void",         "volatile"
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
    { "__STDC__",                       { false, PRE_DEF__STDC } },
    { "__STDC_HOSTED__",                { false, PRE_DEF__STDC_HOSTED } },
    { "__STDC_IEC_559__",               { false, PRE_DEF__STDC_IEC_559 } },
    { "__STDC_IEC_559_COMPLEX__",       { false, PRE_DEF__STDC_IEC_559_COMPLEX } },
    { "__STDC_ISO_10646__",             { false, PRE_DEF__STDC_ISO_10646 } }
};

CompilerCPreProcMacros::CompilerCPreProcMacros ( const CompilerOptions * opts )
                                               : m_opts ( opts )
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
            std::cout<<"!!! UNMATCHED ()\n";
            // throw ...; unmatched ()
        }
    }
    else if ( ! IS_BLANK(macro[i]) )
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
    switch ( isReserved(macro) )
    {
        case -1:
            std::cout<<"!!! THIS MACRO CANNOT BE REDEFINED\n";
            throw 0;
        case 1:
            std::cout<<"[W] MACRO NAME IS LANGUAGE KEYWORD\n";
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
            std::cout<<"!!! VARIADIC SPECIFIER (`...') MUST APPEAR LAST IN THE PARAMETER LIST\n";
            throw 0;
        }

        for ( int pos = 0; '\0' != param[pos]; pos++ )
        {
            if ( ( !IS_IDENTIFIER(param, pos) || ( 0 != dots ) )
                   &&
                 ( ( '.' != param[pos] ) || ( ++dots > 3 ) ) )
            {
                std::cout<<"!!! MACRO PARAMETER MUST BE IDENTIFIER\n";
                throw 0;
            }
        }

        if ( ( 0 != dots ) && ( 3 != dots ) )
        {
            std::cout<<"!!! MACRO PARAMETER MUST BE IDENTIFIER, MAYBE YOU FORGOT A DOT IN `...' EXPRESSION\n";
            throw 0;
        }

        switch ( isReserved(param) )
        {
            case -1:
                std::cout<<"!!! CANNOT USE MACRO PARAMETER WITH THAT NAME\n";
                throw 0;
            case 1:
                std::cout<<"[W] MACRO PARAMETER IS LANGUAGE KEYWORD\n";
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
                std::cout<<"!!! DUPLICITE PARAMETER NAME\n";
                throw 0;
            }
        }
    }
}

void CompilerCPreProcMacros::undef ( char * macro )
{
    if ( -1 == isReserved(macro) )
    {
        std::cout<<"!!! THIS MACRO CANNOT BE UNDEFINED\n";
        throw 0;
    }

    const auto iter = m_table.find(macro);
    if ( m_table.cend() == iter )
    {
        // throw ...;
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
                std::cout << "[W] MISSING ARGUMENT FOR THE defined() OPERATOR\n";
                throw 0;
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
                        std::cout << "[W] POSSIBLE VARIADIC MACRO EXPANSION, TOO LITTLE ARGUMENTS PROVIDED\n";
                        start = -1;
                        continue;
                    }
                }
                else
                {
                    std::cout << "[W] POSSIBLE MACRO EXPANSION, INVALID NUMBER OF ARGUMENT(S)\n";
                    start = -1;
                    continue;
                }
            }

            pos += argLen;
        }

        if ( ++m_status.m_depth > m_opts->m_maxMacroExp )
        {
            std::cout << "!!! MAXIMUM EXPANSION DEPTH EXCEEDED\n";
            throw 0;
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
                // TODO:
                break;
            case PRE_DEF__LINE:
                // TODO:
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

void CompilerCPreProcMacros::getTimeOrDate ( Buffer & out,
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
        std::cout << "[W] POSSIBLE MACRO EXPANSION, MISSING ARGUMENT(S)\n";
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
            std::cout << "[W] POSSIBLE MACRO EXPANSION, UNMATCHED `()'\n";
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
                    std::cout << "!!! INVALID USE OF PREPROCESSING OPERATOR `##'\n";
                    throw 0;
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
