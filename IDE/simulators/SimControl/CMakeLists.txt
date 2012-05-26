
# ------------------------------------------------------------------------------
# GENERAL OPTIONS
# ------------------------------------------------------------------------------

# Set project name
project(SimControl)

# Set the minimum required version of CMake
cmake_minimum_required(VERSION 2.8)

# Create verbose makefiles if on
set(CMAKE_VERBOSE_MAKEFILE OFF)


# ------------------------------------------------------------------------------
# COMPILATION OPTIONS
# ------------------------------------------------------------------------------

# Load settings specific for the Qt4 library
find_package(Qt4 REQUIRED)

# Include Qt4's XML module
set(QT_USE_QTXML 1)
include(${QT_USE_FILE})

link_directories(
	"${CMAKE_CURRENT_BINARY_DIR}/../MCUSim/"
	"${CMAKE_CURRENT_BINARY_DIR}/../MCUSim/AVR8"
	"${CMAKE_CURRENT_BINARY_DIR}/../MCUSim/McuSimCfgMgr/"
	"${CMAKE_CURRENT_BINARY_DIR}/../../utilities/MCUDataFiles/")

include_directories(
	"${CMAKE_CURRENT_BINARY_DIR}/../MCUSim/"
	"${CMAKE_CURRENT_BINARY_DIR}/../MCUSim/AVR8"
	"${CMAKE_CURRENT_BINARY_DIR}/../MCUSim/McuSimCfgMgr/"
	"${CMAKE_CURRENT_BINARY_DIR}/../../utilities/MCUDataFiles/")

## Add switches to compiler command line
 # -Wall       : Enables all the warnings.
 # -Wextra     : Enables some extra warning flags that are not enabled by -Wall.
 # -std=c++0x  : Default template arguments may not be used in function templates without -std=c++0x or -std=gnu++0x.
 # -O2         : Optimization, level 2.
 # -pipe       : Use pipes rather than temporary files for communication between the various stages of compilation.
add_definitions(-Wall -Wextra -std=c++0x -O2 -pipe)

qt4_wrap_cpp(QT_MOC_GENERATED_CPP_FILES
	SimControl.h
	MCUSimControl.h
)

## Add a library to the project using the specified source files,
 # in this case it will procude dynamic C++ library.
add_library(${PROJECT_NAME} SHARED
	${QT_MOC_GENERATED_CPP_FILES}
	SimControl.cxx
	MCUSimControl.cxx
	MCUSimObserver.cxx)

target_link_libraries(${PROJECT_NAME} ${QT_LIBRARIES} MCUDataFiles McuSimCfgMgr AVR8Sim)

# ------------------------------------------------------------------------------
# INSTALLATION OPTIONS
# ------------------------------------------------------------------------------

install(TARGETS ${PROJECT_NAME}
	RUNTIME DESTINATION bin
	LIBRARY DESTINATION lib)
