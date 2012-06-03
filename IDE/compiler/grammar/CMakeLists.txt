
# ------------------------------------------------------------------------------
# GENERAL OPTIONS
# ------------------------------------------------------------------------------

# Set project name
project(CompilerParsers)

# Set the minimum required version of CMake
cmake_minimum_required(VERSION 2.8)

# Create verbose makefiles if on
set(CMAKE_VERBOSE_MAKEFILE OFF)


# ------------------------------------------------------------------------------
# GENERATE LEXICAL AND SYNTAX ANALYZERS
# ------------------------------------------------------------------------------

find_package(BISON)       # Syntactic analyzer generator
find_package(FLEX 2.5.35) # Lexical analyzer generator

# For 8-bit AVR
BISON_TARGET(avr8parser avr8parser.y avr8parser.cxx VERBOSE "avr8parser.output")
FLEX_TARGET(avr8lexer avr8lexer.l avr8lexer.cxx)
# ADD_FLEX_BISON_DEPENDENCY(avr8lexer avr8parser)

# For 8-bit PIC
BISON_TARGET(pic8parser pic8parser.y pic8parser.cxx VERBOSE "pic8parser.output")
FLEX_TARGET(pic8lexer pic8lexer.l pic8lexer.cxx)
# ADD_FLEX_BISON_DEPENDENCY(pic8lexer pic8parser)

# For MCS-51
BISON_TARGET(mcs51parser mcs51parser.y mcs51parser.cxx VERBOSE "mcs51parser.output")
FLEX_TARGET(mcs51lexer mcs51lexer.l mcs51lexer.cxx)
# ADD_FLEX_BISON_DEPENDENCY(mcs51lexer mcs51parser)


# ------------------------------------------------------------------------------
# COMPILATION OPTIONS
# ------------------------------------------------------------------------------

# List of preprocessor include file search directories.
include_directories(
	"${CMAKE_CURRENT_BINARY_DIR}"
	"${CMAKE_CURRENT_BINARY_DIR}/../core"
)

## Add switches to compiler command line
 # -Wall       : Enables all the warnings.
 # -Wextra     : Enables some extra warning flags that are not enabled by -Wall.
 # -std=c++0x  : Default template arguments may not be used in function templates without -std=c++0x or -std=gnu++0x.
 # -O2         : Optimization, level 2.
 # -pipe       : Use pipes rather than temporary files for communication between the various stages of compilation.
add_definitions(-Wall -Wextra -std=c++0x -O2 -pipe)

## Add a library to the project using the specified source files,
 # in this case it will procude dynamic C++ library.
add_library(${PROJECT_NAME} SHARED
	# For 8-bit AVR
	${FLEX_avr8lexer_OUTPUTS}
	${BISON_avr8parser_OUTPUTS}
	# For 8-bit PIC
	${FLEX_pic8lexer_OUTPUTS}
	${BISON_pic8parser_OUTPUTS}
	# For MCS-51
	${FLEX_mcs51lexer_OUTPUTS}
	${BISON_mcs51parser_OUTPUTS}
)

## Set properties on a target.
 # Targets can have properties that affect how they are built.
set_target_properties(${PROJECT_NAME} PROPERTIES LINKER_LANGUAGE "CXX")


# ------------------------------------------------------------------------------
# INSTALLATION OPTIONS
# ------------------------------------------------------------------------------

install(TARGETS ${PROJECT_NAME}
	RUNTIME DESTINATION bin
	LIBRARY DESTINATION lib)
