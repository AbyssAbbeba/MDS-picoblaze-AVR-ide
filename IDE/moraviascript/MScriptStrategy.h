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
 * @file MScriptStrategy.h
 */
// =============================================================================

#ifndef MSCRIPTSTRATEGY_H
#define MSCRIPTSTRATEGY_H

// MScript language interpreter header files.
#include "MScriptBase.h"
#include "MScriptValue.h"
#include "MScriptIdManager.h"
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
                              const std::vector<std::string> & params,
                              const std::vector<MScriptValue> * defaults = NULL );

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
             * @return
             */
            virtual MScriptValue * variableRead ( int id,
                                                  const MScriptArrayIndex * index ) = 0;

            /**
             * @brief
             * @param[in] id
             * @param[in] index
             * @param[in] value
             */
            virtual void variableWritten ( int id,
                                           const MScriptArrayIndex * index,
                                           const MScriptValue & value ) = 0;
        //@}

        /// @name Miscellaneous operations.
        //@{
            /**
             * @brief
             * @param[in] type
             * @param[in] text
             */
            void message ( MScriptBase::MessageType type,
                           const std::string & text );
        //@}

    ////    Public Attributes    ////
    public:
        /// @brief
        MScriptStrategyInterface * m_core;

    ////    Private Attributes    ////
    private:
        /// @brief
        MScriptIdManager m_funcId;

        /// @brief
        MScriptIdManager m_varId;
};

#endif // MSCRIPTSTRATEGY_H
