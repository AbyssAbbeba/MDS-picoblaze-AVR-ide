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
# BUILD COMPONENTS IN THE SUBDIRECTORIES & CONFIGURE FEATURES
# ------------------------------------------------------------------------------

# List of additional compiler modules which can be enabled or disabled for this particular build.
set ( MDS_ADDITONAL_MODULES "" )

# Compiler module for assembly language for AVR8.
if ( MDS_FEATURE_AVR8 )
    add_subdirectory ( avr8 )
    list ( APPEND MDS_ADDITONAL_MODULES "CompilerModuleAsmAvr8" )
endif()

# Compiler module for assembly language for MCS-51.
if ( MDS_FEATURE_MCS51 )
    add_subdirectory ( mcs51 )
    list ( APPEND MDS_ADDITONAL_MODULES "CompilerModuleAsmMcs51" )
endif()

# Compiler module for assembly language for PIC8.
if ( MDS_FEATURE_PIC8 )
    add_subdirectory ( pic8 )
    list ( APPEND MDS_ADDITONAL_MODULES "CompilerModuleAsmPic8" )
endif()

# Compiler module for assembly language for PicoBlaze.
if ( MDS_FEATURE_PICOBLAZE )
    add_subdirectory ( PicoBlaze )
    list ( APPEND MDS_ADDITONAL_MODULES "CompilerModuleAsmPicoBlaze" )
endif()

# Compiler module for Adaptable Assembler.
if ( MDS_FEATURE_PICOBLAZE )
    add_subdirectory ( Adaptable )
    list ( APPEND MDS_ADDITONAL_MODULES "CompilerModuleAsmAdaptable" )
endif()

# Common code used by assemblers.
add_subdirectory ( common )

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
                                        ${MDS_ADDITONAL_MODULES} )

# ------------------------------------------------------------------------------
# INSTALLATION OPTIONS
# ------------------------------------------------------------------------------

if ( BUILD_SHARED_LIBS )
    install ( TARGETS ${PROJECT_NAME}
              RUNTIME DESTINATION ${INSTALL_DIR_BIN}
              LIBRARY DESTINATION ${INSTALL_DIR_LIB} )
endif()
