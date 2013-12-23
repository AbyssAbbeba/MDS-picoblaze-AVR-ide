# ==============================================================================
#
# Compiler semantic analyzer for assembly language for MCS-51.
#
# (C) copyright 2013 Moravia Microsystems, s.r.o.
#
# ==============================================================================


# ------------------------------------------------------------------------------
# GENERAL OPTIONS
# ------------------------------------------------------------------------------

# Set project name
project ( CompilerModuleAsmMcs51 )

# Set the minimum required version of CMake
cmake_minimum_required ( VERSION 2.8 )

# Create verbose makefiles if on
set ( CMAKE_VERBOSE_MAKEFILE OFF )

# ------------------------------------------------------------------------------
# COMPILATION OPTIONS
# ------------------------------------------------------------------------------

# Generate lexical and syntax analyzers
include ( "${CMAKE_CURRENT_BINARY_DIR}/../../../../FlexBisonPair.cmake" )
FLEX_BISON_PAIR ( AsmMcs51Lexer AsmMcs51Parser AsmMcs51LexerParser )

# Load settings specific for the Qt4 library (we use it here for i18n)
find_package ( Qt4 REQUIRED )
include ( ${QT_USE_FILE} )

# List of preprocessor include file search directories.
include_directories ( "${CMAKE_CURRENT_BINARY_DIR}/../../../core" )

## Add a library to the project using the specified source files,
 # in this case it will procude static C++ library.
add_library ( ${PROJECT_NAME} SHARED
              AsmMcs51SemanticAnalyzer.cxx )

# Link a target to given libraries
target_link_libraries ( ${PROJECT_NAME} ${QT_LIBRARIES}
                                        MCUDataFiles
                                        AsmMcs51LexerParser
                                        CompilerModuleAsmGenerators )

## Set properties on a target.
 # Targets can have properties that affect how they are built.
set_target_properties ( ${PROJECT_NAME} PROPERTIES LINKER_LANGUAGE "CXX" )

# ------------------------------------------------------------------------------
# INSTALLATION OPTIONS
# ------------------------------------------------------------------------------

install ( TARGETS ${PROJECT_NAME}
          RUNTIME DESTINATION bin
          LIBRARY DESTINATION lib )
