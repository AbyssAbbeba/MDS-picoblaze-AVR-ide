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
 * @file DataFile.h
 */
// =============================================================================


#ifndef DATAFILE_H
#define DATAFILE_H

#include <string>
#include <cstdint>

/**
 * @brief
 * @ingroup MCUDataFiles
 * @class DataFile
 */
class DataFile
{
    ////    Public Datatypes    ////
    public:
        /**
         * @brief
         */
        class Exception
        {
            ////    Public Datatypes    ////
            public:
                /**
                * @brief
                */
                enum Type
                {
                    EXP_IO_ERROR,          ///< Unable to work with the specified file.
                    EXP_BAD_RECORD_LENGTH, ///< Bad record length.
                    EXP_BAD_CRC,           ///< Bad CRC.
                    EXP_BAD_RECORD_TYPE,   ///< Bad record type.
                    EXP_MEMORY_OVERFLOW,   ///< Object cannot contain such large amount of data.
                    EXP_OUT_OF_RANGE,      ///< Requested byte address is out of range.
                    EXP_BAD_RECORD_FORMAT, ///<
                    EXP_BAD_RECORD_COUNT   ///<
                };

            ////    Constructors and Destructors    ////
            public:
                /**
                 * @brief
                 * @param[in] errorType
                 * @param[in] errorInfo
                 */
                Exception ( Type errorType,
                            const std::string & errorInfo = "" )
                          : m_errorType ( errorType ),
                            m_errorInfo ( errorInfo ) {};

            private:
                /**
                 * @brief Forbidden constructor.
                 */
                Exception();

            ////    Public Operations    ////
            public:
                /**
                 * @brief
                 * @return
                 */
                std::string toString() const
                {
                    std::string result;
                    switch ( m_errorType )
                    {
                        case EXP_IO_ERROR:          result = "IO_ERROR";          break;
                        case EXP_BAD_RECORD_LENGTH: result = "BAD_RECORD_LENGTH"; break;
                        case EXP_BAD_CRC:           result = "BAD_CRC";           break;
                        case EXP_BAD_RECORD_TYPE:   result = "BAD_RECORD_TYPE";   break;
                        case EXP_MEMORY_OVERFLOW:   result = "MEMORY_OVERFLOW";   break;
                        case EXP_OUT_OF_RANGE:      result = "OUT_OF_RANGE";      break;
                        case EXP_BAD_RECORD_FORMAT: result = "BAD_RECORD_FORMAT"; break;
                        case EXP_BAD_RECORD_COUNT:  result = "BAD_RECORD_COUNT";  break;
                    }

                    return result + ": " + m_errorInfo;
                }

            ////    Public Attributes    ////
            public:
                /**
                 * @brief
                 */
                const Type m_errorType;

                /**
                 * @brief
                 */
                const std::string m_errorInfo;
        };

    ////    Constructors and Destructors    ////
    public:
        /**
         * @brief
         * @param[in] arrsize
         */
        DataFile ( unsigned int arrsize = 0x10000 );

        /**
         * @brief
         */
        ~DataFile();

    ////    Public Operations    ////
    public:
        /**
         * @brief
         * @param[in] obj
         * @return
         */
        bool operator == ( const DataFile & obj ) const;

        /**
         * @brief Load data file into the memory array
         * @param[in] filename
         */
        virtual void clearAndLoad ( const char * filename ) throw ( DataFile::Exception ) = 0;

        /// @overload
        virtual void clearAndLoad ( const std::string & filename ) throw ( DataFile::Exception ) = 0;

        /**
         * @brief Save memory array in data file
         * @param[in] filename Target file
         * @param[in] makeBackup Make backup file
         */
        virtual void save ( const char * filename,
                            bool makeBackup = true ) throw ( DataFile::Exception ) = 0;

        /// @overload
        virtual void save ( const std::string & filename,
                            bool makeBackup = true ) throw ( DataFile::Exception ) = 0;

        /**
         * @brief Set value for certain cell in the memory array
         * @param[in] address Address of the target memory cell
         * @param[in] value New value (0..255)
         */
        void set ( unsigned int address,
                   uint8_t value ) throw ( Exception );

        /**
         * @brief Unset certain cell in the memory array.
         * @param[in] address Address of the target memory cell
         */
        void unset ( unsigned int address ) throw ( Exception );

        /**
         * @brief Same as the indexing operator.
         * @warning In case the index is out of range, the OUT_OF_RANGE exception is thrown.
         * @param[in] idx Cell address
         * @return Cell value (-1..255) (-1 means undefined)
         */
        int at ( int idx ) const throw ( Exception );

        /**
         * @brief Clear the memory array.
         */
        void clear();

        /**
         * @brief Get number of nonempty pages of the given size.
         * @param[in] blockSize Page size (e.g. 64)
         * @return Number of nonempty pages
         */
        unsigned int getNumberOfNotEmptyPages ( unsigned int pageSize ) const;

        /**
         * @brief Determinate if the specified page is empty.
         * @param[in] blockSize Page size (e.g. 64)
         * @param[in] blockNumber Page number
         * @return True if the block is empty
         */
        bool isPageEmpty ( unsigned int pageSize,
                           unsigned int pageNumber ) const;

        /**
         * @brief Get pointer to whole memory array.
         * @return The memory array.
         */
        int16_t * getData();

        /**
         * @brief Replace native memory array with another one, the original memory is deleted.
         *
         * @param[in,out] data Memory array from somewhere else. Must be the same size!
         */
        void setData ( int16_t * data );

    ////    Inline Public Operations    ////
    public:
        /**
         * @brief Get value contained in certain memory cell.
         * @warning No check for address validity is performed!
         * @param idx Cell address (index)
         * @return Cell value (-1..255) (-1 means undefined)
         */
        int operator [] ( int idx ) const
        {
            return int(m_memory[idx]);
        }

        /**
         * @brief Get overall capacity.
         * @return capacity in bytes
         */
        unsigned int maxSize() const
        {
            return m_arrsize;
        }

        /**
         * @brief Get value for certain cell in the memory array
         * @note In case the address is out of range, the method returns -1.
         * @param[in] address Address of the source memory cell
         * @return Cell value (-1..255) (-1 means undefined)
         */
        int get ( unsigned int address ) const
        {
            return ( ( address >= m_arrsize ) ? -1 : int(m_memory[address]) );
        }

    ////    Inline Private Operations    ////
    private:
        /**
         * @brief
         */
        inline void allocateMemory();

    ////    Protected Attributes    ////
    protected:
        const unsigned int m_arrsize;   ///< Size of the memory array
        int16_t * m_memory;             ///< Memory array
};

#endif // DATAFILE_H
