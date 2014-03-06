/**
 * @brief
 * C++ Implementation: ...
 *
 * ...
 *
 * (C) copyright 2013, 2014 Moravia Microsystems, s.r.o.
 *
 * @author Erik Chalupa <erik.chalupa@moravia-microsystems.com>
 * @ingroup TestSrc
 * @file testtool_display.cpp
 */


#include <QtGui>
#include "displaytool.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    DisplayTool *a = new DisplayTool(0);

    return app.exec();
    
    
}
