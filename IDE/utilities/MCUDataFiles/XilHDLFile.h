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
 * @file XilHDLFile.h
 */
// =============================================================================

#ifndef XILHDLFILE_H
#define XILHDLFILE_H

#include "DataFile.h"

/**
 * @brief
 * @ingroup MCUDataFiles
 * @class XilHDLFile
 */
class XilHDLFile : public DataFile
{
    ////    Protected Static Constants    ////
    protected:
        /// @brief
        static const char * const MARK_TIMESTAMP;

        /// @brief
        static const char * const MARK_NAME;

        /// @brief
        static const char * const MARK_START;

        /// @brief
        static const char * const MARK_INIT_S;

        /// @brief
        static const char * const MARK_INIT_E;

        /// @brief
        static const char * const MARK_INITP_S;

        /// @brief
        static const char * const MARK_INITP_E;

        /// @brief
        static const char * const EOL_SEQUENCE;

    ////    Public Datatypes    ////
    public:
        /**
         * @brief
         */
        enum OPCodeSize
        {
            SIZE_16b = 0, ///<
            SIZE_18b      ///<
        };

    ////    Constructors and Destructors    ////
    public:
        /**
         * @brief
         * @param[in] name
         * @param[in] templateFile
         * @param[in] opCodeSize
         * @param[in] arrsize
         */
        XilHDLFile ( const std::string & name,
                     const std::string & templateFile,
                     OPCodeSize opCodeSize,
                     unsigned int arrsize )
                   :
                     DataFile ( arrsize ),
                     m_name ( name ),
                     m_templateFile ( templateFile ),
                     m_opCodeSize ( opCodeSize ) {};

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

    ////    Protected Operations    ////
    protected:
        /**
         * @brief
         * @param[in] line
         * @param[in,out] hexField
         * @param[in] markFragment
         * @return
         */
        virtual int extractHexField ( const std::string & line,
                                      std::string * hexField,
                                      const char * markFragment ) = 0;

        /**
         * @brief
         * @param[in,out] line
         * @param[in] markStart
         * @param[in] markEnd
         * @param[in] parity
         * @return
         */
        void substDataMark ( std::string & line,
                             const char * markStart,
                             const char * markEnd,
                             bool parity = false ) const;

        /**
         * @brief
         * @param[in,out] dataField
         * @param[in] addr
         * @param[in] parity
         * @return
         */
        void generateDataField ( std::string * dataField,
                                 unsigned int addr,
                                 bool parity ) const;

        /**
         * @brief
         * @param[in] digit
         * @return
         */
        unsigned int hexDigit2uint ( char digit ) const;

        /**
         * @brief
         * @param[in] str
         * @return
         */
        bool checkHex ( const std::string & str ) const;

    ////    Protected Attributes    ////
    protected:
        /// @brief
        const std::string m_name;

        /// @brief
        const std::string m_templateFile;

        /// @brief
        const OPCodeSize m_opCodeSize;
};

#endif // XILHDLFILE_H
