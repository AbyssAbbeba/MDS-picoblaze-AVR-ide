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

#include <QApplication>

#include "AdjSimProcDefGui.h"

int main(int argc, char** argv)
{
    QApplication app(argc, argv);
    AdjSimProcDefGui gui;
    gui.show();
    return app.exec();
}
