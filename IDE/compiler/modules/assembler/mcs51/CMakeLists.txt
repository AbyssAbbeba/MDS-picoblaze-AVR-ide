# ==============================================================================
#
# Compiler semantic analyzer for assembly language for MCS-51.
#
# (C) copyright 2013, 2014 Moravia Microsystems, s.r.o.
#
# ==============================================================================


# ------------------------------------------------------------------------------
# GENERAL OPTIONS
# ------------------------------------------------------------------------------

# Set project name
project ( CompilerModuleAsmMcs51 )

# ------------------------------------------------------------------------------
# COMPILATION OPTIONS
# ------------------------------------------------------------------------------

# Generate lexical and syntax analyzers.
FLEX_BISON_PAIR ( AsmMcs51Lexer AsmMcs51Parser AsmMcs51LexerParser )

# List of preprocessor include file search directories.
include_directories ( "${CMAKE_CURRENT_SOURCE_DIR}/../../../core" )

## Add a library to the project using the specified source files,
 # in this case it will procude static C++ library.
add_library ( ${PROJECT_NAME} SHARED
              AsmMcs51SemanticAnalyzer.cxx )

# Link a target to given libraries
target_link_libraries ( AsmMcs51LexerParser CompilerCore )
target_link_libraries ( ${PROJECT_NAME} ${QT_LIBRARIES}
                                        MCUDataFiles
                                        CompilerCore
                                        AsmMcs51LexerParser
                                        CompilerModuleAsmGenerators )

# ------------------------------------------------------------------------------
# INSTALLATION OPTIONS
# ------------------------------------------------------------------------------

install ( TARGETS ${PROJECT_NAME}
          RUNTIME DESTINATION ${INSTALL_DIR_BIN}
          LIBRARY DESTINATION ${INSTALL_DIR_LIB} )
