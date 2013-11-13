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

/**
 * @brief
 * @class MScriptStrategy
 * @ingroup MoraviaScript
 */
class MScriptStrategy
{
    ////    Public Operations    ////
    public:
        bool newFunction ( const std::string & ns,
                           const std::string & name,
                           unsigned int params );
};

#endif // MSCRIPTSTRATEGY_H
