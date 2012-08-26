##

# ------------------------------------------------------------------------------
# GENERAL OPTIONS
# ------------------------------------------------------------------------------

# Set project name
project(tests)

# Set the minimum required version of CMake
cmake_minimum_required(VERSION 2.8)

# Create verbose makefiles if on
set(CMAKE_VERBOSE_MAKEFILE OFF)



# ------------------------------------------------------------------------------
# COMPILATION OPTIONS
# ------------------------------------------------------------------------------

# Load settings specific for the Qt4 library
find_package(Qt4 REQUIRED)


link_directories(
	"${CMAKE_CURRENT_BINARY_DIR}/../"
	"${CMAKE_CURRENT_BINARY_DIR}/../../mainform/"
	"${CMAKE_CURRENT_BINARY_DIR}/../../dialogs/")

include_directories(
	"${CMAKE_CURRENT_BINARY_DIR}/MovGridLayout/")

ADD_EXECUTABLE( testGrid ./testgrid.cpp)
TARGET_LINK_LIBRARIES( testGrid ${QT_LIBRARIES} Widgets Mainform Dialogs)


ADD_EXECUTABLE( testSplit ./testsplit.cpp)
TARGET_LINK_LIBRARIES( testSplit ${QT_LIBRARIES} Widgets Mainform Dialogs)
