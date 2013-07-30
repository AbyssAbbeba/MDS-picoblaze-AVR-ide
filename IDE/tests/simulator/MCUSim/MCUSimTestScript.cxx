// =============================================================================
/**
 * @brief
 * C++ Implementation: ...
 *
 * ...
 *
 * (C) copyright 2013 Moravia Microsystems, s.r.o.
 *
 * @author Martin Ošmera <martin.osmera@moravia-microsystems.com>
 * @ingroup testsMCUSimPicoBlaze
 * @file MCUSimTestScript.cxx
 */
// =============================================================================

#include "MCUSimTestScript.h"

#include "MCUSim.h"

#include <fstream>
#include <cstring>
#include <cctype>
#include <cstdlib>
#include <cstdio>
#include <iostream> // DEBUG

MCUSimTestScript::MCUSimTestScript ( MCUSim * simulator )
                                   : m_simulator ( simulator )
{
}

MCUSimTestScript::~MCUSimTestScript()
{
}

bool MCUSimTestScript::runScript ( const std::string & inFileName,
                                   const std::string & outFileName )
{
    m_success = true;

    std::ifstream inFile  ( inFileName,  std::ios_base::in );
    std::ofstream outFile ( outFileName, std::ios_base::out | std::ios_base::trunc );

    char * command;
    char line[MAX_LINE_LENGTH];

    while ( true == inFile.good() )
    {
        inFile.getline(line, MAX_LINE_LENGTH);

        Command cmd = processLine(line);
        if ( false == executeCommand(cmd, outFile) )
        {
            return false;
        }

        outFile << " :";
        if ( 0 != strlen(line) )
        {
            outFile << " " << line;
        }
        outFile << std::endl;

        if ( false == m_execMessage.empty() )
        {
            outFile << ">>> " << m_execMessage << std::endl;
            m_execMessage.clear();
        }
    }

    return m_success;
}

inline MCUSimTestScript::Command MCUSimTestScript::processLine ( const char * line )
{
    Command result;

    std::vector<std::string> tokens(1);

    size_t len = strlen(line);
    for ( size_t i = 0; i < len; i++ )
    {
        char ch = line[i];

        if ( 0 != isspace(ch) )
        {
            if ( false == tokens.back().empty() )
            {
                tokens.push_back(std::string());
            }
            continue;
        }

        if ( '#' == ch )
        {
            break;
        }

        if ( 0 != isalnum(ch) )
        {
            tokens.back().push_back(ch);
        }
        else
        {
            if ( false == tokens.back().empty() )
            {
                tokens.push_back(std::string(1, ch));
            }
            else
            {
                tokens.back().push_back(ch);
            }
            tokens.push_back(std::string());
        }
    }

    while ( true == tokens.back().empty() )
    {
        tokens.pop_back();

        if ( 0 == tokens.size() )
        {
            break;
        }
    }

    result.m_type = CT_ABORT;
    switch ( tokens.size() )
    {
        case 0:
            result.m_type = CT_EMPTY;
            break;

        case 1:
            if ( "STEP" == tokens[0] )
            {
                result.m_type = CT_STEP;
            }
            break;

        case 4:
            if ( ( "PC" == tokens[0] ) && ( "=" == tokens[2] ) && ( true == checkNumber(tokens[3]) ) )
            {
                if ( "=" == tokens[1] )
                {
                    result.m_type = CT_PC_EQ;
                }
                else if ( "!" == tokens[1] )
                {
                    result.m_type = CT_PC_NE;
                }
                else
                {
                    break;
                }

                result.m_args.push_back(atoi(tokens[3].c_str()));
            }
            break;

        case 7:
            if (
                   ( "[" == tokens[1] )
                       &&
                   ( true == checkNumber(tokens[2]) )
                       &&
                   ( "]" == tokens[3] )
                       &&
                   ( "=" == tokens[5] )
                       &&
                   ( true == checkNumber(tokens[6]) )
               )
            {
                if ( "=" == tokens[4] )
                {
                    result.m_type = CT_EQ;
                }
                else if ( "!" == tokens[4] )
                {
                    result.m_type = CT_NE;
                }
                else
                {
                    break;
                }

                int memSpace;
                if ( 0 == strcmp("CODE", tokens[0].c_str()) )
                {
                    memSpace = MCUSimSubsys::ID_MEM_CODE;
                }
                else if ( 0 == strcmp("DATA", tokens[0].c_str()) )
                {
                    memSpace = MCUSimSubsys::ID_MEM_DATA;
                }
                else if ( 0 == strcmp("EEPROM", tokens[0].c_str()) )
                {
                    memSpace = MCUSimSubsys::ID_MEM_EEPROM;
                }
                else if ( 0 == strcmp("REG", tokens[0].c_str()) )
                {
                    memSpace = MCUSimSubsys::ID_MEM_REGISTERS;
                }
                else if ( 0 == strcmp("STACK", tokens[0].c_str()) )
                {
                    memSpace = MCUSimSubsys::ID_STACK;
                }
                else
                {
                    break;
                }

                result.m_args.push_back(memSpace);
                result.m_args.push_back(atoi(tokens[2].c_str()));
                result.m_args.push_back(atoi(tokens[6].c_str()));
            }
            break;

        default:
            break;
    }

    return result;
}

inline bool MCUSimTestScript::checkNumber ( const std::string & token )
{
    bool result = true;

    if ( 0 == token.size() )
    {
        result = false;
    }
    else
    {
        for ( size_t i = 0; i < token.size(); i++ )
        {
            if ( 0 == isdigit(token[i]) )
            {
                result = false;
            }
        }
    }

    return result;
}

inline bool MCUSimTestScript::executeCommand ( const Command & cmd,
                                               std::ofstream & outFile )
{
    switch ( cmd.m_type )
    {
        case CT_ABORT:
            outFile << "[ABORTED] ";
            m_success = false;
            return false;
        case CT_EMPTY:
            outFile << "          ";
            return true;
        case CT_STEP:
            m_simulator->executeInstruction();
            outFile << "[OK]      ";
            return true;
        case CT_PC_EQ:
        case CT_PC_NE:
        {
            MCUSimSubsys * cpuSubsys = m_simulator->getSubsys ( MCUSimSubsys::ID_CPU );
            if ( NULL == cpuSubsys )
            {
                return false;
            }

            MCUSimCPU * cpu = dynamic_cast<MCUSimCPU*>(cpuSubsys);
            if ( NULL == cpu )
            {
                return false;
            }

            unsigned int pc = cpu->getProgramCounter();
            if ( (unsigned int) cmd.m_args[0] == pc )
            {
                if ( CT_PC_EQ == cmd.m_type )
                {
                    outFile << "[OK]      ";
                }
                else
                {
                    outFile << "[FAILED]  ";
                    m_success = false;

                    char val[10];
                    sprintf(val, "%d", pc);
                    m_execMessage = "actual value is ";
                    m_execMessage += val;
                }
            }
            else
            {
                if ( CT_PC_EQ == cmd.m_type )
                {
                    outFile << "[FAILED]  ";
                    m_success = false;

                    char val[10];
                    sprintf(val, "%d", pc);
                    m_execMessage = "actual value is ";
                    m_execMessage += val;
                }
                else
                {
                    outFile << "[OK]      ";
                }
            }

            return true;
        }
        case CT_EQ:
        case CT_NE:
        {
            MCUSimSubsys * memSubsys = m_simulator->getSubsys ( MCUSimSubsys::SubsysId(cmd.m_args[0]) );
            if ( NULL == memSubsys )
            {
                return false;
            }

            MCUSimMemory * memory = dynamic_cast<MCUSimMemory*>(memSubsys);
            if ( NULL == memory )
            {
                return false;
            }

            int data;
            if ( MCUSim::RC_OK != memory->directRead ( (unsigned int) cmd.m_args[1], (unsigned int&) data ) )
            {
                return false;
            }

            if ( cmd.m_args[2] == data )
            {
                if ( CT_EQ == cmd.m_type )
                {
                    outFile << "[OK]      ";
                }
                else
                {
                    outFile << "[FAILED]  ";
                    m_success = false;

                    char val[10];
                    sprintf(val, "%d", data);
                    m_execMessage = "actual value is ";
                    m_execMessage += val;
                }
            }
            else
            {
                if ( CT_EQ == cmd.m_type )
                {
                    outFile << "[FAILED]  ";
                    m_success = false;

                    char val[10];
                    sprintf(val, "%d", data);
                    m_execMessage = "actual value is ";
                    m_execMessage += val;
                }
                else
                {
                    outFile << "[OK]      ";
                }
            }

            return true;
        }
    }

    return false;
}
