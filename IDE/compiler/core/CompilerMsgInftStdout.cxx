// =============================================================================
/**
 * @brief
 * C++ Implementation: ...
 *
 * ...
 *
 * (C) copyright 2013 Moravia Microsystems, s.r.o.
 *
 * @author Martin Ošmera <martin.osmera@gmail.com>
 * @ingroup Compiler
 * @file CompilerMsgInftStdout.cxx
 */
// =============================================================================

#include "CompilerMsgInftStdout.h"
#include <iostream>

void CompilerMsgInftStdout::message ( const std::string & text,
                                      CompilerBase::MessageType /*type*/ )
{
// NOTE: For colorful output, color codes compatible with vt100 terminal.
//     switch ( type )
//     {
//         case CompilerBase::MT_GENERAL:
//             std::cout << "\033[34;1m" << text << "\033[m\n";
//             break;
//         case CompilerBase::MT_ERROR:
//             std::cout << "\033[31;1m" << text << "\033[m\n";
//             break;
//         case CompilerBase::MT_WARNING:
//             std::cout << "\033[33;1m" << text << "\033[m\n";
//             break;
//         case CompilerBase::MT_REMARK:
//             std::cout << "\033[32;1m" << text << "\033[m\n";
//             break;
//     }

    std::cout << text << std::endl;
}
