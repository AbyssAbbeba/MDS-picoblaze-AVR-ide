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