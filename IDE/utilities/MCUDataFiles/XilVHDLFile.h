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
 * @file XilVHDLFile.h
 */
// =============================================================================

#ifndef XILVHDLFILE_H
#define XILVHDLFILE_H

#include "XilHDLFile.h"

/**
 * @brief
 * @ingroup MCUDataFiles
 * @class XilVHDLFile
 */
class XilVHDLFile : public XilHDLFile
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
        XilVHDLFile ( const std::string & name,
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
        XilVHDLFile ( const std::string & inputFile,
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

#endif // XILVHDLFILE_H
