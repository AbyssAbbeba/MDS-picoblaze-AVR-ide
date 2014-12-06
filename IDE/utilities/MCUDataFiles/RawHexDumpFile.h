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
 * @file RawHexDumpFile.h
 */
// =============================================================================

#ifndef RAWHEXDUMPFILE_H
#define RAWHEXDUMPFILE_H

#include "DataFile.h"

/**
 * @brief Tool for working with Intel® 16 Hex files.
 * @ingroup MCUDataFiles
 * @class HexFile
 *
 * This class provides interface for reading and creating valid HEX-16 files.
 */
class RawHexDumpFile : public DataFile
{
    ////    Public Datatypes    ////
    public:
        /**
         * @brief
         */
        enum OPCodeSize
        {
            SIZE_16b = 16, ///<
            SIZE_18b = 18  ///<
        };

    ////    Constructors and Destructors    ////
    public:
        /**
         * @brief
         * @param[in] size Data size.
         */
        RawHexDumpFile ( OPCodeSize opCodeSize,
                         unsigned int arrsize = ( 3 * 4096 ) )
                       :
                         DataFile ( arrsize ),
                         m_opCodeSize ( opCodeSize ) {};

        /**
         * @brief
         * @param[in] file
         */
        RawHexDumpFile ( OPCodeSize opCodeSize,
                         const std::string & file )
                       :
                         m_opCodeSize ( opCodeSize )
        {
            clearAndLoad(file);
        }

    ////    Public Operations    ////
    public:
        /**
         * @brief Load Raw Hex Dump file into the memory array.
         * @param[in] filename Source file
         */
        virtual void clearAndLoad ( const char * filename ) override;

        /// @overload
        virtual void clearAndLoad ( const std::string & filename ) override;

        /**
         * @brief Save memory array in Raw Hex Dump file.
         * @param[in] filename Target file
         * @param[in] makeBackup Make backup file
         */
        virtual void save ( const char * filename,
                            bool makeBackup = true ) override;

        /// @overload
        virtual void save ( const std::string & filename,
                            bool makeBackup = true ) override;

    ////    Inline Private Operations    ////
    private:
        /**
         * @brief
         * @param[in] addr
         */
        inline void checkAddr ( unsigned int addr );

    ////    Private Attributes    ////
    private:
        /// @brief
        OPCodeSize m_opCodeSize;
};

#endif // RAWHEXDUMPFILE_H
