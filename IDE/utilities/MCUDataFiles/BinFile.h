// =============================================================================
/**
 * @brief
 * C++ Interface: ...
 *
 * ...
 *
 * (C) copyright 2013 Moravia Microsystems, s.r.o.
 *
 * @author Martin Ošmera <martin.osmera@gmail.com>
 * @ingroup MCUDataFiles
 * @file BinFile.h
 */
// =============================================================================

#ifndef BINFILE_H
#define BINFILE_H

#include "DataFile.h"
#include <string>

/**
 * @brief
 * @ingroup MCUDataFiles
 * @class BinFile
 */
class BinFile : public DataFile
{
    ////    Constructors and Destructors    ////
    public:
        /**
         * @brief
         * @param[in] arrsize
         */
        BinFile ( unsigned int arrsize = 0x10000 ) : DataFile(arrsize) {};

        /**
         * @brief
         * @param[in] file
         */
        BinFile ( const std::string & file )
        {
            clearAndLoad(file);
        }

    ////    Public Operations    ////
    public:
        /**
         * @brief Load binary file into the memory array
         * @param[in] filename Source file
         */
        void clearAndLoad ( const char * filename ) throw(DataFile::Exception);

        /// @overload
        void clearAndLoad ( const std::string & filename ) throw(DataFile::Exception);

        /**
         * @brief Save memory array in binary file
         * @param[in] filename Target file
         * @param[in] makeBackup Make backup file
         */
        void save ( const char * filename,
                    bool makeBackup = true ) throw(DataFile::Exception);

        /// @overload
        void save ( const std::string & filename,
                    bool makeBackup = true ) throw(DataFile::Exception);
};

#endif // BINFILE_H
