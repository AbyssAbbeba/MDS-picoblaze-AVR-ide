/**
 * @brief
 * C++ Implementation: ...
 *
 * ...
 *
 * (C) copyright 2013, 2014 Moravia Microsystems, s.r.o.
 *
 * @author Erik Chalupa <erik.chalupa@moravia-microsystems.com>
 * @ingroup GUI
 * @file projectdlg_generalr.cpp
 */


#include <QtGui>
#include <math.h>
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
    QPixmap pm_btnChange(":/resources//icons//drive_magnify.png");
    QIcon icon_btnChange(pm_btnChange);
    ui.btnChange->setIcon(icon_btnChange);

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
    notes.append("64");
    notes.append("31");
    notes.append("1024");

    notes.append("256");
    notes.append("2x16");
    notes.append("256");
    notes.append("31");
    notes.append("4096");


    //ui.teNotes->append("Name:\t\t" + ui.cmbFamily->currentText());
    //ui.teNotes->append("IO port address count:\t" + notes.at(0));
    //ui.teNotes->append("Register count:\t" + notes.at(1));
    //ui.teNotes->append("Scratchpad:\t\t" + notes.at(2));
    //ui.teNotes->append("Callstack:\t\t" + notes.at(3));
    //ui.teNotes->append("Program memory:\t" + notes.at(4));

    

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

    ui.cmbFamily->setCurrentIndex(4);

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

void Projectdlg_General::familyChanged(const QString &text)
{
    if ("kcpsm6" == text)
    {
        emit setHWBuildEnabled(true);
    }
    else
    {
        emit setHWBuildEnabled(false);
    }

    int index = ui.cmbFamily->findText(text);
    ui.teNotes->clear();
    ui.teNotes->append("IO port address count:\t" + notes.at(5*index));
    ui.teNotes->append("Register count:\t" + notes.at(5*index+1));
    ui.teNotes->append("Callstack size:\t" + notes.at(5*index+3));

    if (notes.at(5*index+2) != "-")
    {
        emit setScratchpadMaximum(log2(notes.at(5*index+2).toInt()));
    }
    else
    {
        emit setScratchpadMaximum(-1);
    }

    emit setProgMemMaximum(log2(notes.at(5*index+4).toInt()));
}