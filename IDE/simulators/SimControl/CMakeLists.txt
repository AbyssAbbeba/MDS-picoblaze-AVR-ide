# ==============================================================================
#
# Simulator control unit.
#
# (C) copyright 2013 Moravia Microsystems, s.r.o.
#
# ==============================================================================

# ------------------------------------------------------------------------------
# GENERAL OPTIONS
# ------------------------------------------------------------------------------

# Set project name
project ( SimControl )

# Set the minimum required version of CMake
cmake_minimum_required ( VERSION 2.8 )

# Create verbose makefiles if on
set ( CMAKE_VERBOSE_MAKEFILE OFF )


# ------------------------------------------------------------------------------
# COMPILATION OPTIONS
# ------------------------------------------------------------------------------

# Load settings specific for the Qt4 library
find_package ( Qt4 REQUIRED )

# Include Qt4's XML module
set ( QT_USE_QTXML 1 )
include ( ${QT_USE_FILE} )

# Specify directories in which the linker will look for libraries.
link_directories ( "${CMAKE_CURRENT_SOURCE_DIR}/../MCUSim/"
                   "${CMAKE_CURRENT_SOURCE_DIR}/../MCUSim/AVR8"
                   "${CMAKE_CURRENT_SOURCE_DIR}/../MCUSim/PIC8"
                   "${CMAKE_CURRENT_SOURCE_DIR}/../MCUSim/PicoBlaze"
                   "${CMAKE_CURRENT_SOURCE_DIR}/../MCUSim/McuSimCfgMgr/"
                   "${CMAKE_CURRENT_SOURCE_DIR}/../../utilities/DbgFile/"
                   "${CMAKE_CURRENT_SOURCE_DIR}/../../utilities/MCUDataFiles/" )

# Add include directories to the build.
include_directories ( "${CMAKE_CURRENT_SOURCE_DIR}/../MCUSim/"
                      "${CMAKE_CURRENT_SOURCE_DIR}/../MCUSim/AVR8"
                      "${CMAKE_CURRENT_SOURCE_DIR}/../MCUSim/PIC8"
                      "${CMAKE_CURRENT_SOURCE_DIR}/../MCUSim/PicoBlaze"
                      "${CMAKE_CURRENT_SOURCE_DIR}/../MCUSim/McuSimCfgMgr/"
                      "${CMAKE_CURRENT_SOURCE_DIR}/../../utilities/DbgFile/"
                      "${CMAKE_CURRENT_SOURCE_DIR}/../../utilities/MCUDataFiles/" )

# Produce moc files for all the listed  .h files.
qt4_wrap_cpp ( QT_MOC_GENERATED_CPP_FILES
               SimControl.h
               MCUSimControl.h )

## Add a library to the project using the specified source files,
 # in this case it will procude dynamic C++ library.
add_library ( ${PROJECT_NAME} SHARED
              ${QT_MOC_GENERATED_CPP_FILES}
              SimControl.cxx
              MCUSimControl.cxx
              MCUSimObserver.cxx )

# Link a target to given libraries.
target_link_libraries ( ${PROJECT_NAME} ${QT_LIBRARIES} MCUDataFiles McuSimCfgMgr AVR8Sim PIC8Sim PicoBlazeSim DbgFile )


# ------------------------------------------------------------------------------
# INSTALLATION OPTIONS
# ------------------------------------------------------------------------------

install ( TARGETS ${PROJECT_NAME}
          RUNTIME DESTINATION bin
          LIBRARY DESTINATION lib )
