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
 * @ingroup testsMCUSimPicoBlaze
 * @file PicoBlazeStrategy.cxx
 */
// =============================================================================

#include "PicoBlazeStrategy.h"

#include "../../../../simulators/MCUSim/MCUSim.h"
#include "../../../../simulators/MCUSim/PicoBlaze/PicoBlazeStatusFlagsBase.h"
#include "../../../../simulators/MCUSim/PicoBlaze/PicoBlazeInterruptController.h"

bool PicoBlazeStrategy::processLine ( std::vector<std::string> * tokens,
                                      bool useAsmFile,
                                      MCUSimTestScript::Command * cmd )
{
    switch ( tokens->size() )
    {
        case 1:
            if ( "INTERRUPT" == tokens->at(0) )
            {
                cmd->m_type = CTE_INTERRUPT;
                return true;
            }
            break;

        case 7:
            if (
                   ( "[" == tokens->at(1) )
                       &&
                   ( "]" == tokens->at(3) )
                       &&
                   ( "=" == tokens->at(5) )
                       &&
                   ( true == MCUSimTestScript::checkBool(tokens->at(6)) )
               )
            {
                if ( "=" == tokens->at(4) )
                {
                    cmd->m_type = CTE_FLAG_EQ;
                }
                else if ( "!" == tokens->at(4) )
                {
                    cmd->m_type = CTE_FLAG_NE;
                }
                else
                {
                    break;
                }

                if ( 0 != strcmp("FLAG", tokens->at(0).c_str()) )
                {
                    break;
                }

                std::transform(tokens->at(2).begin(), tokens->at(2).end(), tokens->at(2).begin(), ::toupper);

                int flagID;
                if ( 0 == strcmp("Z", tokens->at(2).c_str()) )
                {
                    flagID = FID_Z;
                }
                else if ( 0 == strcmp("C", tokens->at(2).c_str()) )
                {
                    flagID = FID_C;
                }
                else if ( 0 == strcmp("PZ", tokens->at(2).c_str()) )
                {
                    flagID = FID_PZ;
                }
                else if ( 0 == strcmp("PC", tokens->at(2).c_str()) )
                {
                    flagID = FID_PC;
                }
                else if ( 0 == strcmp("IE", tokens->at(2).c_str()) )
                {
                    flagID = FID_IE;
                }
                else if ( 0 == strcmp("I", tokens->at(2).c_str()) )
                {
                    flagID = FID_I;
                }
                else
                {
                    *m_execMessage = "flag not recognized";
                    cmd->m_type = MCUSimTestScript::CT_ABORT;
                    break;
                }

                cmd->m_args.push_back(flagID);
                cmd->m_args.push_back(MCUSimTestScript::str2bool(tokens->at(6)));
                return true;
            }
            break;
    }

    return false;
}

bool PicoBlazeStrategy::executeCommand ( const MCUSimTestScript::Command & cmd,
                                         std::ofstream & outFile )
{
    switch ( cmd.m_type )
    {
        case CTE_FLAG_EQ:
        case CTE_FLAG_NE:
        {
            int flagValue;
            PicoBlazeStatusFlagsBase * flags = dynamic_cast<PicoBlazeStatusFlagsBase*>
                                               (m_simulator->getSubsys(MCUSimSubsys::ID_FLAGS));

            if ( NULL == flags )
            {
                outFile << "[ABORTED] ";
                *m_execMessage = "PicoBlazeStatusFlagsBase subsystem not available.";
                return false;
            }

            switch ( cmd.m_args[0] )
            {
                case FID_Z:  flagValue = ( ( true == flags->m_zero     ) ? 1 : 0 ); break;
                case FID_C:  flagValue = ( ( true == flags->m_carry    ) ? 1 : 0 ); break;
                case FID_PZ: flagValue = ( ( true == flags->m_preZero  ) ? 1 : 0 ); break;
                case FID_PC: flagValue = ( ( true == flags->m_preCarry ) ? 1 : 0 ); break;
                case FID_IE: flagValue = ( ( true == flags->m_inte     ) ? 1 : 0 ); break;
                case FID_I:  flagValue = ( ( flags->m_interrupted > 0  ) ? 1 : 0 ); break;

                default:
                    return false;
            }

            if ( cmd.m_args[1] == flagValue )
            {
                if ( CTE_FLAG_EQ != cmd.m_type )
                {
                    outFile << "[FAILED]  ";
                    *m_success = false;
                }
                else
                {
                    outFile << "[OK]      ";
                }
            }
            else
            {
                if ( CTE_FLAG_NE != cmd.m_type )
                {
                    outFile << "[FAILED]  ";
                    *m_success = false;
                }
                else
                {
                    outFile << "[OK]      ";
                }
            }

            return true;
        }

        case CTE_INTERRUPT:
        {
            PicoBlazeInterruptController * intrCtrl = dynamic_cast<PicoBlazeInterruptController*>
                                                      (m_simulator->getSubsys(MCUSimSubsys::ID_INTERRUPTS));

            if ( NULL == intrCtrl )
            {
                outFile << "[ABORTED] ";
                *m_execMessage = "PicoBlazeInterruptController subsystem not available.";
                return false;
            }

            intrCtrl->irq();
            outFile << "[OK]      ";
            return true;
        }
    }

    return false;
}
