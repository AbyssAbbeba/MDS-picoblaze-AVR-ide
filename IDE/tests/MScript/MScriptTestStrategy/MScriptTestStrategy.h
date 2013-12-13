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
 * @ingroup testsMScript
 * @file MScriptTestStrategy.h
 */
// =============================================================================

#ifndef MSCRIPTTESTSTRATEGY_H
#define MSCRIPTTESTSTRATEGY_H

// MScript language interpreter header files.
#include "MScriptStrategy.h"

/**
 * @brief
 * @class MScriptTestStrategy
 * @ingroup MoraviaScript
 */
class MScriptTestStrategy : public MScriptStrategy
{
    ////    Public Operations    ////
    public:
        /**
         * @brief
         * @param[in] id
         * @param[in] arguments
         * @param[out] returnValue
         * @return
         */
        virtual void functionCalled ( int id,
                                      const std::vector<MScriptValue> & arguments,
                                      MScriptValue * returnValue );

        /**
         * @brief
         * @param[in] id
         * @param[in] index
         * @return
         */
        virtual MScriptValue * variableRead ( int id,
                                              const MScriptArrayIndex * index );

        /**
         * @brief
         * @param[in] id
         * @param[in] index
         * @param[in] value
         */
        virtual void variableWritten ( int id,
                                       const MScriptArrayIndex * index,
                                       const MScriptValue & value );
};

#endif // MSCRIPTTESTSTRATEGY_H
