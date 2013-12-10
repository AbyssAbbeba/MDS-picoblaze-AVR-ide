// =============================================================================
/**
 * @brief
 * C++ Implementation: ...
 *
 * ...
 *
 * (C) copyright 2013, 2014 Moravia Microsystems, s.r.o.
 *
 * @author Martin Ošmera <martin.osmera@moravia-microsystems.com>
 * @ingroup testsMScript
 * @file MScriptTestStrategy.cxx
 */
// =============================================================================

#include "MScriptTestStrategy.h"

void MScriptTestStrategy::functionCalled ( int id,
                                           const std::vector<MScriptValue> & arguments,
                                           MScriptValue * returnValue )
{
}

MScriptValue * MScriptTestStrategy::variableRead ( int id,
                                                   const MScriptArrayIndex * index )
{
}

void MScriptTestStrategy::variableWritten ( int id,
                                            const MScriptArrayIndex * index,
                                            const MScriptValue & value )
{
}
