# ==============================================================================
#
# Library for validating and reading license certificates.
#
# (C) copyright 2014 Moravia Microsystems, s.r.o.
#
# ==============================================================================


# ------------------------------------------------------------------------------
# GENERAL OPTIONS
# ------------------------------------------------------------------------------

# Set project name
project ( LicenseCertificate )

# ------------------------------------------------------------------------------
# Convert RSA public key from PEM to ASN.1 DER in form of C++ code.
# ------------------------------------------------------------------------------

set_directory_properties ( PROPERTIES
                           ADDITIONAL_MAKE_CLEAN_FILES "LicenseCertificateKey.h" )

if ( "${CMAKE_CURRENT_SOURCE_DIR}/LicenseKey.pem" IS_NEWER_THAN "${CMAKE_CURRENT_SOURCE_DIR}/LicenseCertificateKey.h" )
    add_custom_target ( ${PROJECT_NAME}-key ALL
                        COMMAND bash keygen.sh LicenseKey.pem
                        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
                        COMMENT "Converting RSA public key from PEM to ASN.1 DER in form of C++ code."
                        VERBATIM
                        SOURCES LicenseKey.pem )
else()
    add_custom_target ( ${PROJECT_NAME}-key ALL
                        COMMENT "RSA public key in form of C++ code has already been generated ... skipping." )
endif()

# ------------------------------------------------------------------------------
# COMPILATION OPTIONS
# ------------------------------------------------------------------------------

# Add include directories to the build.
include_directories ( "${CMAKE_SOURCE_DIR}/3rdParty" )

# Add a library to the project using the specified source files.
add_library ( ${PROJECT_NAME}
              LicenseCertificate.cxx )

# Link a target to given libraries
target_link_libraries ( ${PROJECT_NAME} ${QT_LIBRARIES} CryptoPP )

# Add a dependency between top-level targets.
add_dependencies ( ${PROJECT_NAME} ${PROJECT_NAME}-key )

# Build certificate validation utility.
add_executable ( mds-certutil main.cxx )
target_link_libraries ( mds-certutil ${PROJECT_NAME} )

# ------------------------------------------------------------------------------
# INSTALLATION OPTIONS
# ------------------------------------------------------------------------------

if ( BUILD_SHARED_LIBS )
    install ( TARGETS ${PROJECT_NAME}
              RUNTIME DESTINATION ${INSTALL_DIR_BIN}
              LIBRARY DESTINATION ${INSTALL_DIR_LIB} )
endif()
