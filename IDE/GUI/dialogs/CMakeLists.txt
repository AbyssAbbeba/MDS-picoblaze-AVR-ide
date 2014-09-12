##

# ------------------------------------------------------------------------------
# GENERAL OPTIONS
# ------------------------------------------------------------------------------

add_subdirectory(cfginterface)
add_subdirectory(projectcfg)
add_subdirectory(interfacecfg)
add_subdirectory(projectdlg)

if ( MDS_FEATURE_TRIAL )
    add_subdirectory(TrialExpired)
endif()

if ( MDS_FEATURE_FILECONVERTER )
    add_subdirectory(fileconvertdlg)
endif()

if ( MDS_FEATURE_DISASSEMBLER )
    add_subdirectory(disasmdlg)
endif()

if ( MDS_FEATURE_TRANSLATOR )
    add_subdirectory(translatordlg)
endif()
