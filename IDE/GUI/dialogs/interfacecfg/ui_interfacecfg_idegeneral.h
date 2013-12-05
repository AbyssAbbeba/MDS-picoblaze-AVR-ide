/********************************************************************************
** Form generated from reading UI file 'interfacecfg_idegeneral.ui'
**
** Created: Thu Dec 5 18:47:58 2013
**      by: Qt User Interface Compiler version 4.8.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_INTERFACECFG_IDEGENERAL_H
#define UI_INTERFACECFG_IDEGENERAL_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QCheckBox>
#include <QtGui/QComboBox>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_InterfaceCfg_IDEGeneral
{
public:
    QLabel *label;
    QComboBox *cmbLanguage;
    QLabel *label_2;
    QLabel *label_3;
    QCheckBox *chckTips;
    QCheckBox *chckSplash;

    void setupUi(QWidget *InterfaceCfg_IDEGeneral)
    {
        if (InterfaceCfg_IDEGeneral->objectName().isEmpty())
            InterfaceCfg_IDEGeneral->setObjectName(QString::fromUtf8("InterfaceCfg_IDEGeneral"));
        InterfaceCfg_IDEGeneral->resize(400, 300);
        label = new QLabel(InterfaceCfg_IDEGeneral);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(20, 80, 81, 17));
        cmbLanguage = new QComboBox(InterfaceCfg_IDEGeneral);
        cmbLanguage->setObjectName(QString::fromUtf8("cmbLanguage"));
        cmbLanguage->setGeometry(QRect(210, 80, 78, 27));
        label_2 = new QLabel(InterfaceCfg_IDEGeneral);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setGeometry(QRect(20, 50, 161, 17));
        label_3 = new QLabel(InterfaceCfg_IDEGeneral);
        label_3->setObjectName(QString::fromUtf8("label_3"));
        label_3->setGeometry(QRect(20, 20, 191, 17));
        chckTips = new QCheckBox(InterfaceCfg_IDEGeneral);
        chckTips->setObjectName(QString::fromUtf8("chckTips"));
        chckTips->setGeometry(QRect(210, 50, 97, 22));
        chckSplash = new QCheckBox(InterfaceCfg_IDEGeneral);
        chckSplash->setObjectName(QString::fromUtf8("chckSplash"));
        chckSplash->setGeometry(QRect(210, 20, 97, 22));

        retranslateUi(InterfaceCfg_IDEGeneral);

        QMetaObject::connectSlotsByName(InterfaceCfg_IDEGeneral);
    } // setupUi

    void retranslateUi(QWidget *InterfaceCfg_IDEGeneral)
    {
        InterfaceCfg_IDEGeneral->setWindowTitle(QApplication::translate("InterfaceCfg_IDEGeneral", "Form", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("InterfaceCfg_IDEGeneral", "Language", 0, QApplication::UnicodeUTF8));
        cmbLanguage->clear();
        cmbLanguage->insertItems(0, QStringList()
         << QApplication::translate("InterfaceCfg_IDEGeneral", "English", 0, QApplication::UnicodeUTF8)
        );
        label_2->setText(QApplication::translate("InterfaceCfg_IDEGeneral", "Display tips on start-up", 0, QApplication::UnicodeUTF8));
        label_3->setText(QApplication::translate("InterfaceCfg_IDEGeneral", "Display splash screen", 0, QApplication::UnicodeUTF8));
        chckTips->setText(QString());
        chckSplash->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class InterfaceCfg_IDEGeneral: public Ui_InterfaceCfg_IDEGeneral {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_INTERFACECFG_IDEGENERAL_H
