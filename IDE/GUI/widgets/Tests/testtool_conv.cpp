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
#include "convertortool.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    ConvertorTool *a = new ConvertorTool(0);

    return app.exec();
    
    
}
