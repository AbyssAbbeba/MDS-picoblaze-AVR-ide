##

# ------------------------------------------------------------------------------
# GENERAL OPTIONS
# ------------------------------------------------------------------------------

# Set project name
project(ProjectCfg)

# Set the minimum required version of CMake
cmake_minimum_required(VERSION 2.8)

# Create verbose makefiles if on
set(CMAKE_VERBOSE_MAKEFILE OFF)

# with SET() command you can change variables or define new ones
# here we define SAMPLE_SRCS variable that contains a list of all .cpp files 
# note that we don't need \ at the end of line
SET( SAMPLE_SRCS
        ./projectcfgdlg_core.cpp
        ./projectcfg_filemgr.cpp
        ./projectcfg_general.cpp
        ./projectcfg_compiler.cpp
        ./projectcfg_comppaths.cpp
#        ./cfginterface.cpp
)
  
# another list, this time it includes all header files that should be treated with moc
SET( SAMPLE_MOC_HDRS
        ./projectcfgdlg_core.h
        ./projectcfg_general.h
        ./projectcfg_filemgr.h
        ./projectcfg_comppaths.h
#        ./cfginterface.h
)

# some .ui files
SET( SAMPLE_UIS
        ./projectcfg_compiler.ui
        ./projectcfg_comppaths.ui
        ./projectcfg_filemgr.ui
        ./projectcfg_general.ui
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

target_link_libraries(${PROJECT_NAME} ${QT_LIBRARIES} Project CfgInterface)# SimControl)

if ( BUILD_SHARED_LIBS )
    install ( TARGETS ${PROJECT_NAME}
              RUNTIME DESTINATION ${INSTALL_DIR_BIN}
              LIBRARY DESTINATION ${INSTALL_DIR_LIB} )
endif()