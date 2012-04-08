
## Simulator engine module for 8-bit AVR

# NOTE for Ubuntu:
#   apt-get install python-cxx-dev

# ------------------------------------------------------------------------------
# GENERAL OPTIONS
# ------------------------------------------------------------------------------

# Set project name
project(libAVR8test)

# Set the minimum required version of CMake
cmake_minimum_required(VERSION 2.8)

# Create verbose makefiles if on
set(CMAKE_VERBOSE_MAKEFILE OFF)

# After this command, you can add tests in the project using ADD_TEST command.
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

## Add switches to compiler command line
 # -pipe       : Use pipes rather than temporary files for communication between the various stages of compilation.
 # -std=c++0x  : Default template arguments may not be used in function templates without -std=c++0x or -std=gnu++0x.
 # -O1         : Optimization, level 1.
add_definitions(-pipe -std=c++0x -O1)

# We will need Python libraries here
find_package(PythonLibs REQUIRED)
include_directories(${PYTHON_INCLUDE_PATH})

# We will need Qt4 libraries here
find_package(Qt4 REQUIRED)
set(QT_USE_QTXML 1) # Include Qt4's XML module
include(${QT_USE_FILE})

# Substitute '${...}' in the given .in file with current values of corresponding variables defined in this CMake script
configure_file("${CMAKE_CURRENT_BINARY_DIR}/libAVR8test.i.in" "${CMAKE_CURRENT_BINARY_DIR}/libAVR8test.i")

# Use the SWIG tool (Simplified Wrapper and Interface Generator)
find_package(SWIG 1.3 REQUIRED)
include(${SWIG_USE_FILE})

# Automatically generate a Python module from an .i file and several C++ sources
set(CMAKE_SWIG_FLAGS "")
set_source_files_properties(libAVR8test.i PROPERTIES CPLUSPLUS ON)
set_source_files_properties(libAVR8test.i PROPERTIES SWIG_FLAGS "-includeall")
SWIG_ADD_MODULE(libAVR8test python libAVR8test.i)
SWIG_LINK_LIBRARIES(libAVR8test ${PYTHON_LIBRARIES} ${QT_LIBRARIES} AVR8Sim MCUDataFiles McuSimCfgMgr)

# ------------------------------------------------------------------------------
# TESTS
# ------------------------------------------------------------------------------

# add_test(NAME SimpleTest0 COMMAND ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME} Hello)
# add_test(NAME SimpleTest1 COMMAND ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME} Hello)
# add_test(NAME SimpleTest2 COMMAND ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME} Hello)

