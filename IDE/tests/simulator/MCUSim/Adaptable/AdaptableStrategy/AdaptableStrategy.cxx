// =============================================================================
/**
 * @brief
 * C++ Implementation: ...
 *
 * ...
 *
 * (C) copyright 2014 Moravia Microsystems, s.r.o.
 *
 * @author Martin Ošmera <martin.osmera@moravia-microsystems.com>
 * @ingroup testsMCUSimAdaptable
 * @file AdaptableStrategy.cxx
 */
// =============================================================================

#include "AdaptableStrategy.h"

#include "../../../../../simulators/MCUSim/MCUSim.h"
#include "../../../../../simulators/MCUSim/AdaptableSim/AdaptableSimStatusFlagsBase.h"
#include "../../../../../simulators/MCUSim/AdaptableSim/AdaptableSimInterruptController.h"

const std::map<std::string, AdaptableStrategy::FlagID> AdaptableStrategy::PROCESSOR_FLAGS =
{
    { "Z",   FID_Z   }, { "C",   FID_C   }, { "PZ",  FID_PZ  }, { "PC",  FID_PC  },
    { "IE",  FID_IE  }, { "I",   FID_I   }, { "OV",  FID_OV  }, { "N",   FID_N   },
    { "HC",  FID_HC  }, { "P",   FID_P   }, { "F",   FID_F   }, { "POV", FID_POV },
    { "PN",  FID_PN  }, { "PHC", FID_PHC }, { "PP",  FID_PP  }
};

bool AdaptableStrategy::processLine ( std::vector<std::string> * tokens,
                                      bool /*useAsmFile*/,
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
                const auto iter = PROCESSOR_FLAGS.find(tokens->at(2).c_str());
                if ( PROCESSOR_FLAGS.cend() == iter )
                {
                    *m_execMessage = "flag not recognized";
                    cmd->m_type = MCUSimTestScript::CT_ABORT;
                    break;
                }
                else
                {
                    flagID = iter->second;
                }

                cmd->m_args.push_back(flagID);
                cmd->m_args.push_back(MCUSimTestScript::str2bool(tokens->at(6)));
                return true;
            }
            break;
    }

    return false;
}

bool AdaptableStrategy::executeCommand ( const MCUSimTestScript::Command & cmd,
                                         std::ofstream & outFile )
{
    switch ( cmd.m_type )
    {
        case CTE_FLAG_EQ:
        case CTE_FLAG_NE:
        {
            *m_anyAssertionMade = true;

            int flagValue;
            AdaptableSimStatusFlagsBase * flags = dynamic_cast<AdaptableSimStatusFlagsBase*>
                                                  (m_simulator->getSubsys(MCUSimSubsys::ID_FLAGS));

            if ( nullptr == flags )
            {
                outFile << "[ABORTED] ";
                *m_execMessage = "AdaptableSimStatusFlagsBase subsystem not available.";
                return false;
            }

            switch ( cmd.m_args[0] )
            {
                case FID_Z:   flagValue = ( ( true == flags->m_zero         ) ? 1 : 0 ); break;
                case FID_C:   flagValue = ( ( true == flags->m_carry        ) ? 1 : 0 ); break;
                case FID_PZ:  flagValue = ( ( true == flags->m_preZero      ) ? 1 : 0 ); break;
                case FID_PC:  flagValue = ( ( true == flags->m_preCarry     ) ? 1 : 0 ); break;
                case FID_IE:  flagValue = ( ( true == flags->m_inte         ) ? 1 : 0 ); break;
                case FID_I:   flagValue = ( ( flags->m_interrupted > 0      ) ? 1 : 0 ); break;
                case FID_OV:  flagValue = ( ( true == flags->m_overflow     ) ? 1 : 0 ); break;
                case FID_N:   flagValue = ( ( true == flags->m_negative     ) ? 1 : 0 ); break;
                case FID_HC:  flagValue = ( ( true == flags->m_halfCarry    ) ? 1 : 0 ); break;
                case FID_P:   flagValue = ( ( true == flags->m_parity       ) ? 1 : 0 ); break;
                case FID_F:   flagValue = ( ( true == flags->m_flag         ) ? 1 : 0 ); break;
                case FID_POV: flagValue = ( ( true == flags->m_preOverflow  ) ? 1 : 0 ); break;
                case FID_PN:  flagValue = ( ( true == flags->m_preNegative  ) ? 1 : 0 ); break;
                case FID_PHC: flagValue = ( ( true == flags->m_preHalfCarry ) ? 1 : 0 ); break;
                case FID_PP:  flagValue = ( ( true == flags->m_preParity    ) ? 1 : 0 ); break;

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
            AdaptableSimInterruptController * intrCtrl = dynamic_cast<AdaptableSimInterruptController*>
                                                         (m_simulator->getSubsys(MCUSimSubsys::ID_INTERRUPTS));

            if ( nullptr == intrCtrl )
            {
                outFile << "[ABORTED] ";
                *m_execMessage = "AdaptableSimInterruptController subsystem not available.";
                return false;
            }

            intrCtrl->irq();
            outFile << "[OK]      ";
            return true;
        }
    }

    return false;
}
