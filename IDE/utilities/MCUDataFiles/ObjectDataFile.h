// =============================================================================
/**
 * @brief
 * C++ Interface: ...
 *
 * ...
 *
 * (C) copyright 2013, 2014 Moravia Microsystems, s.r.o.
 *
 * @author Martin Ošmera <martin.osmera@moravia-microsystems.com>
 * @ingroup MCUDataFiles
 * @file ObjectDataFile.h
 */
// =============================================================================

#ifndef OBJECTDATAFILE_H
#define OBJECTDATAFILE_H

#include "DataFile.h"
#include <cstdint>
#include <string>
#include <fstream>
#include <vector>

/**
 * @brief
 * @ingroup MCUDataFiles
 * @class ObjectDataFile
 */
class ObjectDataFile : public DataFile
{
    ////    Constructors and Destructors    ////
    public:
        /**
         * @brief
         * @param[in] arrsize
         */
        ObjectDataFile ( unsigned int arrsize = 0x10000 );

        /**
         * @brief
         * @param[in] file
         */
        ObjectDataFile ( const std::string & file )
        {
            clearAndLoad(file);
        }

    ////    Public Operations    ////
    public:
        /**
         * @brief Load data file into the memory array
         * @param[in] filename
         */
        virtual void clearAndLoad ( const char * filename ) override;

        /// @overload
        virtual void clearAndLoad ( const std::string & filename ) override;

        /**
         * @brief Save memory array in data file
         * @param[in] filename Target file
         * @param[in] makeBackup Make backup file
         */
        virtual void save ( const char * filename,
                            bool makeBackup = true ) override;

        /// @overload
        virtual void save ( const std::string & filename,
                            bool makeBackup = true ) override;
};

#endif // OBJECTDATAFILE_H
