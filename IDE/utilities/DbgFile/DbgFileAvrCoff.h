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
 * @ingroup DbgFile
 * @file DbgFileAvrCoff.h
 */
// =============================================================================

#ifndef DBGFILEAVRCOFF_H
#define DBGFILEAVRCOFF_H

// Forward declarations
class ObjectDataFile;

#include "DbgFile.h"
#include <fstream>

/**
 * @brief
 * @ingroup DbgFile
 * @class DbgFileAvrCoff
 */
class DbgFileAvrCoff : public DbgFile
{
    ////    Public Static Constants    ////
    public:
        /// @brief
        static const unsigned int IN_BFR_SIZE = 4095;

    ////    Public Datatypes    ////
    public:
        /**
         * @brief
         */
        enum ObjFileType
        {
            OFT_NORELOC   = 0x0001, ///< Relocation information stripped from the file
            OFT_NOUNRES   = 0x0002, ///< The file has no unresolved external references
            OFT_NOLINES   = 0x0004, ///< Line numbers stripped from the file
            OFT_NOSYMBOLS = 0x0008  ///< Local symbols stripped from the file
        };

        /**
         * @brief
         */
        enum SectionFlag
        {
            SF_TEXT    = 0x0020, ///< Section contains executable text (.text)
            SF_DATA    = 0x0040, ///< Section contains initialised data (.data)
            SF_BSS     = 0x0080, ///< Section contains uninitialised data (.bss)
            SF_COMMENT = 0x0200  ///< Comment section
        };

    ////    Constructors and Destructors    ////
    public:
        /**
         * @brief
         */
        DbgFileAvrCoff();

        /**
         * @brief
         */
        ~DbgFileAvrCoff();

    ////    Public Operations    ////
    public:
        /**
         * @brief
         * @param[in] filename
         */
        void openFile ( const std::string & filename ) throw ( Exception );

        /**
         * @brief
         * @param[in,out] dataContainer
         */
        void assignCodeMemDataContainer ( ObjectDataFile * dataContainer );

        /**
         * @brief
         * @param[in] addr
         * @return
         */
        int getLineByAddr ( unsigned int addr ) const;

        /**
         * @brief
         * @param[in] addr
         * @param[in] filename
         * @return
         */
        int getAddrByLine ( unsigned int line,
                            const std::string & filename ) const;

    ////    Inline Private Operations    ////
    private:
        /**
         * @brief
         * @param[in] line
         * @return
         */
        inline const char * checkHexNumber ( const char * line ) const;

        /**
         * @brief
         * @param[in] filename
         */
        inline void loadFile ( const std::string & filename );

        /**
         * @brief
         */
        inline void clear();

        /**
         * @brief
         * @param[in] filename
         * @return
         */
        inline int getFileNumber ( const std::string & filename ) const;

        /**
         * @brief
         */
        inline void generateLineAddressMaps();

        /**
         * @brief
         */
        inline void checkBadAndEof ( const std::fstream & file, char * buffer = NULL );

    ////    Private Attributes    ////
    private:
        /**
         * @brief
         */
        int * m_addrToLineMap;

        /**
         * @brief
         */
        int ** m_lineToAddrMap;
};

#endif // DBGFILEAVRCOFF_H
