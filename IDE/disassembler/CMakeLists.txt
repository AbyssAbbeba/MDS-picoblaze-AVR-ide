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

# ------------------------------------------------------------------------------
# BUILD COMPONENTS IN THE SUBDIRECTORIES
# ------------------------------------------------------------------------------

# Build disassembler for PicoBlaze.
add_subdirectory ( PicoBlaze )


# ------------------------------------------------------------------------------
# COMPILATION OPTIONS
# ------------------------------------------------------------------------------

# List of preprocessor include file search directories.
include_directories ( ${Boost_INCLUDE_DIRS}
                      "${CMAKE_CURRENT_SOURCE_DIR}/../utilities/MCUDataFiles/"
                      "${CMAKE_CURRENT_SOURCE_DIR}/PicoBlaze/" )

# Specify directories in which the linker will look for libraries.
link_directories ( ${Boost_LIBRARY_DIRS}
                   "${CMAKE_CURRENT_SOURCE_DIR}/../utilities/MCUDataFiles/"
                   "${CMAKE_CURRENT_SOURCE_DIR}/PicoBlaze/" )

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
