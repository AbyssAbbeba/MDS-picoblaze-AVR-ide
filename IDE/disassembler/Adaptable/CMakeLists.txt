# ==============================================================================
#
# Adaptable Disassembler.
#
# (C) copyright 2013, 2014 Moravia Microsystems, s.r.o.
#
# ==============================================================================


# ------------------------------------------------------------------------------
# GENERAL OPTIONS
# ------------------------------------------------------------------------------

# Set project name
project ( DAsmAdaptable )

# ------------------------------------------------------------------------------
# COMPILATION OPTIONS
# ------------------------------------------------------------------------------

# List of preprocessor include file search directories.
include_directories ( "${CMAKE_SOURCE_DIR}/utilities/AdjSimProcDef/"
                      "${CMAKE_SOURCE_DIR}/simulators/MCUSim/AdaptableSim/" )

## Add a library to the project using the specified source files,
 # in this case it will produce static C++ library.
add_library ( ${PROJECT_NAME} OBJECT
              DAsmAdaptable.cxx )
