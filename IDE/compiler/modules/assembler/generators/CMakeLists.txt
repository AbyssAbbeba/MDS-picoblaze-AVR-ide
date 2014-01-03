# ==============================================================================
#
# Code generators used by assembler.
#
# (C) copyright 2013 Moravia Microsystems, s.r.o.
#
# ==============================================================================


# ------------------------------------------------------------------------------
# GENERAL OPTIONS
# ------------------------------------------------------------------------------

# Set project name
project ( CompilerModuleAsmGenerators )

# Set the minimum required version of CMake
cmake_minimum_required ( VERSION 2.8 )

# Create verbose makefiles if on
set ( CMAKE_VERBOSE_MAKEFILE OFF )

# ------------------------------------------------------------------------------
# COMPILATION OPTIONS
# ------------------------------------------------------------------------------

# Load settings specific for the Qt4 library (we use it here for i18n)
find_package ( Qt4 REQUIRED )
include ( ${QT_USE_FILE} )

# List of preprocessor include file search directories.
include_directories ( "${CMAKE_CURRENT_SOURCE_DIR}/../../../core/"
                      "${CMAKE_CURRENT_SOURCE_DIR}/../../../../utilities/MCUDataFiles/"
                      "${CMAKE_CURRENT_SOURCE_DIR}/../../../../utilities/DbgFile/" )

# Specify directories in which the linker will look for libraries.
link_directories ( "${CMAKE_CURRENT_SOURCE_DIR}/../../../../utilities/MCUDataFiles/"
                   "${CMAKE_CURRENT_SOURCE_DIR}/../../../../utilities/DbgFile/" )

## Add a library to the project using the specified source files,
 # in this case it will procude static C++ library.
add_library ( ${PROJECT_NAME} SHARED
              AsmMachineCodeGen.cxx
              AsmDgbFileGen.cxx )

# Link a target to given libraries
target_link_libraries ( ${PROJECT_NAME} ${QT_LIBRARIES}
                                        DbgFile
                                        MCUDataFiles )

# ------------------------------------------------------------------------------
# INSTALLATION OPTIONS
# ------------------------------------------------------------------------------

install ( TARGETS ${PROJECT_NAME}
          RUNTIME DESTINATION bin
          LIBRARY DESTINATION lib )
