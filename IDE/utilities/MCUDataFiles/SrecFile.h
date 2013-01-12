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
 * @file SrecFile.h
 */
// =============================================================================

#ifndef SRECFILE_H
#define SRECFILE_H

#include "DataFile.h"
#include <cstdint>
#include <string>
#include <fstream>
#include <vector>

/**
 * @brief
 * @ingroup MCUDataFiles
 * @class SrecFile
 */
class SrecFile : public DataFile
{
    ////    Public Datatypes    ////
    public:
        /**
         * @brief
         */
        struct BlockHeader
        {
            std::string m_mname;        ///<
            std::string m_ver;          ///<
            std::string m_rev;          ///<
            std::string m_description;  ///<
        };

    ////    Constructors and Destructors    ////
    public:
        /**
         * @brief
         * @param[in] arrsize
         */
        SrecFile ( unsigned int arrsize = 0x10000 );

    ////    Public Operations    ////
    public:
        /**
         * @brief Load Motorola S-Record file into the memory array
         * @param[in] filename
         */
        void clearAndLoad ( const char * filename ) throw ( DataFile::DataFileException );

        /// @overload
        void clearAndLoad ( const std::string & filename ) throw ( DataFile::DataFileException );

        /**
         * @brief Save memory array in Motorola S-Record file
         * @param[in] filename Target file
         * @param[in] makeBackup Make backup file
         */
        void save ( const char * filename,
                    bool makeBackup = true ) throw(DataFile::DataFileException);

        /// @overload
        void save ( const std::string & filename,
                    bool makeBackup = true ) throw ( DataFile::DataFileException );

        /**
         * @brief
         * @return
         */
        const std::string & getModuleName() const;

        /**
         * @brief
         * @return
         */
        const std::string & getVersion() const;

        /**
         * @brief
         * @return
         */
        const std::string & getRevision() const;

        /**
         * @brief
         * @return
         */
        const std::string & getDescription() const;

        /**
         * @brief
         * @return
         */
        unsigned int getStartingExecutionAddress() const;

        /**
         * @brief
         * @param[in] mname
         * @return
         */
        bool setModuleName ( const std::string & mname );

        /**
         * @brief
         * @param[in] ver
         * @return
         */
        bool setVersion ( const std::string & ver );

        /**
         * @brief
         * @param[in] rev
         * @return
         */
        bool setRevision ( const std::string & rev );

        /**
         * @brief
         * @param[in] desc
         * @return
         */
        bool setDescription ( const std::string & desc );

        /**
         * @brief
         * @param[in] addr
         * @return
         */
        bool setStartingExecutionAddress ( uint32_t addr );

    ////    Inline Private Operations    ////
    private:
        /**
         * @brief Compute Motorola S-Record CRC from the given string
         * @param[in] data Source string (e.g. "S00F000068656C6C6F20202020200000")
         * @return CRC (0..255)
         */
        inline int computeCRC ( const std::string & data ) const;

        /**
         * @brief
         * @param[in] source
         * @return
         */
        inline int hex2int ( const std::string & source ) const;

        /**
         * @brief
         * @param[in] source
         * @return
         */
        inline std::vector<unsigned char> readHex ( const std::string & source ) const;

        /**
         * @brief
         * @param[in] source
         * @return
         */
        inline std::string vec2str ( const std::vector<unsigned char> & source ) const;

        /**
         * @brief
         * @param[in,out] file
         * @param[in] mname
         * @param[in] ver
         * @param[in] rev
         * @param[in] description
         */
        inline void writeBlockHeaderSRecord ( std::fstream & file,
                                              const std::string & mname,
                                              const std::string & ver,
                                              const std::string & rev,
                                              const std::string & description ) const;

        /**
         * @brief
         * @param[in,out] file
         * @param[in] recordCount
         */
        inline void writeRecordCountSRecord ( std::fstream & file,
                                              int recordCount ) const;

        /**
         * @brief
         * @param[in,out] file
         * @param[in] address
         * @param[in] dataField
         */
        inline bool writeDataSRecord ( std::fstream & file,
                                       unsigned int address,
                                       const std::string & dataField ) const;

        /**
         * @brief
         * @param[in,out] file
         * @param[in] sea
         */
        inline void writeEndOfBlockSRecord ( std::fstream & file,
                                             unsigned int sea ) const;

    ////    Private Attributes    ////
    private:
        /**
         * @brief
         */
        BlockHeader m_blockHeader;

        /**
         * @brief
         */
        uint32_t m_startingExecutionAddress;
};

#endif // SRECFILE_H
