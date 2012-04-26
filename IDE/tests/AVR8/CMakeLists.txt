
## Unit testing for simulator engine module for 8-bit AVR

# ------------------------------------------------------------------------------
# GENERAL OPTIONS
# ------------------------------------------------------------------------------

# Set project name
project(AVR8test)

# Set the minimum required version of CMake
cmake_minimum_required(VERSION 2.8)

# Create verbose makefiles if on
set(CMAKE_VERBOSE_MAKEFILE OFF)

# # After this command, you can add tests in the project using ADD_TEST command.
enable_testing()


# ------------------------------------------------------------------------------
# COMPILATION OPTIONS
# ------------------------------------------------------------------------------

# A local variable which is used used later
set(MY_IDE_ROOT_DIR "${CMAKE_CURRENT_BINARY_DIR}/../..")

# Specify directories in which the linker will look for libraries.
link_directories(
	"${MY_IDE_ROOT_DIR}/utilities/MCUDataFiles"
	"${MY_IDE_ROOT_DIR}/simulators/MCUSim/AVR8"
	"${MY_IDE_ROOT_DIR}/simulators/MCUSim/McuSimCfgMgr")

# Add include directories to the build.
include_directories(
	"${MY_IDE_ROOT_DIR}/utilities"
	"${MY_IDE_ROOT_DIR}/simulators/MCUSim")

# Include Qt4's XML module
find_package(Qt4 REQUIRED)
set(QT_USE_QTXML 1)
include(${QT_USE_FILE})

## Add switches to compiler command line
 # -pipe       : Use pipes rather than temporary files for communication between the various stages of compilation.
 # -std=c++0x  : Default template arguments may not be used in function templates without -std=c++0x or -std=gnu++0x.
 # -O1         : Optimization, level 1.
add_definitions(-pipe -std=c++0x -O1)

add_executable(${PROJECT_NAME}
	AVR8test.cxx
)

target_link_libraries(${PROJECT_NAME} cunit ${QT_LIBRARIES} McuSimCfgMgr AVR8Sim MCUDataFiles)


# ------------------------------------------------------------------------------
# TESTS
# ------------------------------------------------------------------------------

add_test(NAME ${PROJECT_NAME} COMMAND ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME})
