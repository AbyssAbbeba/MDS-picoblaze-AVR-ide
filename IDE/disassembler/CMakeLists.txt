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
include_directories ( "${CMAKE_CURRENT_SOURCE_DIR}/../utilities/MCUDataFiles/"
                      "${CMAKE_CURRENT_SOURCE_DIR}/PicoBlaze/" )

#
## Disassembler base as dynamic library.
#

## Add a library to the project using the specified source files,
 # in this case it will produce dynamic C++ library.
add_library ( ${PROJECT_NAME} SHARED
              DAsm.cxx
              DAsmCore.cxx )

#
## Entire disassembler as executable file.
#

# Add an executable to the project using the specified source files.
add_executable ( disasm main.cxx )

# Link a target to given libraries
target_link_libraries ( disasm DAsmPicoBlaze )

# ------------------------------------------------------------------------------
# INSTALLATION OPTIONS
# ------------------------------------------------------------------------------

install ( TARGETS ${PROJECT_NAME} disasm
          RUNTIME DESTINATION bin
          LIBRARY DESTINATION lib )
