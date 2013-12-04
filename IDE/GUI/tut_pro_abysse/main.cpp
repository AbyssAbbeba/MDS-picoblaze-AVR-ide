#include <QtGui>
#include "main.h"

Xxx::Xxx()
    : QWidget(0)
{
    ui.setupUi(this);
    connect(ui.pushButton, SIGNAL(clicked()), this, SLOT(proved()));
    
}

Xxx::~Xxx()
{

}

void Xxx::proved()
{
    ui.lineEdit->setText("aaaaa");
}

void Xxx::funkce()
{

    
}


int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    Xxx xxx;
    xxx.show();
    return app.exec();
}