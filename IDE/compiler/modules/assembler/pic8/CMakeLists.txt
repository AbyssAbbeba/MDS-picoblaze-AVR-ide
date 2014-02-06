# ==============================================================================
#
# Compiler semantic analyzer for assembly language for PIC8.
#
# (C) copyright 2013, 2014 Moravia Microsystems, s.r.o.
#
# ==============================================================================


# ------------------------------------------------------------------------------
# GENERAL OPTIONS
# ------------------------------------------------------------------------------

# Set project name
project ( CompilerModuleAsmPic8 )

# ------------------------------------------------------------------------------
# COMPILATION OPTIONS
# ------------------------------------------------------------------------------

# Generate lexical and syntax analyzers.
FLEX_BISON_PAIR ( AsmPic8Lexer AsmPic8Parser AsmPic8LexerParser )

# List of preprocessor include file search directories.
include_directories ( "${CMAKE_SOURCE_DIR}/compiler/core" )

## Add a library to the project using the specified source files,
 # in this case it will procude static C++ library.
add_library ( ${PROJECT_NAME}
              AsmPic8SemanticAnalyzer.cxx )

# Link a target to given libraries
target_link_libraries ( AsmPic8LexerParser CompilerCore )

# Link a target to given libraries
target_link_libraries ( ${PROJECT_NAME} ${QT_LIBRARIES}
                                        MCUDataFiles
                                        CompilerCore
                                        AsmPic8LexerParser
                                        CompilerModuleAsmGenerators )

# ------------------------------------------------------------------------------
# INSTALLATION OPTIONS
# ------------------------------------------------------------------------------

if ( BUILD_SHARED_LIBS )
    install ( TARGETS ${PROJECT_NAME}
              RUNTIME DESTINATION ${INSTALL_DIR_BIN}
              LIBRARY DESTINATION ${INSTALL_DIR_LIB} )
endif()
