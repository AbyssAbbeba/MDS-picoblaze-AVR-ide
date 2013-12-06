/********************************************************************************
** Form generated from reading UI file 'interfacecfg_editfonts.ui'
**
** Created: Thu Dec 5 18:47:58 2013
**      by: Qt User Interface Compiler version 4.8.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_INTERFACECFG_EDITFONTS_H
#define UI_INTERFACECFG_EDITFONTS_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QListWidget>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_InterfaceCfg_EditFonts
{
public:
    QLabel *label;
    QLabel *label_2;
    QListWidget *listFonts;
    QListWidget *listSize;
    QLabel *lblSample;

    void setupUi(QWidget *InterfaceCfg_EditFonts)
    {
        if (InterfaceCfg_EditFonts->objectName().isEmpty())
            InterfaceCfg_EditFonts->setObjectName(QString::fromUtf8("InterfaceCfg_EditFonts"));
        InterfaceCfg_EditFonts->resize(400, 300);
        label = new QLabel(InterfaceCfg_EditFonts);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(20, 20, 66, 17));
        label_2 = new QLabel(InterfaceCfg_EditFonts);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setGeometry(QRect(280, 20, 66, 17));
        listFonts = new QListWidget(InterfaceCfg_EditFonts);
        listFonts->setObjectName(QString::fromUtf8("listFonts"));
        listFonts->setGeometry(QRect(20, 50, 231, 161));
        listSize = new QListWidget(InterfaceCfg_EditFonts);
        listSize->setObjectName(QString::fromUtf8("listSize"));
        listSize->setGeometry(QRect(280, 50, 91, 161));
        lblSample = new QLabel(InterfaceCfg_EditFonts);
        lblSample->setObjectName(QString::fromUtf8("lblSample"));
        lblSample->setGeometry(QRect(20, 230, 351, 51));

        retranslateUi(InterfaceCfg_EditFonts);

        QMetaObject::connectSlotsByName(InterfaceCfg_EditFonts);
    } // setupUi

    void retranslateUi(QWidget *InterfaceCfg_EditFonts)
    {
        InterfaceCfg_EditFonts->setWindowTitle(QApplication::translate("InterfaceCfg_EditFonts", "Form", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("InterfaceCfg_EditFonts", "Font", 0, QApplication::UnicodeUTF8));
        label_2->setText(QApplication::translate("InterfaceCfg_EditFonts", "Size", 0, QApplication::UnicodeUTF8));
        lblSample->setText(QApplication::translate("InterfaceCfg_EditFonts", "The Quick Brown Fox Jumps Over A Lazy Dog", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class InterfaceCfg_EditFonts: public Ui_InterfaceCfg_EditFonts {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_INTERFACECFG_EDITFONTS_H
