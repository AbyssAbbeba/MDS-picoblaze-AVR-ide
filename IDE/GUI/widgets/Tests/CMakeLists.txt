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

# and finally this will run moc:
QT4_WRAP_CPP( SAMPLE_MOC_SRCS ${SAMPLE_MOC_HDRS} )

ADD_EXECUTABLE( testGrid ./testgrid.cpp)
TARGET_LINK_LIBRARIES( testGrid ${QT_LIBRARIES} Editor MovGridLayout Mainform Dialogs)


ADD_EXECUTABLE( testSplit ./testsplit.cpp)
TARGET_LINK_LIBRARIES( testSplit ${QT_LIBRARIES} HexEdit Mainform Dialogs)

ADD_EXECUTABLE( testToolConv ./testtool_conv.cpp)
TARGET_LINK_LIBRARIES( testToolConv ${QT_LIBRARIES} ConvertorTool)

ADD_EXECUTABLE( testToolDisplay ./testtool_display.cpp)
TARGET_LINK_LIBRARIES( testToolDisplay ${QT_LIBRARIES} DisplayTool)
