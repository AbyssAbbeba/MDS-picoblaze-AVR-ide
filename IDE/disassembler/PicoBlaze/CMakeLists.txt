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

# ------------------------------------------------------------------------------
# COMPILATION OPTIONS
# ------------------------------------------------------------------------------

# Specify directories in which the linker will look for libraries.
link_directories ("${CMAKE_CURRENT_SOURCE_DIR}/.." )

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

