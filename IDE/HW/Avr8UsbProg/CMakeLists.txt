# ==============================================================================
#
# Software for USB programmer for SPI ISP capable 8-bit AVR microcontrollers.
#
# (C) copyright 2013 Moravia Microsystems, s.r.o.
#
# ==============================================================================

# ------------------------------------------------------------------------------
# GENERAL OPTIONS
# ------------------------------------------------------------------------------

# Set project name
project ( Avr8UsbProg )

# Set the minimum required version of CMake
cmake_minimum_required ( VERSION 2.8 )

# Create verbose makefiles if on
set ( CMAKE_VERBOSE_MAKEFILE OFF )

# ------------------------------------------------------------------------------
# COMPILATION OPTIONS
# ------------------------------------------------------------------------------

set ( PLUGIN_LIB "${PROJECT_NAME}Plugin" )
set ( EXECUTABLE "${PROJECT_NAME}"       )

# Load settings specific for the Qt4 library.
find_package ( Qt4 REQUIRED )
set( QT_USE_QTXML  1 )
set( QT_USE_QTHELP 1 )
include ( ${QT_USE_FILE} )

# List of preprocessor include file search directories.
include_directories ( "${CMAKE_CURRENT_SOURCE_DIR}/../../utilities/MCUDataFiles/" )

# Specify directories in which the linker will look for libraries.
link_directories ( "${CMAKE_CURRENT_SOURCE_DIR}/../../utilities/MCUDataFiles/" )

# Create Qt user interfaces Wrappers.
qt4_wrap_ui ( UI_FILES Avr8UsbProgPlugin.ui )

# Take the Qt Resource Collection file(s) and compile it to C++ source file using Qt Resource Collection Compiler.
qt4_add_resources ( QRC_FILES Avr8UsbProg.qrc )

## Take Qt specific C++ header file(s) and compile them using Qt Meta Object Compiler.
 # All C++ classes containing the Qt's "Q_OBJECT" macro must be compiled with moc, otherwise Qt's signal/slot mechanism
 # will not work and compilation will therefore probably fail.
qt4_wrap_cpp ( MOC_FILES Avr8UsbProgCore.h
                         Avr8UsbProgThread.h
                         Avr8UsbProgPlugin.h
                         HWPluginInterface.h )

# Add a library to the project using the specified source files,
add_library ( ${PLUGIN_LIB} SHARED
              ${UI_FILES}
              ${QRC_FILES}
              ${MOC_FILES}
              Avr8UsbProgIsp.cxx
              Avr8UsbProgCore.cxx
              Avr8UsbProgThread.cxx
              Avr8UsbProgPlugin.cxx
              Avr8UsbProgDeviceDB.cxx )

# Add an executable to the project using the specified source files.
add_executable ( ${EXECUTABLE}
                 main.cxx )

# Link targets to given libraries.
target_link_libraries ( ${PLUGIN_LIB} ${QT_LIBRARIES} MCUDataFiles )
target_link_libraries ( ${EXECUTABLE} ${PLUGIN_LIB} )

# ------------------------------------------------------------------------------
# INSTALLATION OPTIONS
# ------------------------------------------------------------------------------

install ( TARGETS ${PROJECT_NAME}
          RUNTIME DESTINATION bin
          LIBRARY DESTINATION lib )