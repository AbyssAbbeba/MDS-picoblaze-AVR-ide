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
 * @file XilVerilogFile.h
 */
// =============================================================================

#ifndef XILVERILOGFILE_H
#define XILVERILOGFILE_H

#include "DataFile.h"

/**
 * @brief
 * @ingroup MCUDataFiles
 * @class XilVerilogFile
 */
class XilVerilogFile : public DataFile
{
    ////    Constructors and Destructors    ////
    public:
        /**
         * @param[in] size Maximum data size
         */
        XilVerilogFile ( unsigned int arrsize = 0x10000 )
                       : DataFile ( arrsize ) {};

        /**
         * @brief
         * @param[in] file
         */
        XilVerilogFile ( const std::string & file )
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
};

#endif // XILVERILOGFILE_H