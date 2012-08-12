#include <QtGui>
#include "movgridlayout.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    MovGridLayout *grid = new MovGridLayout(0);

    QLabel *a = new QLabel("this", grid);
    QLabel *b = new QLabel("that", grid);
    QLabel *c = new QLabel("thisandthat", grid);

    grid->addWidget(a);
    grid->addWidget(b);
    grid->addWidget(c);

    grid->show();

    return app.exec();
    
    
}
