#include <QtGui>
#include "mainform/mainform.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    MainForm MainGUI;
    MainGUI.showMaximized();
    
    return app.exec();
    
    
}
