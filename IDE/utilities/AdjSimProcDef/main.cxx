// =============================================================================
/**
 * @brief
 * C++ Implementation: ...
 *
 * ...
 *
 * (C) copyright 2014 Moravia Microsystems, s.r.o.
 *
 * @author Martin Ošmera <martin.osmera@moravia-microsystems.com>
 * @ingroup AdjSimProcDef
 * @file main.cxx
 */
// =============================================================================

#include <iostream>

#include <QString>
#include <QObject>
#include <QApplication>

#include "AdjSimProcDefGui.h"

int main ( int argc, char ** argv )
{
    QApplication app(argc, argv);
    AdjSimProcDefGui gui;

    if ( 2 == argc )
    {
        gui.openFile(QString::fromStdString(argv[1]));
    }
    else if ( argc > 2 )
    {
        std::cerr << QObject::tr("Invalid arguments: expecing a single file name, or no arguments.").toStdString()
                  << std::endl;
        return 2;
    }

    gui.show();
    return app.exec();
}
