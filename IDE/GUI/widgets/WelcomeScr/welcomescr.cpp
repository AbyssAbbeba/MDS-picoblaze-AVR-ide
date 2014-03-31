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
            SLOT(exampleSlot())
           );
    connect(ui.btnNewProject,
            SIGNAL(clicked()),
            this,
            SLOT(newProjectSlot())
           );
    connect(ui.btnOpenProject,
            SIGNAL(clicked()),
            this,
            SLOT(openProjectSlot())
           );
    connect(ui.btnManual,
            SIGNAL(clicked()),
            this,
            SLOT(manualSlot()));
    connect(ui.btnQuickGuide,
            SIGNAL(clicked()),
            this,
            SLOT(quickGuideSlot())
           );
    connect(ui.lstProjects,
            SIGNAL(itemClicked(QListWidgetItem*)),
            this,
            SLOT(listClickSlot(QListWidgetItem*))
           );

}

/*WelcomeScr::~WelcomeScr()
{
    delete ui;
}*/



void WelcomeScr::exampleSlot()
{
    emit example();
}

void WelcomeScr::openProjectSlot()
{
    emit openProject();
}

void WelcomeScr::newProjectSlot()
{
    emit newProject();
}

void WelcomeScr::manualSlot()
{
    emit manual();
}

void WelcomeScr::quickGuideSlot()
{
    emit quickGuide();
}

void WelcomeScr::listClickSlot(QListWidgetItem* item)
{
    emit recentProject(item->toolTip());
}

/*int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    WelcomeScr w;
    w.show();

    return a.exec();
}*/
