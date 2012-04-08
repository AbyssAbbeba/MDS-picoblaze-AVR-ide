
## MCU simulator configuration management library.

# ------------------------------------------------------------------------------
# GENERAL OPTIONS
# ------------------------------------------------------------------------------

# Set project name
project(McuSimCfgMgr)

# Set the minimum required version of CMake
cmake_minimum_required(VERSION 2.8)

# Create verbose makefiles if on
set(CMAKE_VERBOSE_MAKEFILE OFF)


# ------------------------------------------------------------------------------
# COMPILATION OPTIONS
# ------------------------------------------------------------------------------

# Add include directories to the build.
include_directories(
	"${CMAKE_CURRENT_BINARY_DIR}/.."
	"${CMAKE_CURRENT_BINARY_DIR}/../AVR8"
)

# Load settings specific for the Qt4 library
find_package(Qt4 REQUIRED)

# Include Qt4's XML module
set(QT_USE_QTXML 1)
include(${QT_USE_FILE})

## Add switches to compiler command line
 # -Wall       : Enables all the warnings.
 # -Wextra     : Enables some extra warning flags that are not enabled by -Wall.
 # -std=c++0x  : Default template arguments may not be used in function templates without -std=c++0x or -std=gnu++0x.
 # -O2         : Optimization, level 2.
 # -pipe       : Use pipes rather than temporary files for communication between the various stages of compilation.
add_definitions(-Wall -Wextra -std=c++0x -O2 -pipe)

## Add a library to the project using the specified source files,
 # in this case it will procude dynamic C++ library.
add_library(McuSimCfgMgr SHARED
	McuDeviceSpecAVR8.cxx
	McuSimCfgMgr.cxx)

target_link_libraries(McuSimCfgMgr ${QT_LIBRARIES})

# ------------------------------------------------------------------------------
# INSTALLATION OPTIONS
# ------------------------------------------------------------------------------

install(TARGETS McuSimCfgMgr
	RUNTIME DESTINATION bin
	LIBRARY DESTINATION lib)
