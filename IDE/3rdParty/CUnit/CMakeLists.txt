# ==============================================================================
#
# CUnit-2.1-2 modified by Moravia Microsystems.
#
# WARNING:
# This library is not mend to be used outside of the corporate environment of
# Moravia Microsystems, s.r.o., and is licensed under GNU LGPL-2 (as well as the
# original work).
#
# (C) copyright 2014 Moravia Microsystems, s.r.o.
#
# ==============================================================================


# ------------------------------------------------------------------------------
# GENERAL OPTIONS
# ------------------------------------------------------------------------------

# Set project name
project ( CUnit )

# ------------------------------------------------------------------------------
# COMPILATION OPTIONS
# ------------------------------------------------------------------------------

# List of source files to compile.
set ( SOURCE_FILES Util.c Basic.c MyMem.c TestDB.c Console.c CUError.c TestRun.c Automated.c )

# Force C++ compiler, otherwise C compiler would be used.
set_source_files_properties ( ${SOURCE_FILES} PROPERTIES LANGUAGE "CXX" )

## Add a library to the project using the specified source files,
 # in this case it will produce dynamic C++ library.
add_library ( ${PROJECT_NAME} SHARED ${SOURCE_FILES} )

## Disable compiler warnings for this library.
set_target_properties ( ${PROJECT_NAME} PROPERTIES
                                        COMPILE_FLAGS "-w"
                                        LINKER_LANGUAGE "CXX" )
