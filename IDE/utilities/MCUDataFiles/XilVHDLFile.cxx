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
 * @file XilVHDLFile.cxx
 */
// =============================================================================

#include "XilVHDLFile.h"

void XilVHDLFile::clearAndLoad ( const char * filename ) throw ( DataFileException )
{
    clearAndLoad(std::string(filename));
}

void XilVHDLFile::clearAndLoad ( const std::string & filename ) throw ( DataFileException )
{
}

void XilVHDLFile::save ( const char * filename,
                         bool makeBackup ) throw ( DataFileException )
{
    save(std::string(filename), makeBackup);
}

void XilVHDLFile::save ( const std::string & filename,
                         bool makeBackup ) throw ( DataFileException )
{
}
