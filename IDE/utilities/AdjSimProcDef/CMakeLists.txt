# ==============================================================================
#
# Library for working with processor definition files used Adaptable Simulator.
#
# (C) copyright 2013, 2014 Moravia Microsystems, s.r.o.
#
# ==============================================================================


# ------------------------------------------------------------------------------
# GENERAL OPTIONS
# ------------------------------------------------------------------------------

# Set project name
project ( AdjSimProcDef )

# ------------------------------------------------------------------------------
# WINDRES RC FILES.
# Windows only: generate windres RC file(s) to provide information for Windows.
# ------------------------------------------------------------------------------

windres_rc ( OUTPUT_VAR         RC_FILE
             FILETYPE           "DLL"
             FILE_VERSION       "1.0"
             FILEOS             "NT_WINDOWS32"
             FILE_DESCRIPTION   "MDS ASPD"
             COMPANY_NAME       "Moravia Microsystems, s.r.o."
             TARGET             ${PROJECT_NAME}
             PRODUCT_NAME       ${PRODUCT_NAME}
             PRODUCT_VERSION    ${PRODUCT_VERSION}
             YEARS              ${COPYRIGHT_YEARS} )

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
add_library ( ${PROJECT_NAME} ${RC_FILE}
                              ${UI_FILES}
                              ${MOC_FILES}
                              AdjSimProcDef.cxx
                              AdjSimProcDefGui.cxx
                              AdjSimProcDefParser.cxx
                              AdjSimProcDefGenerator.cxx )

# Link a target to given libraries.
target_link_libraries ( ${PROJECT_NAME} ${QT_LIBRARIES} )

# Build processor definition utility.
add_executable ( mds-proc-def-util main.cxx )
target_link_libraries ( mds-proc-def-util ${PROJECT_NAME} ${QT_LIBRARIES} )

# ------------------------------------------------------------------------------
# INSTALLATION OPTIONS
# ------------------------------------------------------------------------------

if ( BUILD_SHARED_LIBS )
    install ( TARGETS ${PROJECT_NAME}
              RUNTIME DESTINATION ${INSTALL_DIR_BIN}
              LIBRARY DESTINATION ${INSTALL_DIR_LIB} )
endif()
