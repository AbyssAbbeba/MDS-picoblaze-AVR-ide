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


/**
 * @brief
 * @param
 * @param
 */
ProjectDialog::ProjectDialog(QMainWindow *dialogParent, ProjectMan *dialogProjectMan)
    : QDialog(dialogParent)
{
    //layout = new QGridLayout(this);
#ifndef PICOBLAZE
    architecture = new QComboBox(this);
    architecture->addItem("ATmega8A");
    language = new QComboBox(this);
    language->addItem("ASM");
    language->addItem("C");
#endif
    projName = new QLineEdit(this);
    projDir = new QLineEdit(this);
    labelName = new QLabel(this);
    labelName->setText("Project name");
    labelDir = new QLabel(this);
    labelDir->setText("Project Directory");
    chooseName = new QPushButton("Dir", this);
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
    projName->setMinimumWidth(200);
    labelDir->move(0, 50);
    projDir->move(0, 70);
    projDir->setMinimumWidth(168);
    chooseName->move(170, 70);
    chooseName->setMaximumWidth(30);
#ifndef PICOBLAZE
    architecture->move(0, 100);
    language->move(140, 100);
#endif
    buttonBox->move(100, 130);
    buttonBox->setMaximumWidth(100);
    this->show();
    this->setMaximumWidth(205);
    
    parent = dialogParent;
    projectMan = dialogProjectMan;
}


/**
 * @brief
 */
void ProjectDialog::bSetPath()
{
    QString path = QFileDialog::getExistingDirectory (this, tr("Project Directory"), "");
    projDir->setText(path);
}


/**
 * @brief
 */
void ProjectDialog::bCreate()
{
     //vytvoreni projektu
     QFile file(projDir->text() + "/" + projName->text() + ".mmp");
     if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
     {
         QMessageBox msgBox;
         if (projName->text().isEmpty()==true)
         {
             msgBox.setText("Enter valid project name");
         }
         else
         {
             msgBox.setText("Error opening file");
         }
         msgBox.exec();
     }
     else
     {
         //nacteni projektu do manageru otevrenych projektu
#ifndef PICOBLAZE
         projectMan->addProject(projName->text(), projDir->text() + "/" + projName->text()+ ".mmp", architecture->currentText(), (LangType)language->currentIndex(), &file);
#else
         projectMan->addProject(projName->text(), projDir->text() + "/" + projName->text()+ ".mmp", "PICOBLAZE", LANG_ASM, &file);
#endif
         file.close();

         freeDialog();
     }
}


/**
 * @brief
 */
void ProjectDialog::bReject()
{
    //opravdu nutne? Qt si mozna vsechny deti maze samo...
    freeDialog();
}


/**
 * @brief
 */
void ProjectDialog::freeDialog()
{
    //delete layout;
    delete projName;
    delete projDir;
    delete labelName;
    delete labelDir;
    delete chooseName;
#ifndef PICOBLAZE
    delete architecture;
    delete language;
#endif
    delete buttonBox;
    accept();
}
