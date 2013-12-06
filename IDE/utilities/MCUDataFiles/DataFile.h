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
 * @file DataFile.h
 */
// =============================================================================

#ifndef DATAFILE_H
#define DATAFILE_H

#include "DataFileException.h"

#include <string>
#include <cstdint>

/**
 * @brief
 * @ingroup MCUDataFiles
 * @class DataFile
 */
class DataFile
{
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
        virtual void clearAndLoad ( const char * filename ) throw ( DataFileException ) = 0;

        /// @overload
        virtual void clearAndLoad ( const std::string & filename ) throw ( DataFileException ) = 0;

        /**
         * @brief Save memory array in data file
         * @param[in] filename Target file
         * @param[in] makeBackup Make backup file
         */
        virtual void save ( const char * filename,
                            bool makeBackup = true ) throw ( DataFileException ) = 0;

        /// @overload
        virtual void save ( const std::string & filename,
                            bool makeBackup = true ) throw ( DataFileException ) = 0;

        /**
         * @brief Set value for certain cell in the memory array
         * @param[in] address Address of the target memory cell
         * @param[in] value New value (0..255)
         */
        void set ( unsigned int address,
                   uint8_t value ) throw ( DataFileException );

        /**
         * @brief Unset certain cell in the memory array.
         * @param[in] address Address of the target memory cell
         */
        void unset ( unsigned int address ) throw ( DataFileException );

        /**
         * @brief Same as the indexing operator.
         * @warning In case the index is out of range, the OUT_OF_RANGE exception is thrown.
         * @param[in] idx Cell address
         * @return Cell value (-1..255) (-1 means undefined)
         */
        int at ( int idx ) const throw ( DataFileException );

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
