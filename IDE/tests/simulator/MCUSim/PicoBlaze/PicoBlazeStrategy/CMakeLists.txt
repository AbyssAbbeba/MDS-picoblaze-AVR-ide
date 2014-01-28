# ==============================================================================
#
# Test strategy for PicoBlaze simulator
#
# (C) copyright 2013 Moravia Microsystems, s.r.o.
#
# ==============================================================================


# ------------------------------------------------------------------------------
# GENERAL OPTIONS
# ------------------------------------------------------------------------------

# Set project name
project ( PicoBlazeStrategy )

# ------------------------------------------------------------------------------
# COMPILATION OPTIONS
# ------------------------------------------------------------------------------

## Add a library to the project using the specified source files,
 # in this case it will produce dynamic C++ library.
add_library ( ${PROJECT_NAME} SHARED
              PicoBlazeStrategy.cxx )

# Link a target to given libraries
target_link_libraries ( ${PROJECT_NAME} MCUSimTestScript
                                        PicoBlazeSim )
