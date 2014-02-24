# ==============================================================================
#
# Simulator engine module for PicoBlaze soft-processors.
#
# (C) copyright 2013, 2014 Moravia Microsystems, s.r.o.
#
# ==============================================================================


# ------------------------------------------------------------------------------
# GENERAL OPTIONS
# ------------------------------------------------------------------------------

# Set project name
project ( PicoBlazeSim )

# ------------------------------------------------------------------------------
# COMPILATION OPTIONS
# ------------------------------------------------------------------------------

# Add include directories to the build.
include_directories ( "${CMAKE_CURRENT_SOURCE_DIR}/../../../utilities" )

## Add a library to the project using the specified source files,
 # in this case it will produce dynamic C++ library.
add_library ( ${PROJECT_NAME} OBJECT
              PicoBlazeIO.cxx
              PicoBlazeSim.cxx
              PicoBlazeStack.cxx
              PicoBlazeConfig.cxx
              PicoBlazeRegisters.cxx
              PicoBlazeDataMemory.cxx
              PicoBlazeStatusFlags.cxx
              PicoBlazeClockControl.cxx
              PicoBlazeProgramMemory.cxx
              PicoBlazeInstructionSet.cxx
              PicoBlazeInstructionSet6.cxx
              PicoBlazeInstructionSet3.cxx
              PicoBlazeInstructionSet2.cxx
              PicoBlazeInstructionSet1.cxx
              PicoBlazeInstructionSet1CPLD.cxx
              PicoBlazeInterruptController.cxx )
