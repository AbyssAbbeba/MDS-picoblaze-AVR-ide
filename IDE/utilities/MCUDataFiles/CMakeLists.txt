
## Simulator engine module for 8-bit AVR

# ------------------------------------------------------------------------------
# GENERAL OPTIONS
# ------------------------------------------------------------------------------

# Set project name
project(MCUDataFiles)

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
 # in this case it will procude dynamic C++ library.
add_library(MCUDataFiles SHARED
	DataFile.cxx
	BinFile.cxx
	HexFile.cxx
	SrecFile.cxx)


# ------------------------------------------------------------------------------
# INSTALLATION OPTIONS
# ------------------------------------------------------------------------------

install(TARGETS MCUDataFiles
	RUNTIME DESTINATION bin
	LIBRARY DESTINATION lib)
