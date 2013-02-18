/**
 * @brief
 * C++ Implementation: ...
 *
 * ...
 *
 * Copyright: See COPYING file that comes with this distribution.
 *
 * @author: Erik Chalupa <xchalu10@stud.fit.vutbr.cz>, (C) 2012
 *
 */



#include <QtGui>
#include "projectdlg.h"


ProjectDialog::ProjectDialog(QMainWindow *dialogParent, ProjectMan *dialogProjectMan)
    : QDialog(dialogParent)
{
    //layout = new QGridLayout(this);
    architecture = new QComboBox(this);
    architecture->addItem("ATmega8A");
    language = new QComboBox(this);
    language->addItem("ASM");
    language->addItem("C");
    projName = new QLineEdit(this);
    projDir = new QLineEdit(this);
    labelName = new QLabel(this);
    labelName->setText("Project name");
    labelDir = new QLabel(this);
    labelDir->setText("Project Directory");
    chooseName = new QPushButton("Browse", this);
    connect(chooseName, SIGNAL(pressed()), this, SLOT(bSetPath()));
    
    buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, Qt::Horizontal, this);

     connect(buttonBox, SIGNAL(accepted()), this, SLOT(bCreate()));
     connect(buttonBox, SIGNAL(rejected()), this, SLOT(bReject()));

    /*layout->addWidget(labelName);
    layout->addWidget(projName);
    layout->addWidget(labelDir);
    layout->addWidget(projDir);
    layout->addWidget(chooseName);
    layout->addWidget(architecture);
    layout->addWidget(buttonBox);*/
    //EDIT THIS!
    labelName->move(0, 0);
    projName->move(0, 20);
    labelDir->move(0, 50);
    projDir->move(0, 70);
    chooseName->move(140, 70);
    chooseName->setMaximumWidth(55);
    architecture->move(0, 100);
    language->move(120, 100);
    buttonBox->move(0, 130);
    this->show();
    
    parent = dialogParent;
    projectMan = dialogProjectMan;
}

void ProjectDialog::bSetPath()
{
    QString path = QFileDialog::getExistingDirectory (this, tr("Project Directory"), "");
    projDir->setText(path);
}

void ProjectDialog::bCreate()
{
     //vytvoreni projektu
     QFile file(projDir->text() + "/" + projName->text() + ".mmp");
     if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
     {
         QMessageBox msgBox;
         if (projName->text().isEmpty()==true)
             msgBox.setText("Enter valid project name");
         else
             msgBox.setText("Error opening file");
         msgBox.exec();
     }
     else {
         //nacteni projektu do manageru otevrenych projektu
         projectMan->addProject(projName->text(), projDir->text() + "/" + projName->text()+ ".mmp", architecture->currentText(), (LangType)language->currentIndex(), &file);
         file.close();

         freeDialog();
     }
}


void ProjectDialog::bReject()
{
    //opravdu nutne? Qt si mozna vsechny deti maze samo...
    freeDialog();
}


void ProjectDialog::freeDialog()
{
    //delete layout;
    delete projName;
    delete projDir;
    delete labelName;
    delete labelDir;
    delete chooseName;
    delete architecture;
    delete language;
    delete buttonBox;
    accept();
}
