# ==============================================================================
#
# MDS Compiler module for the C language.
#
# (C) copyright 2013, 2014 Moravia Microsystems, s.r.o.
#
# ==============================================================================


# ------------------------------------------------------------------------------
# GENERAL OPTIONS
# ------------------------------------------------------------------------------

# Set project name
project ( CompilerModuleC )

# ------------------------------------------------------------------------------
# COMPILATION OPTIONS
# ------------------------------------------------------------------------------

# Generate lexical and syntax analyzer for the C language.
flex_bison_pair ( CompilerCLexer CompilerCParser CompilerCLexerParser )

# Generate lexical and syntax analyzer for the C preprocessor artithmetic expression calculator.
flex_bison_pair ( CompilerCPreProcCalcLex CompilerCPreProcCalcPar CompilerCPreProcCalc )

# List of preprocessor include file search directories.
include_directories ( "${CMAKE_SOURCE_DIR}"
                      "${CMAKE_SOURCE_DIR}/compiler/core/" )

## Add a library to the project using the specified source files.
add_library ( ${PROJECT_NAME}
              CompilerCPreProc.cxx
              CompilerCPreProcMacros.cxx
              CompilerCPreProcSupport.cxx
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
