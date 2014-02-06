# ==============================================================================
#
# Compiler modules for the supported assembly languages.
#
# (C) copyright 2013, 2014 Moravia Microsystems, s.r.o.
#
# ==============================================================================


# ------------------------------------------------------------------------------
# GENERAL OPTIONS
# ------------------------------------------------------------------------------

# Set project name
project ( CompilerModulesAsm )

# ------------------------------------------------------------------------------
# BUILD COMPONENTS IN THE SUBDIRECTORIES
# ------------------------------------------------------------------------------

# Compiler modules for assembly language for AVR8.
add_subdirectory ( avr8 )

# Compiler modules for assembly language for MCS-51.
add_subdirectory ( mcs51 )

# Compiler modules for assembly language for PIC8.
add_subdirectory ( pic8 )

# Compiler modules for assembly language for PicoBlaze.
add_subdirectory ( PicoBlaze )

# Code generators.
add_subdirectory ( generators )

# ------------------------------------------------------------------------------
# COMPILATION OPTIONS
# ------------------------------------------------------------------------------

# List of preprocessor include file search directories.
include_directories ( "${CMAKE_SOURCE_DIR}/compiler/core/"
                      "${CMAKE_SOURCE_DIR}/utilities/DbgFile/"
                      "${CMAKE_SOURCE_DIR}/utilities/MCUDataFiles/" )

# Add library to the project using the specified source files.
add_library ( ${PROJECT_NAME} CompilerModulesAsm.cxx )

# Link target to given libraries.
target_link_libraries ( ${PROJECT_NAME} ${QT_LIBRARIES}
                                        CompilerModuleAsmAvr8
                                        CompilerModuleAsmMcs51
                                        CompilerModuleAsmPic8
                                        CompilerModuleAsmPicoBlaze )

# ------------------------------------------------------------------------------
# INSTALLATION OPTIONS
# ------------------------------------------------------------------------------

if ( BUILD_SHARED_LIBS )
    install ( TARGETS ${PROJECT_NAME}
              RUNTIME DESTINATION ${INSTALL_DIR_BIN}
              LIBRARY DESTINATION ${INSTALL_DIR_LIB} )
endif()
