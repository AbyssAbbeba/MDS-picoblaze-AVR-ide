# ==============================================================================
#
# Simulator engine module for 8-bit AVR.
#
# (C) copyright 2013 Moravia Microsystems, s.r.o.
#
# ==============================================================================


# ------------------------------------------------------------------------------
# GENERAL OPTIONS
# ------------------------------------------------------------------------------

# Set project name
project ( AVR8Sim )

# ------------------------------------------------------------------------------
# COMPILATION OPTIONS
# ------------------------------------------------------------------------------

# Add include directories to the build.
include_directories ( "${CMAKE_CURRENT_SOURCE_DIR}/../../../utilities" )

## Add a library to the project using the specified source files,
 # in this case it will procude dynamic C++ library.
add_library ( ${PROJECT_NAME} SHARED
              AVR8IO.cxx
              AVR8Sim.cxx
              AVR8DES.cxx
              AVR8Spi.cxx
              AVR8Twi.cxx
              AVR8Isp.cxx
              AVR8Adc.cxx
              AVR8Usart.cxx
              AVR8Config.cxx
              AVR8DataMemory.cxx
              AVR8DataEEPROM.cxx
              AVR8BootLoader.cxx
              AVR8ParallelProg.cxx
              AVR8ClockControl.cxx
              AVR8SystemControl.cxx
              AVR8ProgramMemory.cxx
              AVR8FusesAndLocks.cxx
              AVR8TimerCounter0.cxx
              AVR8TimerCounter1.cxx
              AVR8TimerCounter2.cxx
              AVR8WatchdogTimer.cxx
              AVR8InstructionSet.cxx
              AVR8AnalogComparator.cxx
              AVR8ExternalInterrupts.cxx
              AVR8InterruptController.cxx )

# Link the target to given libraries.
target_link_libraries ( ${PROJECT_NAME} MCUDataFiles )


# ------------------------------------------------------------------------------
# INSTALLATION OPTIONS
# ------------------------------------------------------------------------------

install ( TARGETS ${PROJECT_NAME}
          RUNTIME DESTINATION bin
          LIBRARY DESTINATION lib )
