# ==============================================================================
#
# Simulator control unit.
#
# (C) copyright 2013, 2014 Moravia Microsystems, s.r.o.
#
# ==============================================================================

# ------------------------------------------------------------------------------
# GENERAL OPTIONS
# ------------------------------------------------------------------------------

# Set project name
project ( SimControl )

# ------------------------------------------------------------------------------
# COMPILATION OPTIONS
# ------------------------------------------------------------------------------

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
 # in this case it will produce dynamic C++ library.
add_library ( ${PROJECT_NAME} OBJECT
              ${QT_MOC_GENERATED_CPP_FILES}
              SimControl.cxx
              MCUSimControl.cxx
              MCUSimObserver.cxx )
