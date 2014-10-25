# set project's name
project ( MDSExample )

foreach ( extension "mds-project" "bin" "ctr" "dbg" "ihex" "lst" "mem" "mtbl" "psm" "rawhex" "srec" "stbl" "v" "vhd" )
    file ( GLOB files2install MDSExample/*.${extension} )
    install ( FILES ${files2install} DESTINATION ${INSTALL_DIR_DEMOPROJECT} )
endforeach ( extension )
