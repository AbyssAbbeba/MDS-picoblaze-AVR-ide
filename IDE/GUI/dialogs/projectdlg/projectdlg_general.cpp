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
    QPixmap *pm_btnChange = new QPixmap(":/resources//icons//drive_magnify.png");
    QIcon *icon_btnChange = new QIcon(*pm_btnChange);
    ui.btnChange->setIcon(*icon_btnChange);

    notes.append("256");
    notes.append("8");
    notes.append("-");
    notes.append("4");
    notes.append("256");

    notes.append("256");
    notes.append("16");
    notes.append("-");
    notes.append("15");
    notes.append("256");

    notes.append("256");
    notes.append("32");
    notes.append("-");
    notes.append("31");
    notes.append("1024");

    notes.append("256");
    notes.append("16");
    notes.append("64 Bytes");
    notes.append("31");
    notes.append("1024");

    notes.append("256");
    notes.append("2x16");
    notes.append("64/128/256 Bytes");
    notes.append("31");
    notes.append("1024/2048/4096");


    ui.teNotes->append("Name:\t\t" + ui.cmbFamily->currentText());
    ui.teNotes->append("IO port address count:\t" + notes.at(0));
    ui.teNotes->append("Register count:\t" + notes.at(1));
    ui.teNotes->append("Scratchpad:\t\t" + notes.at(2));
    ui.teNotes->append("Callstack size:\t" + notes.at(3));
    ui.teNotes->append("Instruction count:\t" + notes.at(4));

    

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

    int index = ui.cmbFamily->findText(text);
    ui.teNotes->clear();
    ui.teNotes->append("Name:\t\t" + text);
    ui.teNotes->append("IO port address count:\t" + notes.at(5*index));
    ui.teNotes->append("Register count:\t" + notes.at(5*index+1));
    ui.teNotes->append("Scratchpad:\t\t" + notes.at(5*index+2));
    ui.teNotes->append("Callstack size:\t" + notes.at(5*index+3));
    ui.teNotes->append("Instruction count:\t" + notes.at(5*index+4));
}