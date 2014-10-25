# ==============================================================================
#
# Moravia Script language support library.
#
# (C) copyright 2013, 2014 Moravia Microsystems, s.r.o.
#
# ==============================================================================


# ------------------------------------------------------------------------------
# GENERAL OPTIONS
# ------------------------------------------------------------------------------

# Set project name
project ( disassembler )

# Target name for the disassembler's CLI executable binary.
set ( DISASSEMBLER_EXEC "mds-disasm" )

# ------------------------------------------------------------------------------
# BUILD COMPONENTS IN THE SUBDIRECTORIES
# ------------------------------------------------------------------------------

# Build disassembler for PicoBlaze.
add_subdirectory ( PicoBlaze )

# Adaptable Disassembler.
if ( MDS_FEATURE_ADAPTABLE_SIMULATOR )
    add_subdirectory ( Adaptable )
endif()

# ------------------------------------------------------------------------------
# WINDRES RC FILES.
# Windows only: generate windres RC file(s) to provide information for Windows.
# ------------------------------------------------------------------------------

windres_rc ( OUTPUT_VAR         LIB_RC_FILE
             FILETYPE           "DLL"
             FILE_VERSION       "1.0"
             FILEOS             "NT_WINDOWS32"
             FILE_DESCRIPTION   "MDS Disassembler"
             COMPANY_NAME       "Moravia Microsystems, s.r.o."
             TARGET             ${PROJECT_NAME}
             PRODUCT_NAME       ${PRODUCT_NAME}
             PRODUCT_VERSION    ${PRODUCT_VERSION}
             YEARS              ${COPYRIGHT_YEARS} )

windres_rc ( OUTPUT_VAR         EXEC_RC_FILE
             FILETYPE           "APP"
             FILE_VERSION       "1.0"
             FILEOS             "NT_WINDOWS32"
             FILE_DESCRIPTION   "MDS Disassembler"
             ICON               "windres/mds-disasm.ico"
             COMPANY_NAME       "Moravia Microsystems, s.r.o."
             TARGET             ${DISASSEMBLER_EXEC}
             PRODUCT_NAME       ${PRODUCT_NAME}
             PRODUCT_VERSION    ${PRODUCT_VERSION}
             YEARS              ${COPYRIGHT_YEARS} )

# ------------------------------------------------------------------------------
# COMPILATION OPTIONS
# ------------------------------------------------------------------------------

# List of preprocessor include file search directories.
include_directories ( "${CMAKE_SOURCE_DIR}/utilities/MCUDataFiles/"
                      "${CMAKE_CURRENT_SOURCE_DIR}/PicoBlaze/" )

#
## Disassembler as shared library.
#

# List of additional libraries which can be enabled or disabled for this particular build.
set ( MDS_ADDITONAL_LIBS "" )
set ( MDS_ADDITONAL_MODULES "" )

if ( MDS_FEATURE_ADAPTABLE_SIMULATOR )
    list ( APPEND MDS_ADDITONAL_MODULES $<TARGET_OBJECTS:DAsmAdaptable> )
    list ( APPEND MDS_ADDITONAL_LIBS "AdjSimProcDef" )
endif()

# Add a library to the project using the specified source files.
add_library ( ${PROJECT_NAME} SHARED
              ${LIB_RC_FILE}
              ${MDS_ADDITONAL_MODULES}
              $<TARGET_OBJECTS:DAsmPicoBlaze>
              DAsm.cxx
              DAsmCore.cxx )

# Link a target to given libraries
target_link_libraries ( ${PROJECT_NAME} ${QT_LIBRARIES}
                                        ${Boost_LIBRARIES}
                                        ${MDS_ADDITONAL_LIBS}
                                        MCUDataFiles )

#
## Disassembler as executable file.
#

# Add an executable to the project using the specified source files.
add_executable ( ${DISASSEMBLER_EXEC} ${EXEC_RC_FILE}
                                      main.cxx )

# Link a target to given libraries
target_link_libraries ( ${DISASSEMBLER_EXEC} ${PROJECT_NAME} )

# ------------------------------------------------------------------------------
# INSTALLATION OPTIONS
# ------------------------------------------------------------------------------

install ( TARGETS ${PROJECT_NAME}
          RUNTIME DESTINATION ${INSTALL_DIR_BIN}
          LIBRARY DESTINATION ${INSTALL_DIR_LIB} )

if ( MDS_FEATURE_CLI_TOOLS )
    install ( TARGETS ${DISASSEMBLER_EXEC}
              RUNTIME DESTINATION ${INSTALL_DIR_BIN}
              LIBRARY DESTINATION ${INSTALL_DIR_LIB} )
endif()
