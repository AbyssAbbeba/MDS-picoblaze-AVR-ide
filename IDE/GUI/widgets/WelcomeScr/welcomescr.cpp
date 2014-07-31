#include "welcomescr.h"
#include "ui_welcomescr.h"
#include "../../guicfg/guicfg.h"
#include <QtGui>

WelcomeScr::WelcomeScr(QWidget *parent)
    : QWidget(parent)
{
    ui.setupUi(this);

    QPixmap pixLogo(":/resources/icons/logo_big.png");
    ui.lblLogo->setPixmap(pixLogo);

    QList<QString> recent = GuiCfg::getInstance().getRecentProjects();

    for (int i = 0; i < recent.count(); i++)
    {
        ui.lstProjects->insertItem(0,recent.at(i));
    }

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
    emit recentProject(item->text());
}

/*int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    WelcomeScr w;
    w.show();

    return a.exec();
}*/
