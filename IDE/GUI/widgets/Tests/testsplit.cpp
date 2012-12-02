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
#include "../Editor/baseeditor.h"
#include "../Editor/codeedit.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    CodeEdit *editor = new CodeEdit(0, false, NULL, NULL);

    BaseEditor *a = new BaseEditor(0, NULL, editor, true);
    a->show();

    return app.exec();
    
    
}
