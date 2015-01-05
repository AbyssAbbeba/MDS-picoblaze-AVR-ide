/**
 * @brief
 * C++ Implementation: ...
 *
 * ...
 *
 * (C) copyright 2013, 2014 Moravia Microsystems, s.r.o.
 *
 * @author Erik Chalupa <erik.chalupa@moravia-microsystems.com>
 * @ingroup InterfaceCfg_ExternalApps
 * @file interfacecfg_externalapps.cpp
 */


#include <QtGui>
#include "interfacecfg_externalapps.h"
#include "../../guicfg/guicfg.h"


InterfaceCfg_ExternalApps::InterfaceCfg_ExternalApps(QWidget *parent)
    : QWidget(parent)
{
    ui.setupUi(this);


    QList<GuiCfg_Items::ExternalApp> apps = GuiCfg::getInstance().getExternalApps();

    if (apps.count() == 3)
    {
        ui.lePath1->setText(apps.at(0).path);
        ui.leArgs1->setText(apps.at(0).args);
        ui.chckToolBar1->setChecked(apps.at(0).toolBar);

        ui.lePath2->setText(apps.at(1).path);
        ui.leArgs2->setText(apps.at(1).args);
        ui.chckToolBar2->setChecked(apps.at(1).toolBar);

        ui.lePath3->setText(apps.at(2).path);
        ui.leArgs3->setText(apps.at(2).args);
        ui.chckToolBar3->setChecked(apps.at(2).toolBar);
    }
    
    connect(ui.btnFind1, SIGNAL(clicked()), this, SLOT(setPath1()));
    connect(ui.btnFind2, SIGNAL(clicked()), this, SLOT(setPath2()));
    connect(ui.btnFind3, SIGNAL(clicked()), this, SLOT(setPath3()));
}


void InterfaceCfg_ExternalApps::setPath1()
{
    QString path = QFileDialog::getOpenFileName(this, "Application path", m_path);
    if (NULL != path)
    {
        ui.lePath1->setText(path);
    }
}


void InterfaceCfg_ExternalApps::setPath2()
{
    QString path = QFileDialog::getOpenFileName(this, "Application path", m_path);
    if (NULL != path)
    {
        ui.lePath2->setText(path);
    }
}


void InterfaceCfg_ExternalApps::setPath3()
{
    QString path = QFileDialog::getOpenFileName(this, "Application path", m_path);
    if (NULL != path)
    {
        ui.lePath3->setText(path);
    }
}


void InterfaceCfg_ExternalApps::save()
{
    QList<GuiCfg_Items::ExternalApp> apps;
    GuiCfg_Items::ExternalApp tempApp;
    
    tempApp.path = ui.lePath1->text();
    tempApp.args = ui.leArgs1->text();
    tempApp.toolBar = ui.chckToolBar1->isChecked();
    apps.append(tempApp);

    tempApp.path = ui.lePath2->text();
    tempApp.args = ui.leArgs2->text();
    tempApp.toolBar = ui.chckToolBar2->isChecked();
    apps.append(tempApp);

    tempApp.path = ui.lePath3->text();
    tempApp.args = ui.leArgs3->text();
    tempApp.toolBar = ui.chckToolBar3->isChecked();
    apps.append(tempApp);
    
    GuiCfg::getInstance().setExternalApps(apps);
}


void InterfaceCfg_ExternalApps::setDialogPath(QString path)
{
    m_path = path;
}