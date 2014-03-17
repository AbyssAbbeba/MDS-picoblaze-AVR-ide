# ==============================================================================
#
# Library for validating license certificates.
#
# (C) copyright 2014 Moravia Microsystems, s.r.o.
#
# ==============================================================================


# ------------------------------------------------------------------------------
# GENERAL OPTIONS
# ------------------------------------------------------------------------------

# Set project name
project ( LicenseCertificates )

# ------------------------------------------------------------------------------
# COMPILATION OPTIONS
# ------------------------------------------------------------------------------

include_directories ( "/usr/include/crypto++" )
link_directories ( "/usr/lib64" )


# Add a library to the project using the specified source files.
add_library ( ${PROJECT_NAME}
              LicenseCertificates.cxx )

# Link a target to given libraries
target_link_libraries ( ${PROJECT_NAME} ${QT_LIBRARIES} )

add_executable ( main main.cxx )
target_link_libraries ( main ${PROJECT_NAME} crypto++ )

# ------------------------------------------------------------------------------
# INSTALLATION OPTIONS
# ------------------------------------------------------------------------------

if ( BUILD_SHARED_LIBS )
    install ( TARGETS ${PROJECT_NAME}
              RUNTIME DESTINATION ${INSTALL_DIR_BIN}
              LIBRARY DESTINATION ${INSTALL_DIR_LIB} )
endif()
