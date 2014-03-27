#include "welcomescr.h"
#include "ui_welcomescr.h"

WelcomeScr::WelcomeScr(QWidget *parent)
    : QWidget(parent)
{
    ui.setupUi(this);

    QPixmap pixLogo(":/resources/icons/logo_big.png");
    ui.lblLogo->setPixmap(pixLogo);

    connect(ui.btnExample,
            SIGNAL(clicked()),
            this,
            SLOT(example())
           );
    connect(ui.btnNewProject,
            SIGNAL(clicked()),
            this,
            SLOT(newProject())
           );
    connect(ui.btnOpenProject,
            SIGNAL(clicked()),
            this,
            SLOT(openProject())
           );
    connect(ui.btnManual,
            SIGNAL(clicked()),
            this,
            SLOT(manual()));
    connect(ui.btnQuickGuide,
            SIGNAL(clicked()),
            this,
            SLOT(quickGuide())
           );
    connect(ui.lstProjects,
            SIGNAL(itemClicked(QListWidgetItem*)),
            this,
            SLOT(listClick(QListWidgetItem*))
           );

}

/*WelcomeScr::~WelcomeScr()
{
    delete ui;
}*/



void WelcomeScr::example()
{

}

void WelcomeScr::openProject()
{

}

void WelcomeScr::newProject()
{

}

void WelcomeScr::manual()
{

}

void WelcomeScr::quickGuide()
{

}

void WelcomeScr::listClick(QListWidgetItem* item)
{

}

/*int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    WelcomeScr w;
    w.show();

    return a.exec();
}*/
