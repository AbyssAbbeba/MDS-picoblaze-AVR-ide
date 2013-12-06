/********************************************************************************
** Form generated from reading UI file 'interfacecfg_simothers.ui'
**
** Created: Thu Dec 5 18:47:58 2013
**      by: Qt User Interface Compiler version 4.8.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_INTERFACECFG_SIMOTHERS_H
#define UI_INTERFACECFG_SIMOTHERS_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QPushButton>
#include <QtGui/QRadioButton>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_InterfaceCfg_SimOthers
{
public:
    QRadioButton *radioRandom;
    QLabel *label;
    QRadioButton *radioZero;
    QRadioButton *radioHighest;
    QLabel *label_2;
    QLabel *label_3;
    QLabel *label_4;
    QPushButton *btnColorLine;
    QPushButton *btnColorBreak;
    QLabel *label_5;
    QPushButton *btnColorWidget;

    void setupUi(QWidget *InterfaceCfg_SimOthers)
    {
        if (InterfaceCfg_SimOthers->objectName().isEmpty())
            InterfaceCfg_SimOthers->setObjectName(QString::fromUtf8("InterfaceCfg_SimOthers"));
        InterfaceCfg_SimOthers->resize(400, 300);
        radioRandom = new QRadioButton(InterfaceCfg_SimOthers);
        radioRandom->setObjectName(QString::fromUtf8("radioRandom"));
        radioRandom->setGeometry(QRect(40, 40, 181, 22));
        label = new QLabel(InterfaceCfg_SimOthers);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(20, 20, 141, 17));
        radioZero = new QRadioButton(InterfaceCfg_SimOthers);
        radioZero->setObjectName(QString::fromUtf8("radioZero"));
        radioZero->setGeometry(QRect(40, 60, 171, 22));
        radioHighest = new QRadioButton(InterfaceCfg_SimOthers);
        radioHighest->setObjectName(QString::fromUtf8("radioHighest"));
        radioHighest->setGeometry(QRect(40, 80, 231, 22));
        label_2 = new QLabel(InterfaceCfg_SimOthers);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setGeometry(QRect(20, 120, 66, 17));
        label_3 = new QLabel(InterfaceCfg_SimOthers);
        label_3->setObjectName(QString::fromUtf8("label_3"));
        label_3->setGeometry(QRect(40, 150, 121, 17));
        label_4 = new QLabel(InterfaceCfg_SimOthers);
        label_4->setObjectName(QString::fromUtf8("label_4"));
        label_4->setGeometry(QRect(40, 180, 121, 17));
        btnColorLine = new QPushButton(InterfaceCfg_SimOthers);
        btnColorLine->setObjectName(QString::fromUtf8("btnColorLine"));
        btnColorLine->setGeometry(QRect(210, 150, 51, 27));
        btnColorBreak = new QPushButton(InterfaceCfg_SimOthers);
        btnColorBreak->setObjectName(QString::fromUtf8("btnColorBreak"));
        btnColorBreak->setGeometry(QRect(210, 180, 51, 27));
        label_5 = new QLabel(InterfaceCfg_SimOthers);
        label_5->setObjectName(QString::fromUtf8("label_5"));
        label_5->setGeometry(QRect(40, 210, 131, 17));
        btnColorWidget = new QPushButton(InterfaceCfg_SimOthers);
        btnColorWidget->setObjectName(QString::fromUtf8("btnColorWidget"));
        btnColorWidget->setGeometry(QRect(210, 210, 51, 27));

        retranslateUi(InterfaceCfg_SimOthers);

        QMetaObject::connectSlotsByName(InterfaceCfg_SimOthers);
    } // setupUi

    void retranslateUi(QWidget *InterfaceCfg_SimOthers)
    {
        InterfaceCfg_SimOthers->setWindowTitle(QApplication::translate("InterfaceCfg_SimOthers", "Form", 0, QApplication::UnicodeUTF8));
        radioRandom->setText(QApplication::translate("InterfaceCfg_SimOthers", "Return random value", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("InterfaceCfg_SimOthers", "Undefined value", 0, QApplication::UnicodeUTF8));
        radioZero->setText(QApplication::translate("InterfaceCfg_SimOthers", "Return zero value", 0, QApplication::UnicodeUTF8));
        radioHighest->setText(QApplication::translate("InterfaceCfg_SimOthers", "Return highest possible value", 0, QApplication::UnicodeUTF8));
        label_2->setText(QApplication::translate("InterfaceCfg_SimOthers", "Colors", 0, QApplication::UnicodeUTF8));
        label_3->setText(QApplication::translate("InterfaceCfg_SimOthers", "Highlighted line", 0, QApplication::UnicodeUTF8));
        label_4->setText(QApplication::translate("InterfaceCfg_SimOthers", "Breakpoint", 0, QApplication::UnicodeUTF8));
        btnColorLine->setText(QString());
        btnColorBreak->setText(QString());
        label_5->setText(QApplication::translate("InterfaceCfg_SimOthers", "Widget changed", 0, QApplication::UnicodeUTF8));
        btnColorWidget->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class InterfaceCfg_SimOthers: public Ui_InterfaceCfg_SimOthers {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_INTERFACECFG_SIMOTHERS_H
