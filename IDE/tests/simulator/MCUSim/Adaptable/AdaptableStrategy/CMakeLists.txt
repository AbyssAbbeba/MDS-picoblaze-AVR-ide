# ==============================================================================
#
# Test strategy for Adaptable simulator
#
# (C) copyright 2014 Moravia Microsystems, s.r.o.
#
# ==============================================================================


# ------------------------------------------------------------------------------
# GENERAL OPTIONS
# ------------------------------------------------------------------------------

# Set project name
project ( AdaptableStrategy )

# ------------------------------------------------------------------------------
# COMPILATION OPTIONS
# ------------------------------------------------------------------------------

## Add a library to the project using the specified source files,
 # in this case it will produce dynamic C++ library.
add_library ( ${PROJECT_NAME} SHARED
              AdaptableStrategy.cxx )

# Link a target to given libraries
target_link_libraries ( ${PROJECT_NAME} MCUSimTestScript
                                        Simulator )
