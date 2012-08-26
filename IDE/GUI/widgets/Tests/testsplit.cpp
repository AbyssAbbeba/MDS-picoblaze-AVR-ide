#include <QtGui>
#include "../Editor/baseeditor.h"
#include "../Editor/codeedit.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    CodeEdit *editor = new CodeEdit(0, false, NULL, NULL);

    BaseEditor *a = new BaseEditor(0, editor, true);
    a->show();

    return app.exec();
    
    
}
