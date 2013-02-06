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
 * @ingroup DbgFile
 * @file DbgFileAvrCoff.h
 */
// =============================================================================

#ifndef DBGFILEAVRCOFF_H
#define DBGFILEAVRCOFF_H

// Forward declarations
class ObjectDataFile;

#include "DbgFile.h"

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
        static const unsigned int MAX_LINE_LENGTH = 4095;

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
        virtual void openFile ( const std::string & filename ) throw ( DbgFileException );

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
        virtual int getLineByAddr ( unsigned int addr ) const;

        /**
         * @brief
         * @param[in] addr
         * @param[in] filename
         * @return
         */
        virtual int getAddrByLine ( unsigned int line,
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
