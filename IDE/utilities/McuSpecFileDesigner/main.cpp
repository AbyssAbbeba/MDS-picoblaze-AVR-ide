#include <QtGui>
#include "mcuspecfiledesigner.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    McuSpecFileDesigner *designer = new McuSpecFileDesigner(0);

    return app.exec();
    
    
}
