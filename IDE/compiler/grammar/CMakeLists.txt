# ==============================================================================
#
# Compiler lexical and syntax analyzers.
#
# (C) copyright 2013 Moravia Microsystems, s.r.o.
#
# ==============================================================================


# ------------------------------------------------------------------------------
# GENERAL OPTIONS
# ------------------------------------------------------------------------------

# Set project name
project ( CompilerParsers )

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
BISON_TARGET ( avr8parser avr8parser.y avr8parser.cxx VERBOSE "avr8parser.output" )
FLEX_TARGET ( avr8lexer avr8lexer.l avr8lexer.cxx )
# ADD_FLEX_BISON_DEPENDENCY ( avr8lexer avr8parser )

# Assembler for 8-bit PIC
BISON_TARGET ( pic8parser pic8parser.y pic8parser.cxx VERBOSE "pic8parser.output" )
FLEX_TARGET ( pic8lexer pic8lexer.l pic8lexer.cxx)
# ADD_FLEX_BISON_DEPENDENCY ( pic8lexer pic8parser )

# Assembler for MCS-51
BISON_TARGET ( mcs51parser mcs51parser.y mcs51parser.cxx VERBOSE "mcs51parser.output" )
FLEX_TARGET ( mcs51lexer mcs51lexer.l mcs51lexer.cxx)
# ADD_FLEX_BISON_DEPENDENCY ( mcs51lexer mcs51parser )

# Assembler for PicoBlaze
BISON_TARGET ( PicoBlazeParser PicoBlazeParser.y PicoBlazeParser.cxx VERBOSE "PicoBlazeParser.output" )
FLEX_TARGET ( PicoBlazeLexer PicoBlazeLexer.l PicoBlazeLexer.cxx)
# ADD_FLEX_BISON_DEPENDENCY ( PicoBlazeLexer PicoBlazeParser )

# List of additional files that will be cleaned as a part of the "make clean" stage.
set_directory_properties ( PROPERTIES
                           ADDITIONAL_MAKE_CLEAN_FILES "mcs51lexer.h;mcs51parser.h;mcs51parser.output;pic8lexer.h;pic8parser.h;pic8parser.output;avr8lexer.h;avr8parser.h;avr8parser.output;PicoBlazeLexer.h;PicoBlazeParser.h;PicoBlazeParser.output")

# ------------------------------------------------------------------------------
# COMPILATION OPTIONS
# ------------------------------------------------------------------------------

# Load settings specific for the Qt4 library (we use it here for i18n)
find_package( Qt4 REQUIRED )
include ( ${QT_USE_FILE} )

# List of preprocessor include file search directories.
include_directories ( "${CMAKE_CURRENT_BINARY_DIR}"
                      "${CMAKE_CURRENT_BINARY_DIR}/../core" )

## Add switches to compiler command line
 # -Wall       : Enables all the warnings.
 # -std=c++0x  : Default template arguments may not be used in function templates without -std=c++0x or -std=gnu++0x.
 # -O2         : Optimization, level 2.
 # -pipe       : Use pipes rather than temporary files for communication between the various stages of compilation.
 # -fPIC       : Generate position-independent code (PIC) suitable for use in a shared library
add_definitions ( -Wall -std=c++0x -O2 -pipe -fPIC )

## Add a library to the project using the specified source files,
 # in this case it will procude static C++ library.
add_library ( ${PROJECT_NAME} STATIC
              LexerUtils.cxx

              # Assembler for 8-bit AVR
              ${FLEX_avr8lexer_OUTPUTS}
              ${BISON_avr8parser_OUTPUTS}

              # Assembler for 8-bit PIC
              ${FLEX_pic8lexer_OUTPUTS}
              ${BISON_pic8parser_OUTPUTS}

              # Assembler for MCS-51
              ${FLEX_mcs51lexer_OUTPUTS}
              ${BISON_mcs51parser_OUTPUTS}

              # Assembler for PicoBlaze
              ${FLEX_PicoBlazeLexer_OUTPUTS}
              ${BISON_PicoBlazeParser_OUTPUTS} )

## Set properties on a target.
 # Targets can have properties that affect how they are built.
set_target_properties ( ${PROJECT_NAME} PROPERTIES LINKER_LANGUAGE "CXX" )


# # ------------------------------------------------------------------------------
# # INSTALLATION OPTIONS
# # ------------------------------------------------------------------------------
#
# install ( TARGETS ${PROJECT_NAME}
#           RUNTIME DESTINATION bin
#           LIBRARY DESTINATION lib )
