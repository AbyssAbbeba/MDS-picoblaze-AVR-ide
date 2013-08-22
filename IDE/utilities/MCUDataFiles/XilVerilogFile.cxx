// =============================================================================
/**
 * @brief
 * C++ Implementation: ...
 *
 * ...
 *
 * (C) copyright 2013 Moravia Microsystems, s.r.o.
 *
 * @author Martin Ošmera <martin.osmera@moravia-microsystems.com>
 * @ingroup MCUDataFiles
 * @file XilVerilogFile.cxx
 */
// =============================================================================

#include "XilVerilogFile.h"

void XilVerilogFile::clearAndLoad ( const char * filename ) throw ( DataFileException )
{
    clearAndLoad(std::string(filename));
}

void XilVerilogFile::clearAndLoad ( const std::string & filename ) throw ( DataFileException )
{
}

void XilVerilogFile::save ( const char * filename,
                            bool makeBackup ) throw ( DataFileException )
{
    save(std::string(filename), makeBackup);
}

void XilVerilogFile::save ( const std::string & filename,
                            bool makeBackup ) throw ( DataFileException )
{
}
