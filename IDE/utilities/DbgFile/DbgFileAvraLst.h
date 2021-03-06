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
 * @file DbgFileAvraLst.h
 */
// =============================================================================

#ifndef DBGFILEAVRALST_H
#define DBGFILEAVRALST_H

#include "DbgFile.h"

/**
 * @brief
 * @ingroup DbgFile
 * @class DbgFileAvraLst
 */
class DbgFileAvraLst : public DbgFile
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
        DbgFileAvraLst();

        /**
         * @brief
         */
        ~DbgFileAvraLst();

    ////    Public Operations    ////
    public:
        /**
         * @brief
         * @param[in] filename
         */
        virtual void openFile ( const std::string & filename ) override;

        /**
         * @brief
         * @param[in] addr
         * @param[out] recordNumber
         */
        virtual void getLineByAddr ( unsigned int addr,
                                     std::vector<unsigned int> & recordNumbers ) const override;

        /**
         * @brief
         * @param[in] addr
         * @param[in] filename
         * @return
         */
        virtual int getAddrByLine ( unsigned int line,
                                    const std::string & filename ) const override;

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

#endif // DBGFILEAVRALST_H
