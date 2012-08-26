#include <QtGui>
#include "movgridlayout.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    MovGridLayout *grid = new MovGridLayout(0);

    grid->show();

    QLabel *a = new QLabel("this", grid);
    QLabel *b = new QLabel("that", grid);
    QLabel *c = new QLabel("thisandthat", grid);

    a->show();
    b->show();
    c->show();

    grid->addWidget(a);
    grid->addWidget(b);
    grid->addWidget(c);

    return app.exec();
    
    
}
