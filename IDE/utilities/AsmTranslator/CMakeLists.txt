# ==============================================================================
#
# Assembly language support library, this tool translates between various
# dialects of a particular Assembly language,
#
# (C) copyright 2013 Moravia Microsystems, s.r.o.
#
# ==============================================================================


# ------------------------------------------------------------------------------
# GENERAL OPTIONS
# ------------------------------------------------------------------------------

# Set project name
project ( AsmTranslator )

# ------------------------------------------------------------------------------
# COMPILATION OPTIONS
# ------------------------------------------------------------------------------

## Add a library to the project using the specified source files,
 # in this case it will produce dynamic C++ library.
add_library ( ${PROJECT_NAME} SHARED
              AsmTranslator.cxx
              AsmTranslatorBase.cxx
              AsmTranslatorConfig.cxx
              AsmTranslatorKcpsmXil.cxx
              AsmTranslatorKcpsmMed.cxx
              AsmTranslatorKcpsmPBIDE.cxx )

# Link the target to given libraries.
target_link_libraries ( ${PROJECT_NAME} ${QT_LIBRARIES}
                                        ${Boost_LIBRARIES} )

# Main executable.
add_executable ( AsmTransTool main.cxx )
target_link_libraries ( AsmTransTool ${PROJECT_NAME} )

# ------------------------------------------------------------------------------
# INSTALLATION OPTIONS
# ------------------------------------------------------------------------------

install ( TARGETS ${PROJECT_NAME} AsmTransTool
          RUNTIME DESTINATION bin
          LIBRARY DESTINATION lib )
