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
 * @file XilVerilogFile.h
 */
// =============================================================================

#ifndef XILVERILOGFILE_H
#define XILVERILOGFILE_H

#include "XilHDLFile.h"

/**
 * @brief
 * @ingroup MCUDataFiles
 * @class XilVerilogFile
 */
class XilVerilogFile : public XilHDLFile
{
    ////    Constructors and Destructors    ////
    public:
        /**
         * @brief
         * @param[in] name
         * @param[in] templateFile
         * @param[in] opCodeSize
         * @param[in] arrsize
         */
        XilVerilogFile ( const std::string & name,
                         const std::string & templateFile,
                         OPCodeSize opCodeSize,
                         unsigned int arrsize = 0x4000 )
                       :
                         XilHDLFile ( name,
                                      templateFile,
                                      opCodeSize,
                                      arrsize ) {};

        /**
         * @brief
         * @param[in] inputFile
         * @param[in] name
         * @param[in] templateFile
         * @param[in] opCodeSize
         * @param[in] arrsize
         */
        XilVerilogFile ( const std::string & inputFile,
                         const std::string & name,
                         const std::string & templateFile,
                         OPCodeSize opCodeSize,
                         unsigned int arrsize = 0x4000 )
                       :
                         XilHDLFile ( name,
                                      templateFile,
                                      opCodeSize,
                                      arrsize )
        {
            clearAndLoad(inputFile);
        }

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
                                      const char * markFragment );
};

#endif // XILVERILOGFILE_H
