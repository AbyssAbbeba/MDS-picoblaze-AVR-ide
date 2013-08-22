// =============================================================================
/**
 * @brief
 * C++ Interface: ...
 *
 * ...
 *
 * (C) copyright 2013 Moravia Microsystems, s.r.o.
 *
 * @author Martin Ošmera <martin.osmera@moravia-microsystems.com>
 * @ingroup MCUDataFiles
 * @file XilMemFile.h
 */
// =============================================================================

#ifndef XILMEMFILE_H
#define XILMEMFILE_H

#include "DataFile.h"

/**
 * @brief
 * @ingroup MCUDataFiles
 * @class XilMemFile
 */
class XilMemFile : public DataFile
{
    ////    Public Static Constants    ////
    public:
        /// @brief
        static const unsigned int MAX_LINE_LENGTH = 1024;

    ////    Constructors and Destructors    ////
    public:
        /**
         * @param[in] size Maximum data size
         */
        XilMemFile ( unsigned int bytesPerRecord,
                     unsigned int arrsize        = 0x4000,
                     unsigned int recordsPerLine = 16,
                     const char * eol            = "\r\n" )
                   :
                     DataFile ( arrsize ),
                     m_bytesPerRecord ( bytesPerRecord ),
                     m_recordsPerLine ( recordsPerLine ),
                     m_eol ( eol ) {};

        /**
         * @brief
         * @param[in] file
         */
        XilMemFile ( const std::string & file,
                     unsigned int bytesPerRecord,
                     unsigned int arrsize        = 0x4000,
                     unsigned int recordsPerLine = 16,
                     const char * eol            = "\r\n" )
                   :
                     DataFile ( arrsize ),
                     m_bytesPerRecord ( bytesPerRecord ),
                     m_recordsPerLine ( recordsPerLine ),
                     m_eol ( eol )
        {
            clearAndLoad(file);
        }

    ////    Public Operations    ////
    public:
        /**
         * @brief Load a file into the memory array
         * @param[in] filename
         */
        void clearAndLoad ( const char * filename ) throw ( DataFileException );

        /// @overload
        void clearAndLoad ( const std::string & filename ) throw ( DataFileException );

        /**
         * @brief Save memory array in a file
         * @param[in] filename Target file
         * @param[in] makeBackup Make backup file
         */
        void save ( const char * filename,
                    bool makeBackup = true ) throw ( DataFileException );

        /// @overload
        void save ( const std::string & filename,
                    bool makeBackup = true ) throw ( DataFileException );

    ////    Inline Private Operations    ////
    private:
        /**
         * @brief
         * @param[in] string
         * @return
         */
        inline bool checkHex ( const char * string );

        /**
         * @brief
         * @param[in] string
         * @return
         */
        inline char * getField ( char ** line );

    ////    Private Attributes    ////
    private:
        /// @brief
        const unsigned int m_bytesPerRecord;

        /// @brief
        const unsigned int m_recordsPerLine;

        /// @brief
        const char * const m_eol;
};

#endif // XILMEMFILE_H
