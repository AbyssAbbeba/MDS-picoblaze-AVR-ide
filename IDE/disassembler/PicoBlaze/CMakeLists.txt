# ==============================================================================
#
# PicoBlaze Disassembler.
#
# (C) copyright 2013, 2014 Moravia Microsystems, s.r.o.
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

## Add a library to the project using the specified source files,
 # in this case it will produce static C++ library.
add_library ( ${PROJECT_NAME} OBJECT
              DAsmPicoBlazeCore.cxx
              DAsmPicoBlazeKcpsm6.cxx
              DAsmPicoBlazeKcpsm3.cxx
              DAsmPicoBlazeKcpsm2.cxx
              DAsmPicoBlazeKcpsm1.cxx
              DAsmPicoBlazeKcpsm1CPLD.cxx )
