# ==============================================================================
#
# Utility libraries and tools ...
#
# (C) copyright 2013 Moravia Microsystems, s.r.o.
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
add_subdirectory ( AsmTranslator )

# Library for automated testing environment using libCUnit.
add_subdirectory ( AutoTest )
