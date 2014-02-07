/**
 * @brief
 * C++ Implementation: ...
 *
 * ...
 *
 * Copyright: See COPYING file that comes with this distribution.
 *
 * @author: Erik Chalupa <xchalu10@stud.fit.vutbr.cz>, (C) 2014
 *
 */



#include <QtGui>
#include "projectdlg_general.h"


/**
 * @brief
 * @param
 * @param
 */
Projectdlg_General::Projectdlg_General(QWidget *parent)
    : QWidget(parent)
{
    //layout = new QGridLayout(this);
    ui.setupUi(this);

    connect(ui.btnChange,
            SIGNAL(pressed()),
            this,
            SLOT(setPath())
           );
    connect(ui.cmbFamily,
            SIGNAL(currentIndexChanged(const QString&)),
            this,
            SLOT(familyChanged(const QString&))
           );

    this->show();
}


/**
 * @brief
 */
void Projectdlg_General::setPath()
{
    QString path = QFileDialog::getExistingDirectory (this, tr("Project Directory"), "");
    ui.lePath->setText(path);
}


QString Projectdlg_General::getPath()
{
    return ui.lePath->text();
}


QString Projectdlg_General::getName()
{
    return ui.leName->text();
}


QString Projectdlg_General::getArch()
{
    return ui.cmbArch->currentText();
}


QString Projectdlg_General::getFamily()
{
    return ui.cmbFamily->currentText();
}


QString Projectdlg_General::getIntVector()
{
    return ui.leIntVector->text();
}


QString Projectdlg_General::getHWBuild()
{
    return ui.leHWBuild->text();
}


void Projectdlg_General::familyChanged(const QString &text)
{
    if ("kcpsm6" == text)
    {
        ui.lblHWBuild->setEnabled(true);
        ui.leHWBuild->setEnabled(true);
    }
    else
    {
        ui.lblHWBuild->setDisabled(true);
        ui.leHWBuild->setDisabled(true);
    }
}