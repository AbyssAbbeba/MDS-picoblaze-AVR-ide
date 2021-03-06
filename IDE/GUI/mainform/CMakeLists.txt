# ==============================================================================
#
# Central component of the GUI.
#
# (C) copyright 2013, 2014 Moravia Microsystems, s.r.o.
#
# ==============================================================================

# ------------------------------------------------------------------------------
# GENERAL OPTIONS
# ------------------------------------------------------------------------------

# Set project name
project ( Mainform )

# ------------------------------------------------------------------------------
# CONFIGURE FEATURE SET
# ------------------------------------------------------------------------------

# List of additional libraries which can be enabled or disabled for this particular build.
set ( MDS_ADDITONAL_LIBS "" )

if ( MDS_FEATURE_FILECONVERTER )
    list ( APPEND MDS_ADDITONAL_LIBS "FileConvertDlg" )
endif()
if ( MDS_FEATURE_DISASSEMBLER )
    list ( APPEND MDS_ADDITONAL_LIBS "DisAsmDlg" )
endif()
if ( MDS_FEATURE_TRANSLATOR )
    list ( APPEND MDS_ADDITONAL_LIBS "TranslatorDlg" )
    list ( APPEND MDS_ADDITONAL_LIBS "AsmTranslator" )
endif()
if ( MDS_FEATURE_LICENCE_CERTIFICATE )
    list ( APPEND MDS_ADDITONAL_LIBS "LicenseWidget" )
endif()
if ( MDS_FEATURE_CONVERTER_TOOL )
    list ( APPEND MDS_ADDITONAL_LIBS "ConvertorTool" )
endif()
if ( MDS_FEATURE_LOOP_GENERATOR )
    list ( APPEND MDS_ADDITONAL_LIBS "LoopGen" )
endif()
if ( MDS_FEATURE_8_SEGMENT_EDITOR )
    list ( APPEND MDS_ADDITONAL_LIBS "DisplayTool" )
endif()
if ( MDS_FEATURE_SIM_LED_PANEL )
    list ( APPEND MDS_ADDITONAL_LIBS "SimLed" )
endif()
if ( MDS_FEATURE_SIM_7_SEGMENT )
    list ( APPEND MDS_ADDITONAL_LIBS "Sim7Seg" )
endif()

if ( MDS_FEATURE_SIM_SWITCH )
    list ( APPEND MDS_ADDITONAL_LIBS "SimSwitch" )
endif()
if ( MDS_FEATURE_SIM_PORT_LOGGER )
    list ( APPEND MDS_ADDITONAL_LIBS "SimPortLogger" )
endif()
if ( MDS_FEATURE_SIM_CALLWATCHER )
    list ( APPEND MDS_ADDITONAL_LIBS "CallWatcher" )
endif()
if ( MDS_FEATURE_REG_CALLWATCHER )
    list ( APPEND MDS_ADDITONAL_LIBS "RegWatcher" )
endif()
if ( MDS_FEATURE_VHDL_WIZARD )
    list ( APPEND MDS_ADDITONAL_LIBS "VHDLGen" )
endif()
if ( MDS_FEATURE_SYMBOLTABLE )
    list ( APPEND MDS_ADDITONAL_LIBS "SymbolTable" )
endif()
if ( MDS_FEATURE_SIM_STOPWATCH )
    list ( APPEND MDS_ADDITONAL_LIBS "StopWatch" )
endif()
if ( MDS_FEATURE_SIM_TESTPORTTOOL )
    list ( APPEND MDS_ADDITONAL_LIBS "TestPortTool" )
endif()

# ------------------------------------------------------------------------------
# COMPILATION OPTIONS
# ------------------------------------------------------------------------------

# Take Qt specific C++ header file(s) and compile them using Qt Meta Object Compiler.
qt4_wrap_cpp ( MOC_FILES mainform.h )

# Add a library to the project using the specified source files
add_library ( ${PROJECT_NAME}
              ${MOC_FILES} mainform.cpp )

# Link the target to the specified libraries.
target_link_libraries ( ${PROJECT_NAME} ${QT_LIBRARIES} ${MDS_ADDITONAL_LIBS}
                        WDockManager    MCUDataFiles    InterfaceCfg    ErrorDialog     ProjectTree
                        AboutWidget     CompileInfo     ProjectDlg      ProjectCfg      HelpWidget
                        Simulator       compiler        Project         Editor          GuiCfg
                        WelcomeWidget   SaveDlg         WelcomeScr      CompatibilityMode )

# ------------------------------------------------------------------------------
# INSTALLATION OPTIONS
# ------------------------------------------------------------------------------

if ( BUILD_SHARED_LIBS )
    install ( TARGETS ${PROJECT_NAME}
              RUNTIME DESTINATION ${INSTALL_DIR_BIN}
              LIBRARY DESTINATION ${INSTALL_DIR_LIB} )
endif()
