/**
 * @brief
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

void error(int errCode)
{
    QMessageBox msgBox;
    switch (errCode)
    {
        case ERR_OPENFILE:
            msgBox.setText("Error opening file");
            break;
        case ERR_XML_CONTENT:
            msgBox.setText("Error: Assigning xml file");
            break;
        case ERR_XML_ASSIGN:
            msgBox.setText("Error: Wrong xml file");
            break;
    }
    msgBox.exec();
}
