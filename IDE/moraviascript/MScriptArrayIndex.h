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
 * @ingroup MoraviaScript
 * @file MScriptArrayIndex.h
 */
// =============================================================================

#ifndef MSCRIPTARRAYINDEX_H
#define MSCRIPTARRAYINDEX_H

// Standard header files.
#include <vector>
#include <string>

/**
 * @brief
 * @class MScriptArrayIndex
 * @ingroup MoraviaScript
 */
class MScriptArrayIndex
{
    ////    Public Operations    ////
    public:
        /**
         * @brief
         * @return
         */
        std::string toString() const;

        /**
         * @brief
         * @return 0 == scalar, lower than 0 == associative array, higher than 0 == indexed array.
         */
        int dimensions() const;

        /**
         * @brief
         */
        void clear();

    ////    Public Attributes    ////
    public:
        /// @brief
        std::vector<unsigned int> m_index;

        /// @brief
        std::vector<std::string> m_key;
};

#endif // MSCRIPTARRAYINDEX_H
