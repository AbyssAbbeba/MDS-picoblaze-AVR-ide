# ==============================================================================
#
# Assembly language support library, this tool translates between various
# dialects of a particular Assembly language,
#
# (C) copyright 2013, 2014 Moravia Microsystems, s.r.o.
#
# ==============================================================================


# ------------------------------------------------------------------------------
# GENERAL OPTIONS
# ------------------------------------------------------------------------------

# Set project name
project ( AsmTranslator )

# Target name for the AsmTranslator's CLI executable binary.
set ( TRANSLATOR_EXEC_TARGET "mds-translator" )

# ------------------------------------------------------------------------------
# WINDRES RC FILES.
# Windows only: generate windres RC file(s) to provide information for Windows.
# ------------------------------------------------------------------------------

windres_rc ( OUTPUT_VAR         LIB_RC_FILE
             FILETYPE           "DLL"
             FILE_VERSION       "1.0"
             FILEOS             "NT_WINDOWS32"
             FILE_DESCRIPTION   "MDS Assembler Translator"
             COMPANY_NAME       "Moravia Microsystems, s.r.o."
             TARGET             ${PROJECT_NAME}
             PRODUCT_NAME       ${PRODUCT_NAME}
             PRODUCT_VERSION    ${PRODUCT_VERSION}
             YEARS              ${COPYRIGHT_YEARS} )

windres_rc ( OUTPUT_VAR         EXEC_RC_FILE
             FILETYPE           "APP"
             FILE_VERSION       "1.0"
             FILEOS             "NT_WINDOWS32"
             FILE_DESCRIPTION   "MDS Assembler Translator"
             ICON               "windres/mds-translator.ico"
             COMPANY_NAME       "Moravia Microsystems, s.r.o."
             TARGET             ${TRANSLATOR_EXEC_TARGET}
             PRODUCT_NAME       ${PRODUCT_NAME}
             PRODUCT_VERSION    ${PRODUCT_VERSION}
             YEARS              ${COPYRIGHT_YEARS} )

# ------------------------------------------------------------------------------
# COMPILATION OPTIONS
# ------------------------------------------------------------------------------

## Add a library to the project using the specified source files,
 # in this case it will produce dynamic C++ library.
add_library ( ${PROJECT_NAME} SHARED
              ${LIB_RC_FILE}
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
add_executable ( ${TRANSLATOR_EXEC_TARGET} ${EXEC_RC_FILE} main.cxx )
target_link_libraries ( ${TRANSLATOR_EXEC_TARGET} ${PROJECT_NAME} )

# ------------------------------------------------------------------------------
# INSTALLATION OPTIONS
# ------------------------------------------------------------------------------

if ( MDS_FEATURE_PICOBLAZE AND ( NOT MDS_GRADE STREQUAL "Basic" ) )
    install ( TARGETS ${PROJECT_NAME}
              RUNTIME DESTINATION ${INSTALL_DIR_BIN}
              LIBRARY DESTINATION ${INSTALL_DIR_LIB} )

    if ( MDS_FEATURE_COMPILER_CLI )
        install ( TARGETS ${TRANSLATOR_EXEC_TARGET}
                  RUNTIME DESTINATION ${INSTALL_DIR_BIN}
                  LIBRARY DESTINATION ${INSTALL_DIR_LIB} )
    endif()
endif()
