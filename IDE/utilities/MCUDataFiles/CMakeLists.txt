# ==============================================================================
# Library for working with Intel Hex files, Motorola® S-Record files, and binary files.
#
# (C) copyright 2013 Moravia Microsystems, s.r.o.
#
# ==============================================================================


# ------------------------------------------------------------------------------
# GENERAL OPTIONS
# ------------------------------------------------------------------------------

# Set project name
project ( MCUDataFiles )

# ------------------------------------------------------------------------------
# COMPILATION OPTIONS
# ------------------------------------------------------------------------------

## Add a library to the project using the specified source files,
 # in this case it will produce dynamic C++ library.
add_library ( ${PROJECT_NAME} SHARED
              DataFile.cxx
              BinFile.cxx
              HexFile.cxx
              SrecFile.cxx
              XilHDLFile.cxx
              XilMemFile.cxx
              XilVHDLFile.cxx
              XilVerilogFile.cxx
              ObjectDataFile.cxx )

# ------------------------------------------------------------------------------
# INSTALLATION OPTIONS
# ------------------------------------------------------------------------------

install ( TARGETS ${PROJECT_NAME}
          RUNTIME DESTINATION bin
          LIBRARY DESTINATION lib )
