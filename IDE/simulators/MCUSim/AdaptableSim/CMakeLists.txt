# ==============================================================================
#
# Adaptable Simulator engine module.
#
# (C) copyright 2013, 2014 Moravia Microsystems, s.r.o.
#
# ==============================================================================


# ------------------------------------------------------------------------------
# GENERAL OPTIONS
# ------------------------------------------------------------------------------

# Set project name
project ( AdaptableSimSim )

# ------------------------------------------------------------------------------
# COMPILATION OPTIONS
# ------------------------------------------------------------------------------

# Add include directories to the build.
include_directories ( "${CMAKE_CURRENT_SOURCE_DIR}/../../../utilities" )

## Add a library to the project using the specified source files,
 # in this case it will produce dynamic C++ library.
add_library ( ${PROJECT_NAME} OBJECT
              AdaptableSimIO.cxx
              AdaptableSimSim.cxx
              AdaptableSimStack.cxx
              AdaptableSimConfig.cxx
              AdaptableSimRegisters.cxx
              AdaptableSimDataMemory.cxx
              AdaptableSimStatusFlags.cxx
              AdaptableSimClockControl.cxx
              AdaptableSimProgramMemory.cxx
              AdaptableSimInstructionSet.cxx
              AdaptableSimInterruptController.cxx )
