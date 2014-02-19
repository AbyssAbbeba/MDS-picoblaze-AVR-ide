# ==============================================================================
#
# Simulator engine module for 8-bit PIC.
#
# (C) copyright 2013, 2014 Moravia Microsystems, s.r.o.
#
# ==============================================================================


# ------------------------------------------------------------------------------
# GENERAL OPTIONS
# ------------------------------------------------------------------------------

# Set project name
project ( PIC8Sim )

# ------------------------------------------------------------------------------
# COMPILATION OPTIONS
# ------------------------------------------------------------------------------

# Add include directories to the build.
include_directories ( "${CMAKE_CURRENT_SOURCE_DIR}/../../../utilities" )

## Add a library to the project using the specified source files,
 # in this case it will produce dynamic C++ library.
add_library ( ${PROJECT_NAME}
              PIC8Sim.cxx
              PIC8InstructionSet.cxx
              PIC8Stack.cxx
              PIC8InterruptController.cxx
              PIC8Config.cxx
              PIC8ClockControl.cxx
              PIC8DataMemory.cxx
              PIC8ProgramMemory.cxx
              PIC8DataEEPROM.cxx
              PIC8ConfigWord.cxx
              PIC8WatchDogTimer.cxx
              PIC8IO.cxx
              PIC8Timer0WdtPrescaller.cxx
              PIC8TimerCounter0.cxx
              PIC8ISP.cxx
              PIC8ExternalInterrupts.cxx )

# Link the target to given libraries.
target_link_libraries ( ${PROJECT_NAME} MCUDataFiles )

# ------------------------------------------------------------------------------
# INSTALLATION OPTIONS
# ------------------------------------------------------------------------------

install ( TARGETS ${PROJECT_NAME}
          RUNTIME DESTINATION ${INSTALL_DIR_BIN}
          LIBRARY DESTINATION ${INSTALL_DIR_LIB} )
