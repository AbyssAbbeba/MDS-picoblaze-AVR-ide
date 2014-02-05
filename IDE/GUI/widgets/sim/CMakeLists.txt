
##

# ------------------------------------------------------------------------------
# GENERAL OPTIONS
# ------------------------------------------------------------------------------

# Set project name
project(SimWidgets)

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

#include_directories(
#	"${CMAKE_CURRENT_BINARY_DIR}/../../../simulators/MCUSim/"
#	"${CMAKE_CURRENT_BINARY_DIR}/../../../simulators/MCUSim/AVR8"
#	"${CMAKE_CURRENT_BINARY_DIR}/../../../simulators/MCUSim/McuSimCfgMgr/"
#	"${CMAKE_CURRENT_BINARY_DIR}/../../../simulators/SimControl/"
#	"${CMAKE_CURRENT_BINARY_DIR}/../../../utilities/MCUDataFiles/"
#    "${CMAKE_CURRENT_BINARY_DIR}/../HexEdit/"
#)

# <DEBUG!>
link_directories(
	"${CMAKE_CURRENT_BINARY_DIR}/../../../utilities/MCUDataFiles/"
	"${CMAKE_CURRENT_BINARY_DIR}/../../../simulators/MCUSim/AVR8/"
	"${CMAKE_CURRENT_BINARY_DIR}/../../../simulators/SimControl/"
	"${CMAKE_CURRENT_BINARY_DIR}/../../../simulators/MCUSim/McuSimCfgMgr/"
)
#include_directories(
#	"${CMAKE_CURRENT_BINARY_DIR}/.."
#	"${CMAKE_CURRENT_BINARY_DIR}/../../../simulators/MCUSim/"
#	"${CMAKE_CURRENT_BINARY_DIR}/../../../simulators/MCUSim/AVR8/"
#	"${CMAKE_CURRENT_BINARY_DIR}/../../../simulators/SimControl/"
#	"${CMAKE_CURRENT_BINARY_DIR}/../../../simulators/MCUSim/McuSimCfgMgr/"
#	"${CMAKE_CURRENT_BINARY_DIR}/../../../utilities/MCUDataFiles/"
#	"${CMAKE_CURRENT_BINARY_DIR}/../HexEdit/")
# </DEBUG!>

## Add switches to compiler command line
 # -Wall       : Enables all the warnings.
 # -Wextra     : Enables some extra warning flags that are not enabled by -Wall.
 # -std=c++0x  : Default template arguments may not be used in function templates without -std=c++0x or -std=gnu++0x.
 # -O2         : Optimization, level 2.
 # -pipe       : Use pipes rather than temporary files for communication between the various stages of compilation.
add_definitions(-Wall -Wextra -std=c++0x -O2 -pipe)

qt4_wrap_cpp(QT_MOC_GENERATED_CPP_FILES
#	RegDisplay.h
#	RegDisplayList.h
	McuMemoryView.h

	${CMAKE_CURRENT_BINARY_DIR}/../HexEdit/hexedit.h
	${CMAKE_CURRENT_BINARY_DIR}/../Counters/wlinecounter.h
	${CMAKE_CURRENT_BINARY_DIR}/../Counters/wcolumncounter.h
)

## Add a library to the project using the specified source files,
 # in this case it will procude dynamic C++ library.
add_library(${PROJECT_NAME} SHARED
	${QT_MOC_GENERATED_CPP_FILES}
#	RegDisplay.cxx
#	RegDisplayList.cxx
	McuMemoryView.cxx

	${CMAKE_CURRENT_BINARY_DIR}/../HexEdit/hexedit.cpp
	${CMAKE_CURRENT_BINARY_DIR}/../Counters/wlinecounter.cpp
	${CMAKE_CURRENT_BINARY_DIR}/../Counters/wcolumncounter.cpp)

target_link_libraries(${PROJECT_NAME} ${QT_LIBRARIES} SimControl McuSimCfgMgr PicoBlazeSim MCUDataFiles)
#target_link_libraries(${PROJECT_NAME} ${QT_LIBRARIES} SimControl McuSimCfgMgr AVR8Sim PIC8Sim MCUDataFiles)

# <DEBUG!>
#qt4_wrap_cpp(QT_MOC_GENERATED_CPP_FILES2
#	main.h)
#add_executable(test
#	${QT_MOC_GENERATED_CPP_FILES2}
#	main.cxx)
#target_link_libraries(test ${PROJECT_NAME} ${QT_LIBRARIES} SimControl McuSimCfgMgr AVR8Sim MCUDataFiles)
# </DEBUG!>

# ------------------------------------------------------------------------------
# INSTALLATION OPTIONS
# ------------------------------------------------------------------------------



install( TARGETS ${PROJECT_NAME}
         RUNTIME DESTINATION ${INSTALL_DIR_BIN}
         LIBRARY DESTINATION ${INSTALL_DIR_LIB} )
