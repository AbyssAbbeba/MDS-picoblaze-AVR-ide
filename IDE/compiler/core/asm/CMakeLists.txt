# ==============================================================================
#
# Compiler semantic analyzers for assembly language.
#
# (C) copyright 2013 Moravia Microsystems, s.r.o.
#
# ==============================================================================


# ------------------------------------------------------------------------------
# GENERAL OPTIONS
# ------------------------------------------------------------------------------

# Set project name
project ( AsmSemanticAnalyzers )

# Set the minimum required version of CMake
cmake_minimum_required ( VERSION 2.8 )

# Create verbose makefiles if on
set ( CMAKE_VERBOSE_MAKEFILE OFF )


# ------------------------------------------------------------------------------
# BUILD COMPONENTS IN THE SUBDIRECTORIES
# ------------------------------------------------------------------------------

# Compiler semantic analyzer for assembly language for AVR8.
add_subdirectory ( avr8 )

# Compiler semantic analyzer for assembly language for MCS-51.
add_subdirectory ( mcs51 )

# Compiler semantic analyzer for assembly language for PIC8.
add_subdirectory ( pic8 )

# Compiler semantic analyzer for assembly language for KCPSM3.
add_subdirectory ( kcpsm3 )


# ------------------------------------------------------------------------------
# COMPILATION OPTIONS
# ------------------------------------------------------------------------------

## Add switches to compiler command line
 # -Wall       : Enables all the warnings.
 # -std=c++0x  : Default template arguments may not be used in function templates without -std=c++0x or -std=gnu++0x.
 # -O2         : Optimization, level 2.
 # -pipe       : Use pipes rather than temporary files for communication between the various stages of compilation.
 # -fPIC       : Generate position-independent code (PIC) suitable for use in a shared library
add_definitions ( -Wall -std=c++0x -O2 -pipe -fPIC )

## Add a library to the project using the specified source files,
 # in this case it will procude dynamic C++ library.
add_library ( ${PROJECT_NAME} STATIC
              AsmMachineCodeGen.cxx )

# Link a target to given libraries
target_link_libraries ( ${PROJECT_NAME}
                        AsmAvr8SemanticAnalyzer
                        AsmMcs51SemanticAnalyzer
                        AsmPic8SemanticAnalyzer
                        AsmKcpsm3SemanticAnalyzer )
