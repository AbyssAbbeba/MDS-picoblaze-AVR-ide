##

# ------------------------------------------------------------------------------
# GENERAL OPTIONS
# ------------------------------------------------------------------------------

# Set project name
project(WDockManager)

# Set the minimum required version of CMake
cmake_minimum_required(VERSION 2.8)

# Create verbose makefiles if on
set(CMAKE_VERBOSE_MAKEFILE OFF)



# with SET() command you can change variables or define new ones
# here we define SAMPLE_SRCS variable that contains a list of all .cpp files 
# note that we don't need \ at the end of line
SET( SAMPLE_SRCS
       ./wdockmanager.cpp
)
  
# another list, this time it includes all header files that should be treated with moc
SET( SAMPLE_MOC_HDRS
       ./wdockmanager.h
)


# ------------------------------------------------------------------------------
# COMPILATION OPTIONS
# ------------------------------------------------------------------------------

# Load settings specific for the Qt4 library
find_package(Qt4 REQUIRED)


# <DEBUG!>
link_directories(
	#"${CMAKE_CURRENT_BINARY_DIR}/../SimulationInfo/"
    "${CMAKE_CURRENT_BINARY_DIR}/../PicoBlazeGrid/"
	"${CMAKE_CURRENT_BINARY_DIR}/../BookmarkList/"
	"${CMAKE_CURRENT_BINARY_DIR}/../BreakpointList/"
    "${CMAKE_CURRENT_BINARY_DIR}/../TabBar/"
    "${CMAKE_CURRENT_BINARY_DIR}/../Analyser/"
    "${CMAKE_CURRENT_BINARY_DIR}/../AnalyserWidget/"
    "${CMAKE_CURRENT_BINARY_DIR}/../Editor/"
    "${CMAKE_CURRENT_BINARY_DIR}/../CompileInfo/"
	)

include_directories(
    "${CMAKE_SOURCE_DIR}/GUI/widgets/PicoBlazeInfo/"
    "${CMAKE_SOURCE_DIR}/GUI/widgets/sim/"
    "${CMAKE_SOURCE_DIR}/simulators/MCUSim/"
)

# </DEBUG!>

## Add switches to compiler command line
 # -Wall       : Enables all the warnings.
 # -Wextra     : Enables some extra warning flags that are not enabled by -Wall.
 # -std=c++0x  : Default template arguments may not be used in function templates without -std=c++0x or -std=gnu++0x.
 # -O2         : Optimization, level 2.
 # -pipe       : Use pipes rather than temporary files for communication between the various stages of compilation.
add_definitions(-Wall -Wextra -std=c++0x -O2 -pipe)


# and finally this will run moc:
QT4_WRAP_CPP( SAMPLE_MOC_SRCS ${SAMPLE_MOC_HDRS} )

## Add a library to the project using the specified source files,
 # in this case it will procude dynamic C++ library.
add_library(${PROJECT_NAME} SHARED
	${SAMPLE_MOC_SRCS} ${SAMPLE_SRCS})


target_link_libraries(${PROJECT_NAME} ${QT_LIBRARIES} PicoBlazeGrid BreakpointList BookmarkList TabBar Analyser AnalyserWidget Editor CompileInfo)

install( TARGETS ${PROJECT_NAME}
         RUNTIME DESTINATION ${INSTALL_DIR_BIN}
         LIBRARY DESTINATION ${INSTALL_DIR_LIB} )