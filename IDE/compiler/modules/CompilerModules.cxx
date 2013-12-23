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
 * @ingroup Compiler
 * @file CompilerModules.cxx
 */
// =============================================================================

#include "CompilerModules.h"

/*
 * Make sure that the data type of semantic values is declared (see Bison manual).
 * Actually the compiler core does not work with this data type, it is used only by
 * the Flex generated lexical analyzer prototypes; in other words, each syntax/lexical
 * analyzer might use it's own data type of semantic values.
 */
#if ! defined ( YYSTYPE ) && ! defined ( YYSTYPE_IS_DECLARED )
    typedef union YYSTYPE {} YYSTYPE;
#endif

// Include all implemented semantic analyzers we have in this compiler collection.
#include "C/CompilerCSemanticAnalyzer.h"
#include "assembler/avr8/AsmAvr8SemanticAnalyzer.h"
#include "assembler/pic8/AsmPic8SemanticAnalyzer.h"
#include "assembler/mcs51/AsmMcs51SemanticAnalyzer.h"
#include "assembler/PicoBlaze/AsmPicoBlazeSemanticAnalyzer.h"

// Include lexer prototypes (they are used by the core to initialize and destroy a lexer).
#include "C/CompilerCLexer.h"
#include "assembler/avr8/AsmAvr8Lexer.h"
#include "assembler/pic8/AsmPic8Lexer.h"
#include "assembler/mcs51/AsmMcs51Lexer.h"
#include "assembler/PicoBlaze/AsmPicoBlazeLexer.h"

// Include additional compiler specific header files.
#include "C/CompilerCPreprocessor.h"

// Parser prototypes (the core uses them to initiate syntactical analysis).
int CompilerCParser_parse    ( yyscan_t yyscanner, CompilerParserInterface * asmCore );
int AsmAvr8Parser_parse      ( yyscan_t yyscanner, CompilerParserInterface * asmCore );
int AsmPic8Parser_parse      ( yyscan_t yyscanner, CompilerParserInterface * asmCore );
int AsmMcs51Parser_parse     ( yyscan_t yyscanner, CompilerParserInterface * asmCore );
int AsmPicoBlazeParser_parse ( yyscan_t yyscanner, CompilerParserInterface * asmCore );

// Compiler header files.
#include "CompilerCore.h"

// Standard header files.
#include <cstdio>
#include <cstdlib>

CompilerModules::ModEmplStatCode CompilerModules::employModule ( CompilerBase::LangId lang,
                                                                 CompilerBase::TargetArch arch,
                                                                 CompilerCore * compilerCore,
                                                                 CompilerSemanticAnalyzer * & semanticAnalyzer )
{
    CompilerOptions * options = compilerCore->getCompilationOptions();
    CompilerParserInterface * parserIntf = dynamic_cast<CompilerParserInterface*>(compilerCore);
    FILE * sourceFile;

    // Pointer to the lexer context.
    yyscan_t yyscanner;

    if ( NULL == parserIntf )
    {
        return MESC_UNKNOWN_ERROR;
    }

    switch ( lang )
    {
        // -------------------------------------------------------------------------------------------------------------
        // C language.
        // -------------------------------------------------------------------------------------------------------------
        case CompilerBase::LI_C:
        {
            switch ( arch )
            {
                /*
                 * AVR-8 microcontroller.
                 */
                case CompilerBase::TA_AVR8:
                {
                    return MESC_ARCH_NOT_SUPPORTED;
                }

                /*
                 * PIC-8 microcontroller.
                 */
                case CompilerBase::TA_PIC8:
                {
                    return MESC_ARCH_NOT_SUPPORTED;
                }
                /*
                 * MCS-51 microcontroller.
                 */
                case CompilerBase::TA_MCS51:
                {
                    return MESC_ARCH_NOT_SUPPORTED;
                }

                /*
                 * PicoBlaze soft-core processor.
                 */
                case CompilerBase::TA_PICOBLAZE:
                {
                    // Setup compiler's semantic analyzer.
                    semanticAnalyzer = new CompilerCSemanticAnalyzer ( compilerCore, options );

                    // Attempt to open the source file.
                    if ( NULL == ( sourceFile = parserIntf->fileOpen ( options->m_sourceFile ) ) )
                    {
                        return MESC_IO_ERROR;
                    }

                    // Initiate C preprocessor.
                    CompilerCPreprocessor preprocessor ( parserIntf, options );
                    char * buffer = preprocessor.processFile ( sourceFile );
                    compilerCore->closeInputFiles();

                    if ( NULL == buffer )
                    {
                        /*
                         * This is clearly an error condition but at this point the error message should have been
                         * already generated somewhere else (by preprocessor preferably), and the compiler module
                         * employement was performed well so far, that is why we return "OK" code. Anyway compilation
                         * stops at this point since neither lexer and parser nor semantical analyzer will be initiated.
                         */
                        return MESC_OK;
                    }

                    // Initiate lexical analyzer.
                    CompilerCLexer_lex_init_extra ( compilerCore, &yyscanner );
                    YY_BUFFER_STATE bufferState = CompilerCLexer__scan_string ( buffer, yyscanner );

                    // Initiate syntax analyzer (syntax analyzer will automatically call semantic analyzer when done).
                    CompilerCParser_parse ( yyscanner, compilerCore );

                    // Clean up.
                    CompilerCLexer__delete_buffer ( bufferState, yyscanner );
                    CompilerCLexer_lex_destroy ( yyscanner );
                    free(buffer);

                    // Done.
                    return MESC_OK;
                }

                /*
                 * Error.
                 */
                default:
                    return MESC_ARCH_NOT_SUPPORTED;
            }
        }

        // -------------------------------------------------------------------------------------------------------------
        // Assembly language.
        // -------------------------------------------------------------------------------------------------------------
        case CompilerBase::LI_ASM:
        {
            switch ( arch )
            {
                /*
                 * AVR-8 macro-assembler.
                 */
                case CompilerBase::TA_AVR8:
                {
                    // Setup compiler's semantic analyzer.
                    semanticAnalyzer = new AsmAvr8SemanticAnalyzer ( compilerCore, options );

                    // Attempt to open the source file.
                    if ( NULL == ( sourceFile = parserIntf->fileOpen ( options->m_sourceFile ) ) )
                    {
                        return MESC_IO_ERROR;
                    }

                    // Initiate language analyzers.
                    AsmAvr8Lexer_lex_init_extra ( compilerCore, &yyscanner );
                    AsmAvr8Lexer_set_in ( sourceFile, yyscanner );
                    AsmAvr8Parser_parse ( yyscanner, compilerCore );
                    AsmAvr8Lexer_lex_destroy ( yyscanner );

                    // Done.
                    return MESC_OK;
                }

                /*
                 * PIC-8 macro-assembler.
                 */
                case CompilerBase::TA_PIC8:
                {
                    // Setup compiler's semantic analyzer.
                    semanticAnalyzer = new AsmPic8SemanticAnalyzer ( compilerCore, options );

                    // Attempt to open the source file.
                    if ( NULL == ( sourceFile = parserIntf->fileOpen ( options->m_sourceFile ) ) )
                    {
                        return MESC_IO_ERROR;
                    }

                    // Initiate language analyzers.
                    AsmPic8Lexer_lex_init_extra ( compilerCore, &yyscanner );
                    AsmPic8Lexer_set_in ( sourceFile, yyscanner );
                    AsmPic8Parser_parse ( yyscanner, compilerCore );
                    AsmPic8Lexer_lex_destroy ( yyscanner );

                    // Done...
                    return MESC_OK;
                }

                /*
                 * MCS-51 macro-assembler.
                 */
                case CompilerBase::TA_MCS51:
                {
                    // Setup compiler's semantic analyzer.
                    semanticAnalyzer = new AsmMcs51SemanticAnalyzer ( compilerCore, options );

                    // Attempt to open the source file.
                    if ( NULL == ( sourceFile = parserIntf->fileOpen ( options->m_sourceFile ) ) )
                    {
                        return MESC_IO_ERROR;
                    }

                    // Initiate language analyzers.
                    AsmMcs51Lexer_lex_init_extra ( compilerCore, &yyscanner );
                    AsmMcs51Lexer_set_in ( sourceFile, yyscanner );
                    AsmMcs51Parser_parse ( yyscanner, compilerCore );
                    AsmMcs51Lexer_lex_destroy ( yyscanner );

                    // Done.
                    return MESC_OK;
                }

                /*
                 * PicoBlaze macro-assembler.
                 */
                case CompilerBase::TA_PICOBLAZE:
                {
                    // Setup compiler's semantic analyzer.
                    semanticAnalyzer = new AsmPicoBlazeSemanticAnalyzer ( compilerCore, options );

                    // Attempt to open the source file.
                    if ( NULL == ( sourceFile = parserIntf->fileOpen ( options->m_sourceFile ) ) )
                    {
                        return MESC_IO_ERROR;
                    }

                    // Initiate language analyzers.
                    AsmPicoBlazeLexer_lex_init_extra ( compilerCore, &yyscanner );
                    AsmPicoBlazeLexer_set_in ( sourceFile, yyscanner );
                    AsmPicoBlazeParser_parse ( yyscanner, compilerCore );
                    AsmPicoBlazeLexer_lex_destroy ( yyscanner );

                    // Done.
                    return MESC_OK;
                }

                /*
                 * Error.
                 */
                default:
                    return MESC_ARCH_NOT_SUPPORTED;
            }
            break;
        }

        // -------------------------------------------------------------------------------------------------------------
        // Error.
        // -------------------------------------------------------------------------------------------------------------
        default:
            return MESC_LANG_NOT_SUPPORTED;
    }

    return MESC_UNKNOWN_ERROR;
}
