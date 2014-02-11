##

# ------------------------------------------------------------------------------
# GENERAL OPTIONS
# ------------------------------------------------------------------------------

# Set project name
project(ProjectDlg)

# Set the minimum required version of CMake
cmake_minimum_required(VERSION 2.8)

# Create verbose makefiles if on
set(CMAKE_VERBOSE_MAKEFILE OFF)

# with SET() command you can change variables or define new ones
# here we define SAMPLE_SRCS variable that contains a list of all .cpp files 
# note that we don't need \ at the end of line
SET( SAMPLE_SRCS
        ./projectdlg.cpp
        ./projectdlg_general.cpp
        ../projectcfg/projectcfg_memory.cpp
        ../projectcfg/projectcfg_compiler.cpp
        ../projectcfg/projectcfg_comppaths.cpp
        ../projectcfg/projectcfg_filemgr.cpp
)
  
# another list, this time it includes all header files that should be treated with moc
SET( SAMPLE_MOC_HDRS
        ./projectdlg.h
        ./projectdlg_general.h
        ../projectcfg/projectcfg_memory.h
        ../projectcfg/projectcfg_compiler.h
        ../projectcfg/projectcfg_comppaths.h
        ../projectcfg/projectcfg_filemgr.h
)

# some .ui files
SET( SAMPLE_UIS
        ./projectdlg_general.ui
)

# ------------------------------------------------------------------------------
# COMPILATION OPTIONS
# ------------------------------------------------------------------------------

# and finally this will run moc:
QT4_WRAP_CPP( SAMPLE_MOC_SRCS ${SAMPLE_MOC_HDRS} )

# this will run uic on .ui files:
QT4_WRAP_UI( SAMPLE_UI_HDRS ${SAMPLE_UIS} )

## Add a library to the project using the specified source files,
 # in this case it will procude dynamic C++ library.
add_library(${PROJECT_NAME}
	${SAMPLE_MOC_SRCS} ${SAMPLE_SRCS} ${SAMPLE_UI_HDRS})

target_link_libraries(${PROJECT_NAME} ${QT_LIBRARIES} Project)# SimControl)

if ( BUILD_SHARED_LIBS )
    install ( TARGETS ${PROJECT_NAME}
              RUNTIME DESTINATION ${INSTALL_DIR_BIN}
              LIBRARY DESTINATION ${INSTALL_DIR_LIB} )
endif()