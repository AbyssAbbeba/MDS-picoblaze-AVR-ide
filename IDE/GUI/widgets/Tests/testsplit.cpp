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
 * @file testsplit.cpp
 */


#include <QtGui>
#include "../Editor/baseeditor.h"
#include "../Editor/codeedit.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    CodeEdit *editor = new CodeEdit(0, false, NULL, NULL, NULL);

    BaseEditor *a = new BaseEditor(0, NULL, editor, true);
    a->show();

    return app.exec();
    
    
}
