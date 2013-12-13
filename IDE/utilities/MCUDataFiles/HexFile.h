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
 * @file HexFile.h
 */
// =============================================================================

#ifndef HEXFILE_H
#define HEXFILE_H

#include "DataFile.h"
#include <string>

/**
 * @brief Tool for working with Intel® 16 Hex files.
 * @ingroup MCUDataFiles
 * @class HexFile
 *
 * This class provides interface for reading and creating valid HEX-16 files.
 */
class HexFile : public DataFile
{
    ////    Constructors and Destructors    ////
    public:
        /**
         * @brief
         * @param[in] size Maximum data size
         */
        HexFile ( unsigned int arrsize = 0x10000,
                  unsigned char maxRecLength = 255 )
                : DataFile ( arrsize ),
                  m_maxRecLength ( maxRecLength ) {};

        /**
         * @brief
         * @param[in] file
         */
        HexFile ( const std::string & file )
                : m_maxRecLength ( 255 )
        {
            clearAndLoad(file);
        }

    ////    Public Operations    ////
    public:
        /**
         * @brief Load I16HEX file into the memory array
         * @param[in] filename
         */
        void clearAndLoad ( const char * filename ) throw ( DataFileException );

        /// @overload
        void clearAndLoad ( const std::string & filename ) throw ( DataFileException );

        /**
         * @brief Save memory array in I16HEX file
         * @param[in] filename Target file
         * @param[in] makeBackup Make backup file
         */
        void save ( const char * filename,
                    bool makeBackup = true ) throw ( DataFileException );

        /// @overload
        void save ( const std::string & filename,
                    bool makeBackup = true ) throw ( DataFileException );

    ////    Private Operations    ////
    private:
        /**
         * @brief Compute I16HEX CRC from the given string
         * @param[in] data Source string (e.g. "FA5568BA98")
         * @return CRC (0..255)
         */
        inline int computeCRC ( const char * data ) const;

    ////    Private Attributes    ////
    private:
        /// Maximum allowed size of HEX record, in bytes of data field (1-255).
        const unsigned char m_maxRecLength;
};

#endif // HEXFILE_H
