##

# ------------------------------------------------------------------------------
# GENERAL OPTIONS
# ------------------------------------------------------------------------------

# Set project name
project(InterfaceCfg)

# Set the minimum required version of CMake
cmake_minimum_required(VERSION 2.8)

# Create verbose makefiles if on
set(CMAKE_VERBOSE_MAKEFILE OFF)

# with SET() command you can change variables or define new ones
# here we define SAMPLE_SRCS variable that contains a list of all .cpp files 
# note that we don't need \ at the end of line
SET( SAMPLE_SRCS
        ./interfacecfgdlg_core.cpp
        ./interfacecfg_editfonts.cpp
        ./interfacecfg_editgeneral.cpp
        ./interfacecfg_editsyntax.cpp
        ./interfacecfg_idegeneral.cpp
        ./interfacecfg_simothers.cpp
        ./interfacecfg_simwarnings.cpp
        ./interfacecfg_syntaxasm.cpp
)
  
# another list, this time it includes all header files that should be treated with moc
SET( SAMPLE_MOC_HDRS
        ./interfacecfgdlg_core.h
        ./interfacecfg_editfonts.h
        ./interfacecfg_editgeneral.h
        ./interfacecfg_editsyntax.h
        ./interfacecfg_idegeneral.h
        ./interfacecfg_simothers.h
        ./interfacecfg_simwarnings.h
        ./interfacecfg_syntaxasm.h
)

# some .ui files
SET( SAMPLE_UIS
        ./interfacecfg_editfonts.ui
        ./interfacecfg_editgeneral.ui
        ./interfacecfg_editsyntax.ui
        ./interfacecfg_idegeneral.ui
        ./interfacecfg_simothers.ui
        ./interfacecfg_simwarnings.ui
        ./interfacecfg_syntaxasm.ui
)


# ------------------------------------------------------------------------------
# COMPILATION OPTIONS
# ------------------------------------------------------------------------------

# Load settings specific for the Qt4 library
#find_package(Qt4 REQUIRED)

# Include Qt4's XML module
set(QT_USE_QTXML 1)
include(${QT_USE_FILE})

#include_directories(
#	"${CMAKE_CURRENT_BINARY_DIR}/../../../simulators/MCUSim/")

# <DEBUG!>
#link_directories(
#	"${CMAKE_CURRENT_BINARY_DIR}/../cfginterface/"
#    "${CMAKE_CURRENT_BINARY_DIR}/../../guicfg/"
#)
#include_directories(
#	"${CMAKE_CURRENT_BINARY_DIR}/.."
#	"${CMAKE_CURRENT_BINARY_DIR}/../../../simulators/MCUSim/")
# </DEBUG!>

## Add switches to compiler command line
 # -Wall       : Enables all the warnings.
 # -Wextra     : Enables some extra warning flags that are not enabled by -Wall.
 # -std=c++0x  : Default template arguments may not be used in function templates without -std=c++0x or -std=gnu++0x.
 # -O2         : Optimization, level 2.
 # -pipe       : Use pipes rather than temporary files for communication between the various stages of compilation.
#add_definitions(-Wall -Wextra -std=c++0x -O2 -pipe -D PICOBLAZE)


# and finally this will run moc:
QT4_WRAP_CPP( SAMPLE_MOC_SRCS ${SAMPLE_MOC_HDRS} )

# this will run uic on .ui files:
QT4_WRAP_UI( SAMPLE_UI_HDRS ${SAMPLE_UIS} )

## Add a library to the project using the specified source files,
 # in this case it will procude dynamic C++ library.
add_library(${PROJECT_NAME}
	${SAMPLE_MOC_SRCS} ${SAMPLE_SRCS} ${SAMPLE_UI_HDRS})

target_link_libraries(${PROJECT_NAME} ${QT_LIBRARIES} CfgInterface GuiCfg)

install( TARGETS ${PROJECT_NAME}
         RUNTIME DESTINATION ${INSTALL_DIR_BIN}
         LIBRARY DESTINATION ${INSTALL_DIR_LIB} )