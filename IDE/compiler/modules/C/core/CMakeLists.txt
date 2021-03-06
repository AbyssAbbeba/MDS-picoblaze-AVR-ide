# ==============================================================================
#
# MDS Compiler module for the C language.
#
# (C) copyright 2014, 2015 Moravia Microsystems, s.r.o.
#
# ==============================================================================


# ------------------------------------------------------------------------------
# GENERAL OPTIONS
# ------------------------------------------------------------------------------

# Set project name
project ( CompilerModuleCCore )

# ------------------------------------------------------------------------------
# COMPILATION OPTIONS
# ------------------------------------------------------------------------------

# Generate lexical and syntax analyzer for the C language.
flex_bison_pair ( CompilerCLexer CompilerCParser CompilerCLexerParser )

# Generate lexical and syntax analyzer for the C preprocessor arithmetic expression calculator.
flex_bison_pair ( CompilerCPreProcCalcLex CompilerCPreProcCalcPar CompilerCPreProcCalc )

# List of preprocessor include file search directories.
include_directories ( "${CMAKE_SOURCE_DIR}"
                      "${CMAKE_SOURCE_DIR}/compiler/core/" )

## Add a library to the project using the specified source files.
add_library ( ${PROJECT_NAME}

              # Parser support
              CompilerCParserExtension.cxx

              # Preprocessor
              CompilerCPreProc.cxx
              CompilerCPreProcMacros.cxx
              CompilerCPreProcSupport.cxx

              # Semantic analyzer.
              CompilerCType.cxx
              CompilerCScope.cxx
              CompilerCTreeDecoder.cxx
              CompilerCDeclaration.cxx
              CompilerCSymbolTable.cxx
              CompilerCExprProcessor.cxx
              CompilerCSemanticAnalyzer.cxx )

# Link a target to given libraries
target_link_libraries ( CompilerCLexerParser CompilerCore )
target_link_libraries ( ${PROJECT_NAME} ${QT_LIBRARIES}
                                        ${Boost_LIBRARIES}
                                        CompilerCore
                                        CompilerCLexerParser
                                        CompilerCPreProcCalc )

# ------------------------------------------------------------------------------
# INSTALLATION OPTIONS
# ------------------------------------------------------------------------------

if ( BUILD_SHARED_LIBS )
    install ( TARGETS ${PROJECT_NAME}
              RUNTIME DESTINATION ${INSTALL_DIR_BIN}
              LIBRARY DESTINATION ${INSTALL_DIR_LIB} )
endif()
