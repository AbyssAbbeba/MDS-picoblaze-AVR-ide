
// -----------------------------------------------------------------------------
// PROLOGUE - BISON DECLARATIONS
// -----------------------------------------------------------------------------

// Require version <version> or higher of Bison.
%require "2.4.1"
// Rename the external symbols used in the parser so that they start with prefix instead of ‘yy’.
%name-prefix "avr8parser_"
// Generate a parser header file containing macro definitions, etc.
%defines "avr8parser.h"
// Generate a pure, reentrant parser.
%define api.pure
// Generate the code processing the locations.
%locations
// Specify the programming language for the generated parser.
%language "C"
// Write an extra output file containing verbose descriptions of the parser states.
%verbose

// Symbol semantic values.
%union {
	int val;
	char sym;
};

%token <val> NUM
%token <sym> OPA OPM LP RP STOP
%type  <val> exp term sfactor factor res

%{
	#include <iostream>  // This is required by Bison
	using namespace std; // This is required by Bison as well

	// Assembler interface to the syntax analyzer (parser) and the lexical analyzer (lexer/scaner)
	#include "CompilerParserInterface.h"

	// Declaration of the lexer prototypes and other things required by Bison
	#include "avr8lexer.h"

	// Name of the lexer function required by Bison
	#define avr8parser_lex avr8lexer_lex

	// Declaration of the error reporting function used by Bison
	int avr8parser_error (
		YYLTYPE * yylloc,
		yyscan_t yyscanner,
		CompilerParserInterface * compiler,
		const char * errorInfo );
%}

// Declare an additional yyparse parameters
%parse-param {yyscan_t yyscanner}
%parse-param {CompilerParserInterface * compiler}

// Declare an additional yylex parameters
%lex-param   {yyscan_t yyscanner}


// -----------------------------------------------------------------------------
// GRAMMAR RULES
// -----------------------------------------------------------------------------
%%

run: res run | res    /* forces bison to process many stmts */

res: exp STOP         { cout << $1 << endl; }

exp: exp OPA term     { $$ = ($2 == '+' ? $1 + $3 : $1 - $3); }
| term                { $$ = $1; }

term: term OPM factor { $$ = ($2 == '*' ? $1 * $3 : $1 / $3); }
| sfactor             { $$ = $1; }

sfactor: OPA factor   { $$ = ($1 == '+' ? $2 : -$2); }
| factor              { $$ = $1; }

factor: NUM           { $$ = $1; }
| LP exp RP           { $$ = $2; }


%%
// -----------------------------------------------------------------------------
// EPILOGUE - FUNCTION DEFINITIONS
// -----------------------------------------------------------------------------

// Definition of the error reporting function used by Bison
int avr8parser_error(YYLTYPE * yylloc, yyscan_t, CompilerParserInterface * compiler, const char * errorInfo) {
	compiler->parserMessage(compiler->toSourceLocation(yylloc), CompilerBase::MT_ERROR, errorInfo);
	return 0;
}
