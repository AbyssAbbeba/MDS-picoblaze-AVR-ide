# ==============================================================================
#
# Moravia Script language support library.
#
# (C) copyright 2013 Moravia Microsystems, s.r.o.
#
# ==============================================================================


# ------------------------------------------------------------------------------
# GENERAL OPTIONS
# ------------------------------------------------------------------------------

# Set project name
project ( moraviascript )

# Set the minimum required version of CMake
cmake_minimum_required ( VERSION 2.8 )

# Create verbose makefiles if on
set ( CMAKE_VERBOSE_MAKEFILE OFF )


# ------------------------------------------------------------------------------
# GENERATE LEXICAL AND SYNTAX ANALYZERS
# ------------------------------------------------------------------------------

find_package ( BISON ) # Syntactic analyzer generator
find_package ( FLEX )  # Lexical analyzer generator

# Assembler for 8-bit AVR
BISON_TARGET ( moraviaScriptParser moraviaScriptParser.y moraviaScriptParser.cxx VERBOSE "moraviaScriptParser.output" )
FLEX_TARGET ( moraviaScriptLexer moraviaScriptLexer.l moraviaScriptLexer.cxx )
# ADD_FLEX_BISON_DEPENDENCY ( moraviaScriptLexer moraviaScriptParser )

# List of additional files that will be cleaned as a part of the "make clean" stage.
set_directory_properties ( PROPERTIES
                           ADDITIONAL_MAKE_CLEAN_FILES "moraviaScriptParser.h;moraviaScriptParser.output;moraviaScriptLexer.h")

# ------------------------------------------------------------------------------
# COMPILATION OPTIONS
# ------------------------------------------------------------------------------

# Load settings specific for the Qt4 library (we use it here for i18n)
find_package( Qt4 REQUIRED )
include ( ${QT_USE_FILE} )

# Specify directories in which the linker will look for libraries.
link_directories ( "${CMAKE_CURRENT_BINARY_DIR}/../utilities/os" )

## Add switches to compiler command line
 # -Wall       : Enables all the warnings.
 # -std=c++0x  : Default template arguments may not be used in function templates without -std=c++0x or -std=gnu++0x.
 # -O2         : Optimization, level 2.
 # -pipe       : Use pipes rather than temporary files for communication between the various stages of compilation.
add_definitions ( -Wall -std=c++0x -O2 -pipe )

## Add a library to the project using the specified source files,
 # in this case it will procude dynamic C++ library.
add_library ( ${PROJECT_NAME} SHARED
              ${FLEX_moraviaScriptLexer_OUTPUTS}
              ${BISON_moraviaScriptParser_OUTPUTS}
              MScriptCore.cxx
              MScriptExpr.cxx
              MScriptStmtTypes.cxx
              MScriptSrcLocation.cxx
              MScriptBase.cxx
              MScriptStatement.cxx
              MScriptLexerAuxFunc.cxx )

## Set properties on a target.
 # Targets can have properties that affect how they are built.
set_target_properties ( ${PROJECT_NAME} PROPERTIES LINKER_LANGUAGE "CXX" )

# Link a target to given libraries.
target_link_libraries ( ${PROJECT_NAME} ${QT_LIBRARIES} os )

# DEBUG: testing executable
add_executable ( main main.cxx )
target_link_libraries ( main ${PROJECT_NAME} )

# ------------------------------------------------------------------------------
# INSTALLATION OPTIONS
# ------------------------------------------------------------------------------

install ( TARGETS ${PROJECT_NAME}
          RUNTIME DESTINATION bin
          LIBRARY DESTINATION lib )
