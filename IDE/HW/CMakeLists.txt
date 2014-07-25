# ==============================================================================
#
# Software for development hardware (this does not include firmware).
#
# (C) copyright 2013, 2014 Moravia Microsystems, s.r.o.
#
# ==============================================================================

# ------------------------------------------------------------------------------
# GENERAL OPTIONS
# ------------------------------------------------------------------------------

# Set project name
project ( HW )

# ------------------------------------------------------------------------------
# BUILD COMPONENTS IN THE SUBDIRECTORIES
# ------------------------------------------------------------------------------

# Software for USB programmer for SPI ISP capable 8-bit AVR microcontrollers.
# if ( NOT TARGET_OS STREQUAL "Windows" )
#   add_subdirectory ( Avr8UsbProg )
# endif()
