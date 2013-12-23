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
project ( DAsmPicoBlaze )

# Set the minimum required version of CMake
cmake_minimum_required ( VERSION 2.8 )

# Create verbose makefiles if on
set ( CMAKE_VERBOSE_MAKEFILE OFF )

# ------------------------------------------------------------------------------
# COMPILATION OPTIONS
# ------------------------------------------------------------------------------

# Load settings specific for the Qt4 library (we use it here for i18n)
find_package ( Qt4 REQUIRED )
include ( ${QT_USE_FILE} )

# Specify directories in which the linker will look for libraries.
link_directories ("${CMAKE_CURRENT_BINARY_DIR}/.." )

## Add switches to compiler command line.
 # -fPIC       : Generate position-independent code (PIC) suitable for use in a shared library
add_definitions ( -fPIC )

## Add a library to the project using the specified source files,
 # in this case it will procude static C++ library.
add_library ( ${PROJECT_NAME} STATIC
              DAsmPicoBlazeCore.cxx
              DAsmPicoBlazeKcpsm6.cxx
              DAsmPicoBlazeKcpsm3.cxx
              DAsmPicoBlazeKcpsm2.cxx
              DAsmPicoBlazeKcpsm1.cxx
              DAsmPicoBlazeKcpsm1CPLD.cxx )

