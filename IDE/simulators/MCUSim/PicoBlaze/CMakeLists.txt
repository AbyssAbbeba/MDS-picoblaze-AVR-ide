# ==============================================================================
#
# Simulator engine module for PicoBlaze soft-processors.
#
# (C) copyright 2013 Moravia Microsystems, s.r.o.
#
# ==============================================================================


# ------------------------------------------------------------------------------
# GENERAL OPTIONS
# ------------------------------------------------------------------------------

# Set project name
project ( PicoBlazeSim )

# Set the minimum required version of CMake
cmake_minimum_required ( VERSION 2.8 )

# Create verbose makefiles if on
set ( CMAKE_VERBOSE_MAKEFILE OFF )

# ------------------------------------------------------------------------------
# COMPILATION OPTIONS
# ------------------------------------------------------------------------------

# Add include directories to the build.
include_directories ( "${CMAKE_CURRENT_BINARY_DIR}/../../../utilities" )

## Add switches to compiler command line
 # -Wall       : Enables all the warnings.
 # -Wextra     : Enables some extra warning flags that are not enabled by -Wall.
 # -std=c++0x  : Default template arguments may not be used in function templates without -std=c++0x or -std=gnu++0x.
 # -O2         : Optimization, level 2.
 # -pipe       : Use pipes rather than temporary files for communication between the various stages of compilation.
add_definitions ( -Wall -Wextra -std=c++0x -O2 -pipe )

## Add a library to the project using the specified source files,
 # in this case it will procude dynamic C++ library.
add_library ( ${PROJECT_NAME} SHARED
              PicoBlazeIO.cxx
              PicoBlazeSim.cxx
              PicoBlazeStack.cxx
              PicoBlazeConfig.cxx
              PicoBlazeRegisters.cxx
              PicoBlazeDataMemory.cxx
              PicoBlazeClockControl.cxx
              PicoBlazeProgramMemory.cxx
              PicoBlazeInstructionSet.cxx
              PicoBlazeInstructionSet2.cxx
              PicoBlazeInstructionSet3.cxx
              PicoBlazeInstructionSet6.cxx
              PicoBlazeInterruptController.cxx )

# Link a target to given libraries.
target_link_libraries ( ${PROJECT_NAME} MCUDataFiles )

# ------------------------------------------------------------------------------
# INSTALLATION OPTIONS
# ------------------------------------------------------------------------------

install ( TARGETS ${PROJECT_NAME}
          RUNTIME DESTINATION bin
          LIBRARY DESTINATION lib )
