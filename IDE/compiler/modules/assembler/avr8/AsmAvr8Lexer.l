/* ========================================================================== */
/**
 * @brief
 *
 * ...
 *
 * (C) copyright 2013, 2014 Moravia Microsystems, s.r.o.
 *
 * @author Martin Ošmera <martin.osmera@moravia-microsystems.com>
 */
/* ========================================================================== */


/* -------------------------------------------------------------------------- */
/* PROLOGUE - FLEX DECLARATIONS                                               */
/* -------------------------------------------------------------------------- */

%{
    // Standard header files
    #include <iostream>
    #include <cctype>
    #include <cstdlib>
    #include <cstdio>
    #include <cstdint>
    // Assembler interface to the syntax analyzer (parser) and the lexical analyzer (lexer/scaner)
    #include "CompilerParserInterface.h"
    // Header file for the parser generated by Bison
    #include "AsmAvr8Parser.h"
    // Functions for handling escape sequences in strings, etc.
    #include "CompilerLexerUtils.h"
    using namespace CompilerLexerUtils;

    // Each time lexer matches a string, move the end cursor to its end.
    #define YY_USER_ACTION \
        yylloc->last_column += yyleng;

    /**
     * @brief
     * @param[in,out] yylval
     * @param[in,out] compiler
     * @param[in] byMin
     */
    inline void enlargeStringBuffer ( YYSTYPE * yylval,
                                      CompilerParserInterface * compiler,
                                      int byMin = 1 );
%}

/* data type of the user-specific data to be stored in compiler */
%option extra-type="CompilerParserInterface *"
/* instructs flex to write a C header to FILE */
%option header-file="AsmAvr8Lexer.h"
/* changes the default `yy' prefix used by flex for all globally-visible variable and function names */
%option prefix="AsmAvr8Lexer_"
/* instruct flex that GNU bison %locations are being used */
%option bison-locations
/* instructs flex to generate a C scanner that is meant to be called by a GNU bison parser */
%option bison-bridge
/* instructs flex to generate a case-insensitive scanner */
%option case-insensitive
/* instructs flex to generate a scanner which never considers its input interactive */
%option never-interactive
/* instructs flex to generate a reentrant C scanner */
%option reentrant
/* instructs flex to generate an 8-bit scanner, i.e., one which can recognize 8-bit characters */
%option 8bit
/* in general, you use `-B' when you are certain that your scanner will never be used interactively */
%option batch
/* suppresses inclusion of the non-ANSI header file unistd.h. */
%option nounistd
/* directs flex to generate a scanner that maintains the number of the current line read from its input */
%option yylineno
/* makes the scanner not call yywrap() upon an end-of-file */
%option noyywrap
/* generate warning messages */
%option warn
/* enable use of start condition stacks */
%option stack
/* get rid of the warning message: 'int yy_top_state(yyscan_t)’ defined but not used */
%option noyy_top_state

/*
 * Named regular expressions used in the lexer
 */
/* White space */
WSPACE  ([ \t]|\u00A0)+
/* New line */
NLINE   (\r?\n)|(\n?\r)
/* An identifier in Assembly language */
ID      [_A-Z][_A-Z0-9]*
/* Comment, in this case it starts with a semicolon (`;') */
COMMENT (;[^\r\n]*)
/* C like escape sequence, such as `\x4A', `\uABCD', `\U1234abcd', and `\177' */
XESC    (\\[0-7]{1,3})|(\\x[0-9A-F]{1,2})|(\\[uU][0-9A-F]{1,8})

/*
 * Exclusive start conditions
 */
/* Inside a string, e.g. "my string" */
%x STR
/* Inside a string, or a character literal; e.g. 'my string', or 'a' */
%x QUOTE
/* Multi-line comment */
%x MCOMMENT
/* Inside an INCLUDE directive */
%x INC
%x INC_END


/* -------------------------------------------------------------------------- */
/* LEXICAL RULES                                                              */
/* -------------------------------------------------------------------------- */

%%

%{
    // Terminate lexical analysis, in case all input files have been closed using the EXIT' directive.
    if ( nullptr == AsmAvr8Lexer_get_in(yyscanner) )
    {
        yyterminate();
    }

    // At each yylex invocation, mark the current position as the start of the next token.
    yylloc_param->first_line   = yylloc_param->last_line;
    yylloc_param->first_column = yylloc_param->last_column;
%}

{WSPACE}        { /* eat up white space */ }
{NLINE}         {
                    yylloc->last_line++;
                    yylloc->last_column = 1;
                    return EOL;
                }

;{COMMENT}          { return COMMENT; }
{COMMENT}           { /* ignore ordinary comment */ }
<INITIAL,INC,INC_END>
{
    "/*"            { yy_push_state(MCOMMENT, yyscanner); }
}
<MCOMMENT>
{
    "*/"            { yy_pop_state(yyscanner); }
    [^*\n]*         { /* eat anything that's not a '*' */ }
    "*"+[^*/\n]*    { /* eat up '*'s not followed by '/'s */ }
    {NLINE}         {
                        yylloc->last_line++;
                        yylloc->last_column = 1;
                        return EOL;
                    }
}

\.?"if"         { return D_IF; }
\.?"ifn"        { return D_IFN; }
\.?"ifdef"      { return D_IFDEF; }
\.?"ifndef"     { return D_IFNDEF; }
\.?"elseifb"    { return D_ELSEIFB; }
\.?"elseifnb"   { return D_ELSEIFNB; }
\.?"else"       { return D_ELSE; }
\.?"elseif"     { return D_ELSEIF; }
\.?"elseifn"    { return D_ELSEIFN; }
\.?"elseifdef"  { return D_ELSEIFDEF; }
\.?"elseifndef" { return D_ELSEIFNDEF; }
\.?"endif"      { return D_ENDIF; }
\.?"org"        { return D_ORG; }
\.?"local"      { return D_LOCAL; }
\.?"ifnb"       { return D_IFNB; }
\.?"ifb"        { return D_IFB; }
\.?"skip"       { return D_SKIP; }
\.?"endmacro"   { return D_ENDM; }
\.?"endm"       { return D_ENDM; }
\.?"exitm"      { return D_EXITM; }
\.?"rept"       { return D_REPT; }
\.?"macro"      { return D_MACRO; }
\.?"byte"       { return D_BYTE; }
\.?"cseg"       { return D_CSEG; }
\.?"db"         { return D_DB; }
\.?"def"        { return D_DEF; }
\.?"device"     { return D_DEVICE; }
\.?"dseg"       { return D_DSEG; }
\.?"dw"         { return D_DW; }
\.?"equ"        { return D_EQU; }
\.?"eseg"       { return D_ESEG; }
\.?"list"       { return D_LIST; }
\.?"listmac"    { return D_LISTMAC; }
\.?"nolist"     { return D_NOLIST; }
\."set"         { return D_SET; }
\.?"exit"       {
                    // Close the current file
                    AsmAvr8Lexer_pop_buffer_state(yyscanner);
                    if ( YY_CURRENT_BUFFER )
                    {
                        yyextra->popFileName();
                        *yylloc = yyextra->m_yyllocStack.back();
                        yyextra->m_yyllocStack.pop_back();
                    }
                    else
                    {
                        AsmAvr8Lexer_set_in(nullptr, yyscanner);
                    }

                    return D_EXIT;
                }

"add"           { return I_ADD; }
"adc"           { return I_ADC; }
"adiw"          { return I_ADIW; }
"sub"           { return I_SUB; }
"subi"          { return I_SUBI; }
"sbc"           { return I_SBC; }
"sbci"          { return I_SBCI; }
"sbiw"          { return I_SBIW; }
"and"           { return I_AND; }
"andi"          { return I_ANDI; }
"or"            { return I_OR; }
"ori"           { return I_ORI; }
"eor"           { return I_EOR; }
"com"           { return I_COM; }
"neg"           { return I_NEG; }
"sbr"           { return I_SBR; }
"cbr"           { return I_CBR; }
"inc"           { return I_INC; }
"dec"           { return I_DEC; }
"tst"           { return I_TST; }
"clr"           { return I_CLR; }
"ser"           { return I_SER; }
"mul"           { return I_MUL; }
"rjmp"          { return I_RJMP; }
"ijmp"          { return I_IJMP; }
"jmp"           { return I_JMP; }
"rcall"         { return I_RCALL; }
"icall"         { return I_ICALL; }
"call"          { return I_CALL; }
"ret"           { return I_RET; }
"reti"          { return I_RETI; }
"cpse"          { return I_CPSE; }
"cp"            { return I_CP; }
"cpc"           { return I_CPC; }
"cpi"           { return I_CPI; }
"sbrc"          { return I_SBRC; }
"sbrs"          { return I_SBRS; }
"sbic"          { return I_SBIC; }
"sbis"          { return I_SBIS; }
"brbs"          { return I_BRBS; }
"brbc"          { return I_BRBC; }
"breq"          { return I_BREQ; }
"brne"          { return I_BRNE; }
"brcs"          { return I_BRCS; }
"brcc"          { return I_BRCC; }
"brsh"          { return I_BRSH; }
"brlo"          { return I_BRLO; }
"brmi"          { return I_BRMI; }
"brpl"          { return I_BRPL; }
"brge"          { return I_BRGE; }
"brlt"          { return I_BRLT; }
"brhs"          { return I_BRHS; }
"brhc"          { return I_BRHC; }
"brts"          { return I_BRTS; }
"brtc"          { return I_BRTC; }
"brvs"          { return I_BRVS; }
"brvc"          { return I_BRVC; }
"brie"          { return I_BRIE; }
"brid"          { return I_BRID; }
"mov"           { return I_MOV; }
"ldi"           { return I_LDI; }
"lds"           { return I_LDS; }
"ld"            { return I_LD; }
"ldd"           { return I_LDD; }
"sts"           { return I_STS; }
"st"            { return I_ST; }
"std"           { return I_STD; }
"lpm"           { return I_LPM; }
"in"            { return I_IN; }
"out"           { return I_OUT; }
"push"          { return I_PUSH; }
"pop"           { return I_POP; }
"lsl"           { return I_LSL; }
"lsr"           { return I_LSR; }
"rol"           { return I_ROL; }
"ror"           { return I_ROR; }
"asr"           { return I_ASR; }
"swap"          { return I_SWAP; }
"bset"          { return I_BSET; }
"bclr"          { return I_BCLR; }
"sbi"           { return I_SBI; }
"cbi"           { return I_CBI; }
"bst"           { return I_BST; }
"bld"           { return I_BLD; }
"sec"           { return I_SEC; }
"clc"           { return I_CLC; }
"sen"           { return I_SEN; }
"cln"           { return I_CLN; }
"sez"           { return I_SEZ; }
"clz"           { return I_CLZ; }
"sei"           { return I_SEI; }
"cli"           { return I_CLI; }
"ses"           { return I_SES; }
"cls"           { return I_CLS; }
"sev"           { return I_SEV; }
"clv"           { return I_CLV; }
"set"           { return I_SET; }
"clt"           { return I_CLT; }
"seh"           { return I_SEH; }
"clh"           { return I_CLH; }
"nop"           { return I_NOP; }
"sleep"         { return I_SLEEP; }
"wdr"           { return I_WDR; }

"x"             { return X; }
"y"             { return Y; }
"z"             { return Z; }

"high"          { return F_HIGH; }
"low"           { return F_LOW; }
"byte2"         { return F_BYTE2; }
"byte3"         { return F_BYTE3; }
"byte4"         { return F_BYTE4; }
"lwrd"          { return F_LWRD; }
"hwrd"          { return F_HWRD; }
"page"          { return F_PAGE; }
"exp2"          { return F_EXP2; }
"log2"          { return F_LOG2; }

"at"            { return AT; }
"@"             { return AT_SIGN; }
"("             { return LP; }
")"             { return RP; }
","             { return COMMA; }
"/"             { return SLASH; }
"+"             { return PLUS; }
"-"             { return MINUS; }
"*"             { return ASTERISK; }
"!"             { return NOT; }
"%"             { return MOD; }
"<<"            { return SHL; }
">>"            { return SHR; }
"&&"            { return AND; }
"||"            { return OR; }
"^^"            { return XOR; }
"&"             { return BITAND; }
"|"             { return BITOR; }
"^"             { return BITXOR; }
"=="            { return EQ; }
"<>"            { return NE; }
"!="            { return NE; }
"<"             { return LT; }
"<="            { return LE; }
">"             { return GT; }
">="            { return GE; }
"="             { return ASSIGN; }
"~"             { return COMPLEMET; }
"+="            { return ADD_ASSIGN; }
"-="            { return SUB_ASSIGN; }
"*="            { return MUL_ASSIGN; }
"/="            { return DIV_ASSIGN; }
"%="            { return MOD_ASSIGN; }
"<<="           { return SHL_ASSIGN; }
">>="           { return SHR_ASSIGN; }
"&="            { return AND_ASSIGN; }
"|="            { return ORB_ASSIGN; }
"^="            { return XOR_ASSIGN; }
"++"            { return INCREMENT; }
"--"            { return DECREMENT; }

\.?"include"    {
                    // Handle `include' directive
                    yy_push_state(INC_END, yyscanner);
                    yy_push_state(INC, yyscanner);
                    yylval->array.size = -1;
                }
<INC,INC_END>{
    ;{COMMENT}      { return COMMENT; }
    {COMMENT}       { /* ignore commets */ }
    {WSPACE}        { /* eat up white space */ }
}
<INITIAL,INC>\" {
                    // Beginning of a string, i.e. --> "some string"
                    yy_push_state(STR, yyscanner);
                    yyextra->m_strMaxSize = 32;
                    yylval->array.size = 0;
                    yylval->array.data = (unsigned char*) malloc(yyextra->m_strMaxSize);
                }
'               {
                    // Beginning of a string, or a character literal; i.e. --> 'a' (char.), or --> 'some string' (str.)
                    yy_push_state(QUOTE, yyscanner);
                    yyextra->m_strMaxSize = 8;
                    yylval->array.size = 0;
                    yylval->array.data = (unsigned char*) malloc(yyextra->m_strMaxSize);
                }
<STR>{
    \"              {
                        // End of the string, i.e. "my string" <--
                        yy_pop_state(yyscanner);
                        switch ( YY_START )
                        {
                            case INITIAL:
                                return STRING;
                            case INC:
                                yy_pop_state(yyscanner);
                                break;
                            default:
                                yyextra->lexerMessage ( yylloc,
                                                        CompilerBase::MT_ERROR,
                                                        QObject::tr("syntax not understood").toStdString() );
                                break;
                        }
                    }
    [^\\"\r\n]+     {
                        // Any string content except for escape sequences, EOL (end of line) is also not allowed

                        // Increase the string size
                        yylval->array.size += yyleng;
                        // Enlarge the string buffer, if necessary
                        enlargeStringBuffer(yylval, yyextra);
                        // Copy matched characters into our string buffer
                        memcpy(yylval->array.data + yylval->array.size - yyleng, yytext, yyleng);
                    }
}
<QUOTE>{
    '               {
                        // End of the string, or single character literal; i.e. 'a' <--, or 'some string' <--
                        yy_pop_state(yyscanner);
                        if ( yylval->array.size == 1 )
                        {
                            yylval->number = int(yylval->array.data[0]);
                            return NUMBER;
                        }
                        else
                        {
                            yyextra->lexerMessage ( yylloc,
                                                    CompilerBase::MT_ERROR,
                                                    QObject::tr("character constant is too long").toStdString() );
                        }
                    }
    [^\\'\r\n]+     {
                        // Any string, content except for escape sequences, EOL (end of line) is also not allowed

                        // Increase the string size
                        yylval->array.size += yyleng;
                        // Enlarge the string buffer, if necessary
                        enlargeStringBuffer(yylval, yyextra);
                        // Copy matched characters into our string buffer
                        memcpy(yylval->array.data + yylval->array.size - yyleng, yytext, yyleng);
                    }
}
<STR,QUOTE>{
    {NLINE}         {
                        // A string has to be terminated at the same line which it was started on
                        yy_pop_state(yyscanner);
                        yyextra->lexerMessage ( yylloc,
                                                CompilerBase::MT_ERROR,
                                                QObject::tr("unterminated string or character constant").toStdString());

                        // Put the EOL character sequence back onto the input stream
                        char * eol = strdup(yytext);
                        for ( int i = yyleng - 1; i >= 0; --i )
                        {
                            unput(eol[i]);
                        }
                        free(eol);

                        if ( INITIAL == YY_START )
                        {
                            return STRING;
                        }
                    }
    {XESC}          {
                        // Handle C like escape sequence, such as `\x4A', `\uABCD', `\U1234abcd', and `\177'

                        int size; // Size of the binary value in bytes
                        uint32_t value = escapeSequence(yyextra, yylloc, yytext, &size);

                        // Check whether the conversion was successful
                        if ( 0 == size ) {
                            yyextra->lexerMessage ( yylloc,
                                                    CompilerBase::MT_ERROR,
                                                    QObject::tr ( "unrecognized escape sequence: " )
                                                                . toStdString()
                                                                + "`" + yytext + "'" );
                            yyterminate();
                        }

                        // Copy converted escape sequence to the string buffer
                        enlargeStringBuffer(yylval, yyextra, size);
                        for ( int i = 0; i < size; i++ )
                        {
                            yylval->array.data [ yylval->array.size++ ] = (unsigned char) (value & 0xFF);
                            value >>= 8;
                        }
                    }
    \\.             {
                        // Handle escape sequences inside strings and character literals, and convert the to binary values

                        // Enlarge the string buffer, if necessary
                        enlargeStringBuffer(yylval, yyextra);
                        // Copy converted escape sequence to the string buffer
                        int size;
                        yylval->array.data [ yylval->array.size++ ] = (unsigned char) escapeSequence ( yyextra,
                                                                                                       yylloc,
                                                                                                       yytext,
                                                                                                       &size );
                    }
    <<EOF>>         {
                        // Handle strings terminated by EOF (End Of File) as ERROR, of course.
                        yy_pop_state(yyscanner);
                        yyextra->lexerMessage ( yylloc,
                                                CompilerBase::MT_ERROR,
                                                QObject::tr ( "unterminated string or character constant" )
                                                            . toStdString() );

                        // Discard the string buffer and terminate lexical analysis (this is a fatal error)
                        free(yylval->array.data);
                        yylval->array.size = 0;
                        yyterminate();
                    }
}
<INC,INC_END>{
    {NLINE}         {
                        // Attempt to open the specified file

                        // Put the EOL character sequence back onto the input stream
                        char * eol = strdup(yytext);
                        for ( int i = yyleng - 1; i >= 0; --i )
                        {
                            unput(eol[i]);
                        }
                        free(eol);

                        yy_pop_state(yyscanner);

                        if ( 1 > yylval->array.size )
                        {
                            yyextra->lexerMessage ( yylloc,
                                                    CompilerBase::MT_ERROR,
                                                    QObject::tr("no file name specified").toStdString() );
                            return INCLUDE;
                        }
                        enlargeStringBuffer(yylval, yyextra);
                        yylval->array.data [ yylval->array.size ] = '\0';

                        char * filename = (char*) yylval->array.data;

                        if ( strlen(filename) != (size_t) yylval->array.size )
                        {
                            yyextra->lexerMessage ( yylloc,
                                                    CompilerBase::MT_WARNING,
                                                    QObject::tr("file name contains a null character").toStdString() );
                        }

                        FILE * file = yyextra->fileOpen(filename);
                        if ( nullptr == file )
                        {
                            yyextra->lexerMessage ( yylloc,
                                                    CompilerBase::MT_ERROR,
                                                    QObject::tr ( "unable to open the specified file: " )
                                                                . toStdString()
                                                                + "`" + filename + "'" );
                            return EOL;
                        }
                        else
                        {
                            yylval->string = filename;

                            yyextra->m_yyllocStack.push_back(*yylloc);
                            AsmAvr8Lexer_push_buffer_state ( AsmAvr8Lexer__create_buffer ( file, YY_BUF_SIZE, yyscanner ),
                                                          yyscanner );

                            yylloc->last_line = 1;
                            yylloc->last_column = 1;

                            return INCLUDE;
                        }
                    }
    .               {
                        // Anything else results in an error, it wouldn't make sense otherwise
                        yyextra->lexerMessage ( yylloc,
                                                CompilerBase::MT_ERROR,
                                                QObject::tr ( "unrecognized token: " )
                                                            . toStdString()
                                                            + "`" + std::string(yytext) + "'" );
                        yyterminate();
                    }
}
[01]+b          {
                    // Binary number (max. 64 digits, max. 32 bits)
                    yytext [ yyleng - 1 ] = '\0'; // Remove the trailing `b'
                    yylval->number = convertStrToNumber(yyextra, yylloc, yytext, 64, 2);
                    return NUMBER;
                }
[0-9A-F]+h      {
                    // Hexadecimal number (max. 16 digits, max. 32 bits)
                    yytext [ yyleng - 1 ] = '\0'; // Remove the trailing `h'
                    yylval->number = convertStrToNumber(yyextra, yylloc, yytext, 16, 16);
                    return NUMBER;
                }
[0-7]+[oq]      {
                    // Octal number (max. 21 digits, max. 32 bits)
                    yytext [ yyleng - 1 ] = '\0'; // Remove the trailing `o' or `q'
                    yylval->number = convertStrToNumber(yyextra, yylloc, yytext, 21, 8);
                    return NUMBER;
                }
[0-9]+d         {
                    // Decimal number (max. 10 digits, max. 32 bits)
                    yytext [ yyleng - 1 ] = '\0'; // Remove the trailing `d'
                    yylval->number = convertStrToNumber(yyextra, yylloc, yytext, 10, 10);
                    return NUMBER;
                }
0b[01]+         {
                    // Binary number (max. 64 digits, max. 32 bits)
                    yylval->number = convertStrToNumber(yyextra, yylloc, (yytext + 2), 64, 2);
                    return NUMBER;
                }
0x[0-9A-F]+     {
                    // Hexadecimal number (max. 16 digits, max. 32 bits)
                    yylval->number = convertStrToNumber(yyextra, yylloc, (yytext + 2), 16, 16);
                    return NUMBER;
                }
0[0-7]+         {
                    // Octal number (max. 21 digits, max. 32 bits)
                    yylval->number = convertStrToNumber(yyextra, yylloc, (yytext + 1), 21, 8);
                    return NUMBER;
                }
[0-9]+          {
                    // Decimal number (max. 10 digits, max. 32 bits)
                    yylval->number = convertStrToNumber(yyextra, yylloc, yytext, 10, 10);
                    return NUMBER;
                }
{ID}:           {
                    // Label, e.g. `main:'

                    // Remove the trailing `:'
                    yytext [ yyleng - 1 ] = '\0';

                    // Convert the string to upper case characters
                    for ( int i = 0; i < yyleng; i++ )
                    {
                        yytext[i] = toupper(yytext[i]);
                    }

                    // Return as label
                    yylval->string = yytext;
                    return LABEL;
                }
{ID}|"$"        {
                    // Arbitrary identifier, e.g. `MYREG_U1'

                    // Convert the string to upper case characters
                    for ( int i = 0; i < yyleng; i++ )
                    {
                        yytext[i] = toupper(yytext[i]);
                    }

                    // Return as general identifier
                    yylval->string = yytext;
                    return IDENTIFIER;
                }
<<EOF>>         {
                    // Handle end of file; possibly switch back to previous file, in case "include" was used
                    AsmAvr8Lexer_pop_buffer_state(yyscanner);
                    if ( !YY_CURRENT_BUFFER )
                    {
                        yyterminate();
                    }
                    else
                    {
                        yyextra->popFileName();
                        *yylloc = yyextra->m_yyllocStack.back();
                        yyextra->m_yyllocStack.pop_back();
                    }
                }
.               {
                    // Unrecognized token => ERROR
                    yyextra->lexerMessage ( yylloc,
                                            CompilerBase::MT_ERROR,
                                            QObject::tr ( "unrecognized token: " )
                                                        . toStdString()
                                                        + "`" + std::string(yytext) + "'" );
                }

%%

/* --------------------------------------------------------------------------- */
/* EPILOGUE - USER CODE - FUNCTION DEFINITIONS                                 */
/* --------------------------------------------------------------------------- */

inline void enlargeStringBuffer ( YYSTYPE * semval,
                                  CompilerParserInterface * compiler,
                                  int byMin )
{
    if ( ( byMin + semval->array.size ) >= compiler->m_strMaxSize )
    {
        compiler->m_strMaxSize = 2 * semval->array.size;
        semval->array.data = (unsigned char*) realloc(semval->array.data, compiler->m_strMaxSize);
    }
}
