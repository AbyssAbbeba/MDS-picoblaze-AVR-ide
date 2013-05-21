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
 * @file DbgFileNative.h
 */
// =============================================================================

#ifndef DBGFILENATIVE_H
#define DBGFILENATIVE_H

#include "DbgFile.h"

/**
 * @brief
 * @ingroup DbgFile
 * @class DbgFileNative
 */
class DbgFileNative : public DbgFile
{
    ////    Constructors and Destructors    ////
    public:
        /**
         * @brief
         */
        DbgFileNative();

        /**
         * @brief
         */
        ~DbgFileNative();

    ////    Public Operations    ////
    public:
        /**
         * @brief
         * @param[in] filename
         */
        void openFile ( const std::string & filename ) throw ( DbgFileException );

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

#endif // DBGFILENATIVE_H
