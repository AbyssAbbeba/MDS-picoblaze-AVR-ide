# ==============================================================================
#
# Compiler semantic analyzer for assembly language for AVR8.
#
# (C) copyright 2013 Moravia Microsystems, s.r.o.
#
# ==============================================================================


# ------------------------------------------------------------------------------
# GENERAL OPTIONS
# ------------------------------------------------------------------------------

# Set project name
project ( CompilerModuleC )

# Set the minimum required version of CMake
cmake_minimum_required ( VERSION 2.8 )

# Create verbose makefiles, if on
set ( CMAKE_VERBOSE_MAKEFILE OFF )

# ------------------------------------------------------------------------------
# GENERATE LEXICAL AND SYNTAX ANALYZERS
# ------------------------------------------------------------------------------

find_package ( BISON ) # Syntactic analyzer generator
find_package ( FLEX )  # Lexical analyzer generator

# Assembler for 8-bit AVR
bison_target ( CompilerCParser CompilerCParser.y CompilerCParser.cxx VERBOSE "CompilerCParser.output" )
flex_target ( CompilerCLexer CompilerCLexer.l CompilerCLexer.cxx )
# ADD_FLEX_BISON_DEPENDENCY ( CompilerCLexer AsmAvr8Parser )

# List of additional files that will be cleaned as a part of the "make clean" stage.
set ( CLEAN_FILES "CompilerCLexer.h;CompilerCParser.h;CompilerCParser.output" )
set_directory_properties ( PROPERTIES
                           ADDITIONAL_MAKE_CLEAN_FILES "${CLEAN_FILES}" )

# ------------------------------------------------------------------------------
# COMPILATION OPTIONS
# ------------------------------------------------------------------------------

# Find Boost include directories and libraries.
find_package ( Boost 1.46
               REQUIRED
               COMPONENTS regex filesystem system )

# Load settings specific for the Qt4 library (we use it here for i18n)
find_package( Qt4 REQUIRED )
include ( ${QT_USE_FILE} )

# List of preprocessor include file search directories.
include_directories ( ${Boost_INCLUDE_DIRS}
                      "${CMAKE_CURRENT_BINARY_DIR}/../../core/" )

# Specify directories in which the linker will look for libraries.
link_directories ( ${Boost_LIBRARY_DIRS} )

## Add switches to compiler command line
 # -Wall       : Enables all the warnings.
 # -Wextra     : Enables some extra warning flags that are not enabled by -Wall.
 # -std=c++0x  : Default template arguments may not be used in function templates without -std=c++0x or -std=gnu++0x.
 # -O2         : Optimization, level 2.
 # -pipe       : Use pipes rather than temporary files for communication between the various stages of compilation.
 # -fPIC       : Generate position-independent code (PIC) suitable for use in a shared library
add_definitions ( -Wall -Wextra -std=c++0x -O2 -pipe -fPIC )

## Add a library to the project using the specified source files,
 # in this case it will procude static C++ library.
add_library ( ${PROJECT_NAME} STATIC
              ${FLEX_CompilerCLexer_OUTPUTS}
              ${BISON_CompilerCParser_OUTPUTS}
              CompilerCPreprocessor.cxx
              CompilerCSemanticAnalyzer.cxx )

## Set properties on a target.
 # Targets can have properties that affect how they are built.
set_target_properties ( ${PROJECT_NAME} PROPERTIES LINKER_LANGUAGE "CXX" )

# Add a dependency between top-level targets.
# add_dependencies ( ${PROJECT_NAME} )

# ------------------------------------------------------------------------------
# INSTALLATION OPTIONS
# ------------------------------------------------------------------------------

# install ( TARGETS ${PROJECT_NAME}
#           RUNTIME DESTINATION bin
#           LIBRARY DESTINATION lib )
