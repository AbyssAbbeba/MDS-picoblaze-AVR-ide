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
#include "displaytool.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    DisplayTool *a = new DisplayTool(0);

    return app.exec();
    
    
}
