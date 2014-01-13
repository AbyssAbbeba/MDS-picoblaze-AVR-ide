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
 * @file DbgFileNative.h
 */
// =============================================================================

#ifndef DBGFILENATIVE_H
#define DBGFILENATIVE_H

#include "DbgFile.h"

// Standard header files.
#include <vector>
#include <string>
#include <utility>

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
         * @param[in] file
         */
        DbgFileNative ( const std::string & file);

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

        /// @name Methods for direct set-up of the debug data container, i.e. without loading a debug file.
        //@{
            /**
             * @brief
             */
            void directSetupPrepare();

            /**
             * @brief
             * @param[in] files
             */
            void directSetupFiles ( const std::vector<std::pair<std::string,FILE*>> & files );

            /**
             * @brief
             * @param[in] addr
             * @param[in] file
             * @param[in] line
             */
            void directSetupRelation ( unsigned int addr,
                                       unsigned int file,
                                       unsigned int line );

            /**
             * @brief
             * @param[in] addr
             * @param[in] locations
             */
            void directSetupRelation ( unsigned int addr,
                                       const std::vector<std::pair<unsigned int, unsigned int>> & locations );

            /**
             * @brief
             */
            void directSetupFinalize();
        //@}

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
        std::vector<unsigned int> * m_addrToLineMap;

        /**
         * @brief
         */
        int ** m_lineToAddrMap;
};

#endif // DBGFILENATIVE_H
