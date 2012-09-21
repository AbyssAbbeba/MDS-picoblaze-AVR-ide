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



#include <QtGui>
#include "mainform/mainform.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    MainForm MainGUI;
    MainGUI.showMaximized();
    
    return app.exec();
    
    
}
