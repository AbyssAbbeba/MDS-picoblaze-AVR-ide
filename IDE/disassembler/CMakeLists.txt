# ==============================================================================
#
# Moravia Script language support library.
#
# (C) copyright 2013 Moravia Microsystems, s.r.o.
#
# ==============================================================================


# ------------------------------------------------------------------------------
# GENERAL OPTIONS
# ------------------------------------------------------------------------------

# Set project name
project ( disassembler )

# Set the minimum required version of CMake
cmake_minimum_required ( VERSION 2.8 )

# Create verbose makefiles if on
set ( CMAKE_VERBOSE_MAKEFILE OFF )


# ------------------------------------------------------------------------------
# BUILD COMPONENTS IN THE SUBDIRECTORIES
# ------------------------------------------------------------------------------

# Build disassembler for PicoBlaze.
add_subdirectory ( PicoBlaze )


# ------------------------------------------------------------------------------
# COMPILATION OPTIONS
# ------------------------------------------------------------------------------

# Find Boost include dirs and libraries.
find_package ( Boost 1.48
               REQUIRED
               COMPONENTS filesystem system )

# Load settings specific for the Qt4 library (we use it here for i18n)
find_package ( Qt4 REQUIRED )
include ( ${QT_USE_FILE} )

# List of preprocessor include file search directories.
include_directories ( ${Boost_INCLUDE_DIRS}
                      "${CMAKE_CURRENT_BINARY_DIR}/../utilities/MCUDataFiles/"
                      "${CMAKE_CURRENT_BINARY_DIR}/PicoBlaze/" )

# Specify directories in which the linker will look for libraries.
link_directories ( ${Boost_LIBRARY_DIRS}
                   "${CMAKE_CURRENT_BINARY_DIR}/../utilities/MCUDataFiles/"
                   "${CMAKE_CURRENT_BINARY_DIR}/PicoBlaze/" )

#
## Disassembler as dynamic library.
#

## Add a library to the project using the specified source files,
 # in this case it will procude dynamic C++ library.
add_library ( ${PROJECT_NAME} SHARED
              DAsm.cxx
              DAsmCore.cxx )

# Link a target to given libraries
target_link_libraries ( ${PROJECT_NAME} ${QT_LIBRARIES}
                                        ${Boost_LIBRARIES}
                                        DAsmPicoBlaze )

#
## Disassembler as executable file.
#

# Add an executable to the project using the specified source files.
add_executable ( disasm
                 main.cxx )

# Link a target to given libraries
target_link_libraries ( disasm MCUDataFiles
                               ${PROJECT_NAME} )


# ------------------------------------------------------------------------------
# INSTALLATION OPTIONS
# ------------------------------------------------------------------------------

install ( TARGETS ${PROJECT_NAME}
         RUNTIME DESTINATION bin
         LIBRARY DESTINATION lib )
