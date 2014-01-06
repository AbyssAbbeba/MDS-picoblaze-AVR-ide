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
        virtual ~DataFile();

    ////    Public Operations    ////
    public:
        /**
         * @brief Compare contents of this container instance with another one.
         * @param[in] obj Object to compare with this one.
         * @return true == they both contain the same data; false == they don't
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
         * @note
         * One you use this, this object becomes bonded with the array provided, any change here will therefore affect
         * the array.
         *
         * @param[in,out] data Memory array from somewhere else, this array won't be copied.
         * @param[in] arrsize Size of the memory array, as returned by maxSize();
         */
        void setData ( int16_t * data,
                       unsigned int arrsize );

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
        unsigned int m_arrsize;   ///< Size of the memory array.
        int16_t * m_memory;       ///< Memory array.
        bool m_dontDelete;        ///< If true, don't delete the memory array.
};

#endif // DATAFILE_H
