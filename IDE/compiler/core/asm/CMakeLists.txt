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


# ------------------------------------------------------------------------------
# COMPILATION OPTIONS
# ------------------------------------------------------------------------------

## Add a library to the project using the specified source files,
 # in this case it will procude dynamic C++ library.
add_library ( ${PROJECT_NAME} STATIC )

# Link a target to given libraries
target_link_libraries ( ${PROJECT_NAME}
                        AsmAvr8SemanticAnalyzer
                        AsmMcs51SemanticAnalyzer
                        AsmPic8SemanticAnalyzer )
