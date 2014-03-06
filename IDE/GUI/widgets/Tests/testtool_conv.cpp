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
 * @file testtool_conv.cpp
 */


#include <QtGui>
#include "convertortool.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    ConvertorTool *a = new ConvertorTool(0);

    return app.exec();
    
    
}
