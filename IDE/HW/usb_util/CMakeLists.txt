
# ------------------------------------------------------------------------------
# GENERAL OPTIONS
# ------------------------------------------------------------------------------

# Set project name
project(usb_util)

# Set the minimum required version of CMake
cmake_minimum_required(VERSION 2.8)

# Create verbose makefiles if on
set(CMAKE_VERBOSE_MAKEFILE OFF)


# ------------------------------------------------------------------------------
# COMPILATION OPTIONS
# ------------------------------------------------------------------------------

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
	main.cxx
)

target_link_libraries(${PROJECT_NAME} ${QT_LIBRARIES} usb-1.0)
