##

# ------------------------------------------------------------------------------
# GENERAL OPTIONS
# ------------------------------------------------------------------------------

# Set project name
project(McuSpecFileDesigner)

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

SET( SAMPLE_SRCS
    "./mcuspecfiledesigner.cpp"
    "./ioregswidget.cpp"
    "./main.cpp"
)
  
# another list, this time it includes all header files that should be treated with moc
SET( SAMPLE_MOC_HDRS
    "./mcuspecfiledesigner.h"
    "./ioregswidget.h"
)


# and finally this will run moc:
QT4_WRAP_CPP( SAMPLE_MOC_SRCS ${SAMPLE_MOC_HDRS} )

ADD_EXECUTABLE( ${PROJECT_NAME} ${SAMPLE_MOC_SRCS} ${SAMPLE_SRCS})
TARGET_LINK_LIBRARIES( ${PROJECT_NAME} ${QT_LIBRARIES})
