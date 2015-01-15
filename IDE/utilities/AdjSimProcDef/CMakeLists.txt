# ==============================================================================
#
# Library for working with processor definition files used by Adaptable Simulator.
#
# (C) copyright 2014, 2015 Moravia Microsystems, s.r.o.
#
# ==============================================================================


# ------------------------------------------------------------------------------
# GENERAL OPTIONS
# ------------------------------------------------------------------------------

# Set project name
project ( AdjSimProcDef )

# ------------------------------------------------------------------------------
# COMPILATION OPTIONS
# ------------------------------------------------------------------------------

# Create Qt user interfaces Wrappers.
QT4_WRAP_UI( UI_FILES AdjSimProcDefGuiWidget.ui )

## Take Qt specific C++ header file(s) and compile them using Qt Meta Object Compiler.
qt4_wrap_cpp ( MOC_FILES AdjSimProcDefGui.h )

# Add include directories to the build.
include_directories ( ${CMAKE_SOURCE_DIR} )

# Add a library to the project using the specified source files.
add_library ( ${PROJECT_NAME} ${UI_FILES}
                              ${MOC_FILES}
                              AdjSimProcDef.cxx
                              AdjSimProcDefGui.cxx
                              AdjSimProcDefParser.cxx
                              AdjSimProcDefGenerator.cxx )

# Link a target to given libraries.
target_link_libraries ( ${PROJECT_NAME} ${QT_LIBRARIES} )

# Build processor definition utility.
set ( PROC_DEF_UTIL_EXEC "mds-proc-def-util" )
add_executable ( ${PROC_DEF_UTIL_EXEC} main.cxx )
target_link_libraries ( ${PROC_DEF_UTIL_EXEC} ${PROJECT_NAME} ${QT_LIBRARIES} )

# ------------------------------------------------------------------------------
# INSTALLATION OPTIONS
# ------------------------------------------------------------------------------

if ( BUILD_SHARED_LIBS )
    install ( TARGETS ${PROJECT_NAME}
              RUNTIME DESTINATION ${INSTALL_DIR_BIN}
              LIBRARY DESTINATION ${INSTALL_DIR_LIB} )
endif()

if ( MDS_FEATURE_CLI_TOOLS )
    install ( TARGETS ${PROC_DEF_UTIL_EXEC}
              RUNTIME DESTINATION ${INSTALL_DIR_BIN}
              LIBRARY DESTINATION ${INSTALL_DIR_LIB} )
endif()
