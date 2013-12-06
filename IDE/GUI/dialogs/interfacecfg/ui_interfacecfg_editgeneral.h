/********************************************************************************
** Form generated from reading UI file 'interfacecfg_editgeneral.ui'
**
** Created: Thu Dec 5 18:47:58 2013
**      by: Qt User Interface Compiler version 4.8.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_INTERFACECFG_EDITGENERAL_H
#define UI_INTERFACECFG_EDITGENERAL_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QCheckBox>
#include <QtGui/QComboBox>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QSpinBox>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_InterfaceCfg_EditGeneral
{
public:
    QLabel *label;
    QSpinBox *spinTabWidth;
    QCheckBox *chckSpaces;
    QLabel *label_2;
    QLabel *label_3;
    QSpinBox *spinSpaces;
    QLabel *label_4;
    QLabel *label_5;
    QLabel *label_6;
    QLabel *label_7;
    QComboBox *comboBox;
    QComboBox *comboBox_2;

    void setupUi(QWidget *InterfaceCfg_EditGeneral)
    {
        if (InterfaceCfg_EditGeneral->objectName().isEmpty())
            InterfaceCfg_EditGeneral->setObjectName(QString::fromUtf8("InterfaceCfg_EditGeneral"));
        InterfaceCfg_EditGeneral->resize(400, 300);
        label = new QLabel(InterfaceCfg_EditGeneral);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(40, 50, 81, 17));
        spinTabWidth = new QSpinBox(InterfaceCfg_EditGeneral);
        spinTabWidth->setObjectName(QString::fromUtf8("spinTabWidth"));
        spinTabWidth->setGeometry(QRect(270, 50, 60, 27));
        spinTabWidth->setMinimum(1);
        spinTabWidth->setMaximum(20);
        spinTabWidth->setValue(4);
        chckSpaces = new QCheckBox(InterfaceCfg_EditGeneral);
        chckSpaces->setObjectName(QString::fromUtf8("chckSpaces"));
        chckSpaces->setGeometry(QRect(270, 80, 97, 22));
        label_2 = new QLabel(InterfaceCfg_EditGeneral);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setGeometry(QRect(40, 80, 201, 17));
        label_3 = new QLabel(InterfaceCfg_EditGeneral);
        label_3->setObjectName(QString::fromUtf8("label_3"));
        label_3->setEnabled(false);
        label_3->setGeometry(QRect(40, 110, 171, 17));
        spinSpaces = new QSpinBox(InterfaceCfg_EditGeneral);
        spinSpaces->setObjectName(QString::fromUtf8("spinSpaces"));
        spinSpaces->setEnabled(false);
        spinSpaces->setGeometry(QRect(270, 110, 60, 27));
        spinSpaces->setMinimum(1);
        spinSpaces->setMaximum(20);
        spinSpaces->setValue(4);
        label_4 = new QLabel(InterfaceCfg_EditGeneral);
        label_4->setObjectName(QString::fromUtf8("label_4"));
        label_4->setGeometry(QRect(20, 20, 66, 17));
        label_5 = new QLabel(InterfaceCfg_EditGeneral);
        label_5->setObjectName(QString::fromUtf8("label_5"));
        label_5->setGeometry(QRect(20, 160, 66, 17));
        label_6 = new QLabel(InterfaceCfg_EditGeneral);
        label_6->setObjectName(QString::fromUtf8("label_6"));
        label_6->setGeometry(QRect(40, 190, 131, 17));
        label_7 = new QLabel(InterfaceCfg_EditGeneral);
        label_7->setObjectName(QString::fromUtf8("label_7"));
        label_7->setGeometry(QRect(40, 220, 111, 17));
        comboBox = new QComboBox(InterfaceCfg_EditGeneral);
        comboBox->setObjectName(QString::fromUtf8("comboBox"));
        comboBox->setGeometry(QRect(270, 220, 78, 27));
        comboBox_2 = new QComboBox(InterfaceCfg_EditGeneral);
        comboBox_2->setObjectName(QString::fromUtf8("comboBox_2"));
        comboBox_2->setGeometry(QRect(270, 190, 78, 27));

        retranslateUi(InterfaceCfg_EditGeneral);

        QMetaObject::connectSlotsByName(InterfaceCfg_EditGeneral);
    } // setupUi

    void retranslateUi(QWidget *InterfaceCfg_EditGeneral)
    {
        InterfaceCfg_EditGeneral->setWindowTitle(QApplication::translate("InterfaceCfg_EditGeneral", "Form", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("InterfaceCfg_EditGeneral", "Tab width", 0, QApplication::UnicodeUTF8));
        chckSpaces->setText(QString());
        label_2->setText(QApplication::translate("InterfaceCfg_EditGeneral", "Insert spaces instead of tabs", 0, QApplication::UnicodeUTF8));
        label_3->setText(QApplication::translate("InterfaceCfg_EditGeneral", "Number of spaces", 0, QApplication::UnicodeUTF8));
        label_4->setText(QApplication::translate("InterfaceCfg_EditGeneral", "Editing", 0, QApplication::UnicodeUTF8));
        label_5->setText(QApplication::translate("InterfaceCfg_EditGeneral", "Files", 0, QApplication::UnicodeUTF8));
        label_6->setText(QApplication::translate("InterfaceCfg_EditGeneral", "Default encoding", 0, QApplication::UnicodeUTF8));
        label_7->setText(QApplication::translate("InterfaceCfg_EditGeneral", "Default EOL", 0, QApplication::UnicodeUTF8));
        comboBox->clear();
        comboBox->insertItems(0, QStringList()
         << QApplication::translate("InterfaceCfg_EditGeneral", "lf", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("InterfaceCfg_EditGeneral", "cr", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("InterfaceCfg_EditGeneral", "crlf", 0, QApplication::UnicodeUTF8)
        );
        comboBox_2->clear();
        comboBox_2->insertItems(0, QStringList()
         << QApplication::translate("InterfaceCfg_EditGeneral", "utf-8", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("InterfaceCfg_EditGeneral", "iso 8859-1", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("InterfaceCfg_EditGeneral", "iso 8859-2", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("InterfaceCfg_EditGeneral", "iso 8859-3", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("InterfaceCfg_EditGeneral", "iso 8859-4", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("InterfaceCfg_EditGeneral", "iso 8859-5", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("InterfaceCfg_EditGeneral", "iso 8859-6", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("InterfaceCfg_EditGeneral", "iso 8859-7", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("InterfaceCfg_EditGeneral", "iso 8859-8", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("InterfaceCfg_EditGeneral", "iso 8859-9", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("InterfaceCfg_EditGeneral", "iso 8859-10", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("InterfaceCfg_EditGeneral", "iso 8859-13", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("InterfaceCfg_EditGeneral", "iso 8859-14", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("InterfaceCfg_EditGeneral", "iso 8859-15", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("InterfaceCfg_EditGeneral", "iso 8859-16", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("InterfaceCfg_EditGeneral", "cp-1250", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("InterfaceCfg_EditGeneral", "cp-1251", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("InterfaceCfg_EditGeneral", "cp-1252", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("InterfaceCfg_EditGeneral", "cp-1253", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("InterfaceCfg_EditGeneral", "cp-1254", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("InterfaceCfg_EditGeneral", "cp-1255", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("InterfaceCfg_EditGeneral", "cp-1256", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("InterfaceCfg_EditGeneral", "cp-1257", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("InterfaceCfg_EditGeneral", "cp-1258", 0, QApplication::UnicodeUTF8)
        );
    } // retranslateUi

};

namespace Ui {
    class InterfaceCfg_EditGeneral: public Ui_InterfaceCfg_EditGeneral {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_INTERFACECFG_EDITGENERAL_H
