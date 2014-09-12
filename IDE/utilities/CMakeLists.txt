# ==============================================================================
#
# Utility libraries and tools ...
#
# (C) copyright 2013, 2014 Moravia Microsystems, s.r.o.
#
# ==============================================================================

# ------------------------------------------------------------------------------
# GENERAL OPTIONS
# ------------------------------------------------------------------------------

# Set project name.
project ( utilities )

# ------------------------------------------------------------------------------
# BUILD COMPONENTS IN THE SUBDIRECTORIES
# ------------------------------------------------------------------------------

# Build the tools for working with Intel 8 Hex, Motorola S-Records, etc.
add_subdirectory ( MCUDataFiles )

# Build the tools for working with debug files.
add_subdirectory ( DbgFile )

# Build the tools for working with mcu spec file (xml).
add_subdirectory ( McuSpecFileDesigner )

# OS compatibility.
add_subdirectory ( os )

# Assembly language support library.
if ( MDS_FEATURE_TRANSLATOR )
    add_subdirectory ( AsmTranslator )
endif()

# Library for validating license certificates.
if ( MDS_FEATURE_LICENCE_CERTIFICATE )
    add_subdirectory ( LicenseCertificate )
endif()

# Library for working with processor definition files used Adjustable Simulator.
if ( MDS_FEATURE_ADAPTABLE_SIMULATOR )
    add_subdirectory ( AdjSimProcDef )
endif()

# Library for automated testing environment using libCUnit.
if ( BUILD_TESTING )
    add_subdirectory ( AutoTest )
endif()
