# ==============================================================================
#
# Simulator widget showing contents of PicoBlaze stack.
#
# (C) copyright 2014 Moravia Microsystems, s.r.o.
#
# ==============================================================================

# ------------------------------------------------------------------------------
# GENERAL OPTIONS
# ------------------------------------------------------------------------------

project ( StackWidget )

# ------------------------------------------------------------------------------
# COMPILATION OPTIONS
# ------------------------------------------------------------------------------

QT4_WRAP_CPP( MOC_FILES stackwidget.h )

add_library ( ${PROJECT_NAME}
              ${MOC_FILES}
              stackwidget.cpp )

target_link_libraries ( ${PROJECT_NAME}
                        ${QT_LIBRARIES} Simulator ErrorDialog GuiCfg )

# ------------------------------------------------------------------------------
# INSTALLATION
# ------------------------------------------------------------------------------

if ( BUILD_SHARED_LIBS )
    install ( TARGETS ${PROJECT_NAME}
              RUNTIME DESTINATION ${INSTALL_DIR_BIN}
              LIBRARY DESTINATION ${INSTALL_DIR_LIB} )
endif()
