/**
 * @brief
 * C++ Implementation: ...
 *
 * ...
 *
 * (C) copyright 2013, 2014 Moravia Microsystems, s.r.o.
 *
 * @author Erik Chalupa <erik.chalupa@moravia-microsystems.com>
 * @ingroup GUI
 * @file errordlg.cpp
 */


#include "errordlg.h"

/**
 * @brief Shows error according to given code
 * @param errCode Error code
 */
void error(int errCode)
{
    QMessageBox msgBox;
    msgBox.setWindowTitle("Error");
    switch (errCode)
    {
        case ERR_OPENFILE:
        {
            msgBox.setText("Error: Opening file");
            break;
        }
        case ERR_XML_ASSIGN:
        {
            msgBox.setText("Error: Assigning xml file");
            break;
        }
        case ERR_XML_CONTENT:
        {
            msgBox.setText("Error: Wrong content of xml file");
            break;
        }
        case ERR_NO_PROJECT:
        {
            msgBox.setText("Error: No active project");
            break;
        }

        
        //simulation errors
        //stack
        case ERR_STACK_OVERFLOW:
        {
            msgBox.setText("Error: Stack overflow, simulation canceled");
            break;
        }
        case ERR_STACK_UNDERFLOW:
        {
            msgBox.setText("Error: Stack underflow, simulation canceled");
            break;
        }
        //mem
        case ERR_MEM_WR_NONEXISTENT:
        {
            msgBox.setText("Error: Write to nonexistent memory cell, simulation canceled");
            break;
        }
        case ERR_MEM_RD_NONEXISTENT:
        {
            msgBox.setText("Error: Read from nonexistent memory cell, simulation canceled");
            break;
        }
        case ERR_MEM_WR_NOT_IMPLEMENTED:
        {
            msgBox.setText("Error: Write to unimplemented memory cell, simulation canceled");
            break;
        }
        case ERR_MEM_RD_NOT_IMPLEMENTED:
        {
            msgBox.setText("Error: Read from unimplemented memory cell, simulation canceled");
            break;
        }
        //cpu
        case ERR_CPU_PC_OVERFLOW:
        {
            msgBox.setText("Error: Program counter overflow, simulation canceled");
            break;
        }
        case ERR_CPU_PC_UNDERFLOW:
        {
            msgBox.setText("Error: Program counter underflow, simulation canceled");
            break;
        }
        case ERR_CPU_SYS_FATAL:
        {
            msgBox.setText("Error: System fatal error, simulation canceled");
            break;
        }
        case ERR_CPU_INVALID_OPCODE:
        {
            msgBox.setText("Error: Invalid opcode, simulation canceled");
            break;
        }
        case ERR_CPU_INVALID_JUMP:
        {
            msgBox.setText("Error: Invalid jump, simulation canceled");
            break;
        }
        case ERR_CPU_INVALID_CALL:
        {
            msgBox.setText("Error: Invalid call, simulation canceled");
            break;
        }
        case ERR_CPU_INVALID_IRQ:
        {
            msgBox.setText("Error: Invalid IRQ, simulation canceled");
            break;
        }
        case ERR_CPU_INVALID_RET:
        {
            msgBox.setText("Error: Invalid return, simulation canceled");
            break;
        }
        case ERR_CPU_INVALID_RETI:
        {
            msgBox.setText("Error: Invalid interrupt return, simulation canceled");
            break;
        }
        case ERR_CPU_INVALID_OPSET:
        {
            msgBox.setText("Error: Invalid opset, simulation canceled");
            break;
        }
        case ERR_CPU_UNSUPPORTED_INST:
        {
            msgBox.setText("Error: Unsupported instruction, simulation canceled");
            break;
        }
        case ERR_CPU_INST_IGNORED:
        {
            msgBox.setText("Error: Instruction ignored, simulation canceled");
            break;
        }

        
        case ERR_UNTRACKED_PROJECT:
        {
            msgBox.setText("Error: Action not available for untracked project");
            break;
        }
        case ERR_SIM_NOSTART:
        {
            msgBox.setText("Error: Simulation could not start, internal error");
            break;
        }
        case ERR_SIM_NOT_COMPILED:
        {
            msgBox.setText("Error: Simulation could not start, project not compiled");
            break;
        }
        case ERR_SIM_NOT_COMPILED_MODIFIED:
        {
            msgBox.setText("Error: Simulation could not start, source files modified");
            break;
        }
        case ERR_SIM_RECOMPILE_FAILED:
        {
            msgBox.setText("Error: Simulation could not start, recompilation failed");
            break;
        }
        case ERR_NO_MAINFILE:
        {
            msgBox.setText("Error: Project main file not set");
            break;
        }
        default:
        {
            msgBox.setText("Error: Error not specified");
            break;
        }
    }
    msgBox.exec();
}
