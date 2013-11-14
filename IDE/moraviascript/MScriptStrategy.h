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
 * @ingroup MoraviaScript
 * @file MScriptStrategy.h
 */
// =============================================================================

#ifndef MSCRIPTSTRATEGY_H
#define MSCRIPTSTRATEGY_H

// MScript language interpreter header files.
#include "MScriptValue.h"
#include "MScriptArrayIndex.h"
#include "MScriptStrategyInterface.h"

// Standard header files.
#include <vector>
#include <string>

/**
 * @brief
 * @class MScriptStrategy
 * @ingroup MoraviaScript
 */
class MScriptStrategy
{
    ////    Public Operations    ////
    public:
        /// @name Operations related to script functions.
        //@{
            /**
             * @brief
             * @param[in] ns
             * @param[in] name
             * @param[in] params
             * @param[in] defaults
             * @return
             */
            int newFunction ( const std::string & ns,
                              const std::string & name,
                              unsigned int params,
                              const std::vector * defaults = NULL );

            /**
             * @brief
             * @param[in] id
             * @return
             */
            bool deleteFunction ( int id );

            /**
             * @brief
             * @param[in] id
             * @param[in] arguments
             * @param[out] returnValue
             * @return
             */
            virtual void functionCalled ( int id,
                                          const std::vector<MScriptValue> & arguments,
                                          MScriptValue * returnValue ) = 0;
        //@}

        /// @name Operations related to script variables.
        //@{
            /**
             * @brief
             * @param[in] ns
             * @param[in] name
             * @param[in] params
             * @param[in] defaults
             * @return
             */
            int newVariable ( const std::string & ns,
                              const std::string & name );

            /**
             * @brief
             * @param[in] id
             * @return
             */
            bool deleteVariable ( int id );

            /**
             * @brief
             * @param[in] id
             * @param[in] index
             * @param[out] value
             */
            virtual void variableRead ( int id,
                                        const MScriptArrayIndex & index,
                                        MScriptValue * value ) = 0;

            /**
             * @brief
             * @param[in] id
             * @param[in] index
             * @param[in] value
             */
            virtual void variableWritten ( int id,
                                          const MScriptArrayIndex & index,
                                          const MScriptValue & value ) = 0;
        //@}

                                          MScriptStrategyInterface
};

#endif // MSCRIPTSTRATEGY_H
