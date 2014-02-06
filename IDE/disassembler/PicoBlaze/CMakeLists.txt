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
project ( DAsmPicoBlaze )

# ------------------------------------------------------------------------------
# COMPILATION OPTIONS
# ------------------------------------------------------------------------------

# Specify directories in which the linker will look for libraries.
link_directories ("${CMAKE_CURRENT_SOURCE_DIR}/.." )

## Add a library to the project using the specified source files,
 # in this case it will produce static C++ library.
add_library ( ${PROJECT_NAME}
              DAsmPicoBlazeCore.cxx
              DAsmPicoBlazeKcpsm6.cxx
              DAsmPicoBlazeKcpsm3.cxx
              DAsmPicoBlazeKcpsm2.cxx
              DAsmPicoBlazeKcpsm1.cxx
              DAsmPicoBlazeKcpsm1CPLD.cxx )

# Link a target to given libraries
target_link_libraries ( ${PROJECT_NAME} ${QT_LIBRARIES}
                                        ${Boost_LIBRARIES}
                                        DisassemblerCore
                                        MCUDataFiles )

# ------------------------------------------------------------------------------
# INSTALLATION OPTIONS
# ------------------------------------------------------------------------------

if ( BUILD_SHARED_LIBS )
    install ( TARGETS ${PROJECT_NAME}
              RUNTIME DESTINATION ${INSTALL_DIR_BIN}
              LIBRARY DESTINATION ${INSTALL_DIR_LIB} )
endif()
