
## Simulator engine module for 8-bit AVR

# ------------------------------------------------------------------------------
# GENERAL OPTIONS
# ------------------------------------------------------------------------------

# Set project name
project(AVR8)

# Set the minimum required version of CMake
cmake_minimum_required(VERSION 2.8)

# Create verbose makefiles if on
set(CMAKE_VERBOSE_MAKEFILE OFF)


# ------------------------------------------------------------------------------
# COMPILATION OPTIONS
# ------------------------------------------------------------------------------

## Add switches to compiler command line
 # -Wall       : Enables all the warnings.
 # -Wextra     : Enables some extra warning flags that are not enabled by -Wall.
 # -std=c++0x  : Default template arguments may not be used in function templates without -std=c++0x or -std=gnu++0x.
 # -O2         : Optimization, level 2.
 # -pipe       : Use pipes rather than temporary files for communication between the various stages of compilation.
add_definitions(-Wall -Wextra -std=c++0x -O2 -pipe)

## Add a library to the project using the specified source files,
 # in this case it will procude static C++ library.
add_library(AVR8Sim STATIC
	AVR8Sim.cxx
	AVR8Config.cxx
	AVR8DataMemory.cxx
	AVR8ProgramMemory.cxx
	AVR8DES.cxx
	AVR8InstructionSet.cxx
	AVR8FusesAndLocks.cxx
	AVR8InterruptController.cxx
	AVR8TimerCounter0.cxx
	AVR8TimerCounter1.cxx
	AVR8TimerCounter2.cxx
	AVR8IO.cxx
	AVR8ExternalInterrupts.cxx
	AVR8WatchdogTimer.cxx
	AVR8DataEEPROM.cxx
	AVR8BootLoader.cxx
	AVR8SystemControl.cxx
	AVR8ClockControl.cxx
	AVR8Spi.cxx
	AVR8Usart.cxx
	AVR8Twi.cxx
	AVR8Adc.cxx
	AVR8AnalogComparator.cxx
	AVR8Isp.cxx
	AVR8ParalelProg.cxx)
