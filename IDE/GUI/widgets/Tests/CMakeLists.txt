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

INCLUDE( ${QT_USE_FILE} )

link_directories(
	"${CMAKE_CURRENT_BINARY_DIR}/../MovGridLayout/"
	"${CMAKE_CURRENT_BINARY_DIR}/../Editor/"
	"${CMAKE_CURRENT_BINARY_DIR}/../HexEdit"
	"${CMAKE_CURRENT_BINARY_DIR}/../../mainform/"
	"${CMAKE_CURRENT_BINARY_DIR}/../../dialogs/")

include_directories(
	"${CMAKE_CURRENT_BINARY_DIR}/../MovGridLayout/")


# and finally this will run moc:
QT4_WRAP_CPP( SAMPLE_MOC_SRCS ${SAMPLE_MOC_HDRS} )

ADD_EXECUTABLE( testGrid ./testgrid.cpp)
TARGET_LINK_LIBRARIES( testGrid ${QT_LIBRARIES} Editor MovGridLayout Mainform Dialogs)


ADD_EXECUTABLE( testSplit ./testsplit.cpp)
TARGET_LINK_LIBRARIES( testSplit ${QT_LIBRARIES} HexEdit Mainform Dialogs)
