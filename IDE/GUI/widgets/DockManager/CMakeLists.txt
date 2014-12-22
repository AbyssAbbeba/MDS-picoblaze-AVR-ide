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

# and finally this will run moc:
QT4_WRAP_CPP( SAMPLE_MOC_SRCS ${SAMPLE_MOC_HDRS} )

## Add a library to the project using the specified source files,
 # in this case it will procude dynamic C++ library.
add_library(${PROJECT_NAME}
	${SAMPLE_MOC_SRCS} ${SAMPLE_SRCS})


target_link_libraries(${PROJECT_NAME} ${QT_LIBRARIES} PicoBlazeGrid BreakpointList BookmarkList TabBar Editor CompileInfo WelcomeScr AsmMacroAnalyser ExtAppOutput)

if ( BUILD_SHARED_LIBS )
    install ( TARGETS ${PROJECT_NAME}
              RUNTIME DESTINATION ${INSTALL_DIR_BIN}
              LIBRARY DESTINATION ${INSTALL_DIR_LIB} )
endif()