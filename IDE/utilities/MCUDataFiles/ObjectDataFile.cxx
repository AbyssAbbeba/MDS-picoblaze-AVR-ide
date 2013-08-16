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
 * @file ObjectDataFile.cxx
 */
// =============================================================================

#include "ObjectDataFile.h"


ObjectDataFile::ObjectDataFile ( unsigned int arrsize ) : DataFile(arrsize)
{
}

void ObjectDataFile::clearAndLoad ( const char * ) throw ( DataFileException )
{
}

void ObjectDataFile::clearAndLoad ( const std::string & ) throw ( DataFileException )
{
}

void ObjectDataFile::save ( const char *,
                            bool ) throw ( DataFileException )
{
}

void ObjectDataFile::save ( const std::string &,
                            bool ) throw ( DataFileException )
{
}
