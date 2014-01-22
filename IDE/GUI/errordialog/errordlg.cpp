/**
 * @brief C++ file for error dialogs
 * C++ Implementation: ...
 *
 * ...
 *
 * Copyright: See COPYING file that comes with this distribution.
 *
 * @author: Erik Chalupa <xchalu10@stud.fit.vutbr.cz>, (C) 2012
 *
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
        case ERR_UNTRACKED_PROJECT:
        {
            msgBox.setText("Error: Action not available for untracked project");
            break;
        }
        case ERR_SIM_NOSTART:
        {
            msgBox.setText("Error: Simulation could not start");
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
