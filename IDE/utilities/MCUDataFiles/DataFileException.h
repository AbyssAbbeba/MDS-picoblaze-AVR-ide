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
 * @file DataFileException.h
 */
// =============================================================================

#ifndef DATAFILEEXCEPTION_H
#define DATAFILEEXCEPTION_H

#include <string>

/**
 * @brief
 * @ingroup MCUDataFiles
 * @class DataFileException
 */
class DataFileException
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
            EXP_BAD_RECORD_COUNT,  ///<
            EXP_TEMPLATE_ERROR,    ///<
            EXP_NOT_UNDERSTOOD     ///<
        };

    ////    Constructors and Destructors    ////
    public:
        /**
         * @brief
         * @param[in] errorType
         * @param[in] errorInfo
         */
        DataFileException ( Type errorType,
                            const std::string & errorInfo = "" )
                          : m_errorType ( errorType ),
                            m_errorInfo ( errorInfo ) {};

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
                case EXP_IO_ERROR:          result = "IO_ERROR";           break;
                case EXP_BAD_RECORD_LENGTH: result = "BAD_RECORD_LENGTH";  break;
                case EXP_BAD_CRC:           result = "BAD_CRC";            break;
                case EXP_BAD_RECORD_TYPE:   result = "BAD_RECORD_TYPE";    break;
                case EXP_MEMORY_OVERFLOW:   result = "MEMORY_OVERFLOW";    break;
                case EXP_OUT_OF_RANGE:      result = "OUT_OF_RANGE";       break;
                case EXP_BAD_RECORD_FORMAT: result = "BAD_RECORD_FORMAT";  break;
                case EXP_BAD_RECORD_COUNT:  result = "BAD_RECORD_COUNT";   break;
                case EXP_TEMPLATE_ERROR:    result = "EXP_TEMPLATE_ERROR"; break;
                case EXP_NOT_UNDERSTOOD:    result = "EXP_NOT_UNDERSTOOD"; break;
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

#endif // DATAFILEEXCEPTION_H
