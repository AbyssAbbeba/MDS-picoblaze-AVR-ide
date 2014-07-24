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
 * @ingroup AdaptableSim
 * @file AdaptableSimOperationID.cxx
 */
// =============================================================================

#include "AdaptableSimOperationID.h"

std::ostream & operator << ( std::ostream & out,
                             AdaptableSimOperationID::ID operationID )
{
    using namespace AdaptableSimOperationID;

    switch ( operationID )
    {
        case OP_NONE:          out << "NONE";                  break;
        case OP_ABS_JUMP:      out << "ABS JUMP";              break;
        case OP_ABS_CALL:      out << "ABS CALL";              break;
        case OP_REL_JUMP:      out << "REL CALL";              break;
        case OP_REL_CALL:      out << "REL CALL";              break;
        case OP_OFS_JUMP:      out << "OFS CALL";              break;
        case OP_OFS_CALL:      out << "OFS CALL";              break;
        case OP_IDX_JUMP:      out << "IDX CALL";              break;
        case OP_IDX_CALL:      out << "IDX CALL";              break;
        case OP_RETURN:        out << "RETURN";                break;
        case OP_ISR_RETURN:    out << "ISR RETURN";            break;
        case OP_SET_BANK:      out << "SET BANK";              break;
        case OP_MOVE:          out << "MOVE";                  break;
        case OP_CB_MOVE:       out << "CROSS BANK MOVE";       break;
        case OP_MOVE_BIT:      out << "MOVE BIT";              break;
        case OP_CB_MOVE_BIT:   out << "CROSS BANK MOVE BIT";   break;
        case OP_SWAP:          out << "SWAP";                  break;
        case OP_CB_SWAP:       out << "CROSS BANK SWAP";       break;
        case OP_SWAP_BIT:      out << "SWAP BIT";              break;
        case OP_CB_SWAP_BIT:   out << "CROSS BANK SWAPBIT";    break;
        case OP_CPL:           out << "CPL";                   break;
        case OP_BIT_TEST:      out << "BIT TEST";              break;
        case OP_ADD:           out << "ADD";                   break;
        case OP_SUB:           out << "SUB";                   break;
        case OP_AND:           out << "AND";                   break;
        case OP_OR:            out << "OR";                    break;
        case OP_XOR:           out << "XOR";                   break;
        case OP_SHIFT_LEFT_0:  out << "SHIFT LEFT 0";          break;
        case OP_SHIFT_RIGHT_0: out << "SHIFT RIGHT 0";         break;
        case OP_SHIFT_LEFT_1:  out << "SHIFT LEFT 1";          break;
        case OP_SHIFT_RIGHT_1: out << "SHIFT RIGHT 1";         break;
        case OP_SHIFT_LEFT_R:  out << "SHIFT LEFT R";          break;
        case OP_SHIFT_RIGHT_R: out << "SHIFT RIGHT R";         break;
        case OP_SHIFT_LEFT_C:  out << "SHIFT LEFT C";          break;
        case OP_SHIFT_RIGHT_C: out << "SHIFT RIGHT C";         break;
        case OP_ROTATE_LEFT:   out << "ROTATE LEFT";           break;
        case OP_ROTATE_RIGHT:  out << "ROTATE RIGHT";          break;

        default:
            out << "<ERROR:Unknown!>";
            break;
    }

    return out;
}
