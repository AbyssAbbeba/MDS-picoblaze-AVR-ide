# ==============================================================================
#
# Compiler semantic analyzer for assembly language for AVR8.
#
# (C) copyright 2013, 2014 Moravia Microsystems, s.r.o.
#
# ==============================================================================


# ------------------------------------------------------------------------------
# GENERAL OPTIONS
# ------------------------------------------------------------------------------

# Set project name
project ( CompilerModuleAsmAvr8 )

# ------------------------------------------------------------------------------
# COMPILATION OPTIONS
# ------------------------------------------------------------------------------

# Generate lexical and syntax analyzers.
flex_bison_pair ( AsmAvr8Lexer AsmAvr8Parser AsmAvr8LexerParser )

# List of preprocessor include file search directories.
include_directories ( "${CMAKE_SOURCE_DIR}/compiler/core/" )

# Add a library to the project using the specified source files.
add_library ( ${PROJECT_NAME}
              AsmAvr8SemanticAnalyzer.cxx )

# Link target to given libraries
target_link_libraries ( AsmAvr8LexerParser CompilerCore )

# Link target to given libraries
target_link_libraries ( ${PROJECT_NAME} ${QT_LIBRARIES}
                                        MCUDataFiles
                                        CompilerCore
                                        AsmAvr8LexerParser
                                        CompilerModuleAsmGenerators )

# ------------------------------------------------------------------------------
# INSTALLATION OPTIONS
# ------------------------------------------------------------------------------

if ( BUILD_SHARED_LIBS )
    install ( TARGETS ${PROJECT_NAME}
              RUNTIME DESTINATION ${INSTALL_DIR_BIN}
              LIBRARY DESTINATION ${INSTALL_DIR_LIB} )
endif()
