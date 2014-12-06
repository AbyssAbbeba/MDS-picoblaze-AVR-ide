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
#include <QMessageBox>

/**
 * @brief Shows error according to given code
 * @param errCode Error code
 */
void error(int errCode, QString detail)
{
    QMessageBox msgBox;
    msgBox.setWindowTitle("Error");
    switch (errCode)
    {
        case ERR_OPENFILE:
        {
            msgBox.setIcon(QMessageBox::Critical);
            msgBox.setText("File not found or cannot be opened\n" + detail);
            break;
        }
        case ERR_XML_ASSIGN:
        {
            msgBox.setIcon(QMessageBox::Critical);
            msgBox.setText("XML file cannot be assigned");
            break;
        }
        case ERR_XML_CONTENT:
        {
            msgBox.setIcon(QMessageBox::Critical);
            msgBox.setText("XML file has wrong content");
            break;
        }
        case ERR_NO_PROJECT:
        {
            msgBox.setIcon(QMessageBox::Critical);
            msgBox.setText("Active project is not set");
            break;
        }


        //simulation errors
        //stack
        case ERR_STACK_OVERFLOW:
        {
            msgBox.setIcon(QMessageBox::Warning);
            msgBox.setText("Stack overflow, simulation aborted");
            break;
        }
        case ERR_STACK_UNDERFLOW:
        {
            msgBox.setIcon(QMessageBox::Warning);
            msgBox.setText("Stack underflow, simulation aborted");
            break;
        }
        //mem
        case ERR_MEM_WR_NONEXISTENT:
        {
            msgBox.setIcon(QMessageBox::Warning);
            msgBox.setText("Write to nonexistent memory cell, simulation aborted");
            break;
        }
        case ERR_MEM_RD_NONEXISTENT:
        {
            msgBox.setIcon(QMessageBox::Warning);
            msgBox.setText("Read from nonexistent memory cell, simulation aborted");
            break;
        }
        case ERR_MEM_WR_NOT_IMPLEMENTED:
        {
            msgBox.setIcon(QMessageBox::Warning);
            msgBox.setText("Write to unimplemented memory cell, simulation aborted");
            break;
        }
        case ERR_MEM_RD_NOT_IMPLEMENTED:
        {
            msgBox.setIcon(QMessageBox::Warning);
            msgBox.setText("Read from unimplemented memory cell, simulation aborted");
            break;
        }
        //cpu
        case ERR_CPU_PC_OVERFLOW:
        {
            msgBox.setIcon(QMessageBox::Warning);
            msgBox.setText("Program counter overflow, simulation aborted");
            break;
        }
        case ERR_CPU_PC_UNDERFLOW:
        {
            msgBox.setIcon(QMessageBox::Warning);
            msgBox.setText("Program counter underflow, simulation aborted");
            break;
        }
        case ERR_CPU_SYS_FATAL:
        {
            msgBox.setIcon(QMessageBox::Critical);
            msgBox.setText("System fatal error, simulation aborted");
            break;
        }
        case ERR_CPU_INVALID_OPCODE:
        {
            msgBox.setIcon(QMessageBox::Warning);
            msgBox.setText("Invalid opcode, simulation aborted");
            break;
        }
        case ERR_CPU_INVALID_JUMP:
        {
            msgBox.setIcon(QMessageBox::Warning);
            msgBox.setText("Invalid jump, simulation aborted");
            break;
        }
        case ERR_CPU_INVALID_CALL:
        {
            msgBox.setIcon(QMessageBox::Warning);
            msgBox.setText("Invalid call, simulation aborted");
            break;
        }
        case ERR_CPU_INVALID_IRQ:
        {
            msgBox.setIcon(QMessageBox::Warning);
            msgBox.setText("Invalid IRQ, simulation aborted");
            break;
        }
        case ERR_CPU_INVALID_RET:
        {
            msgBox.setIcon(QMessageBox::Warning);
            msgBox.setText("Invalid return, simulation aborted");
            break;
        }
        case ERR_CPU_INVALID_RETI:
        {
            msgBox.setIcon(QMessageBox::Warning);
            msgBox.setText("Invalid interrupt return, simulation aborted");
            break;
        }
        case ERR_CPU_INVALID_OPSET:
        {
            msgBox.setIcon(QMessageBox::Warning);
            msgBox.setText("Invalid opset, simulation aborted");
            break;
        }
        case ERR_CPU_UNSUPPORTED_INST:
        {
            msgBox.setIcon(QMessageBox::Warning);
            msgBox.setText("Unsupported instruction, simulation aborted");
            break;
        }
        case ERR_CPU_INST_IGNORED:
        {
            msgBox.setIcon(QMessageBox::Warning);
            msgBox.setText("Instruction ignored, simulation aborted");
            break;
        }


        case ERR_UNTRACKED_PROJECT:
        {
            msgBox.setIcon(QMessageBox::Warning);
            msgBox.setText("Action not available for untracked project");
            break;
        }
        case ERR_SIM_NOSTART:
        {
            msgBox.setIcon(QMessageBox::Critical);
            msgBox.setText("Simulation could not start, try to recompile or check dbg file integrity\n" + detail);
            break;
        }
        case ERR_SIM_NOT_COMPILED:
        {
            msgBox.setIcon(QMessageBox::Critical);
            msgBox.setText("Simulation could not start, project not compiled");
            break;
        }
        case ERR_SIM_NOT_COMPILED_MODIFIED:
        {
            msgBox.setIcon(QMessageBox::Warning);
            msgBox.setText("Simulation could not start, source files modified");
            break;
        }
        case ERR_SIM_RECOMPILE_FAILED:
        {
            msgBox.setIcon(QMessageBox::Critical);
            msgBox.setText("Simulation could not start, recompilation failed");
            break;
        }
        case ERR_NO_MAINFILE:
        {
            msgBox.setIcon(QMessageBox::Warning);
            msgBox.setText("Project main file is not set");
            break;
        }
        case ERR_NO_LICENSE:
        {
            msgBox.setIcon(QMessageBox::Warning);
            msgBox.setText("License file not loaded.");
            break;
        }
        case ERR_COMPILE_DURING_SIMULATION:
        {
            msgBox.setIcon(QMessageBox::Warning);
            msgBox.setText("Can not compile during active simulation.");
            break;
        }
        case ERR_END_OF_DOCUMENT:
        {
            msgBox.setIcon(QMessageBox::Warning);
            msgBox.setText("End of document reached.");
            break;
        }
        case ERR_START_OF_DOCUMENT:
        {
            msgBox.setIcon(QMessageBox::Warning);
            msgBox.setText("Beginning of document reached.");
            break;
        }
        default:
        {
            msgBox.setIcon(QMessageBox::Critical);
            msgBox.setText("Error not specified");
            break;
        }
    }
    msgBox.exec();
}
