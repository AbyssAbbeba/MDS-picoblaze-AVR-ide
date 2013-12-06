/********************************************************************************
** Form generated from reading UI file 'interfacecfg_simwarnings.ui'
**
** Created: Thu Dec 5 18:47:58 2013
**      by: Qt User Interface Compiler version 4.8.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_INTERFACECFG_SIMWARNINGS_H
#define UI_INTERFACECFG_SIMWARNINGS_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QCheckBox>
#include <QtGui/QHeaderView>
#include <QtGui/QTabWidget>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_InterfaceCfg_SimWarnings
{
public:
    QTabWidget *tabWidget;
    QWidget *tab;
    QCheckBox *chckRdUnimpl;
    QCheckBox *chckRdAccess;
    QCheckBox *chckRdUndef;
    QCheckBox *chckRdDefault;
    QCheckBox *chckRdWrOnly;
    QCheckBox *chckRdPtWrOnly;
    QCheckBox *chckRdNonexist;
    QCheckBox *chckWrNonexist;
    QCheckBox *chckWrUnimpl;
    QCheckBox *chckWrAccess;
    QCheckBox *chckWrRdOnly;
    QCheckBox *chckWrPtRdOnly;
    QCheckBox *chckRdReserved;
    QCheckBox *chckWrReserved;
    QWidget *tab_2;
    QCheckBox *chckStackOver;
    QCheckBox *chckStackUnder;
    QWidget *tab_3;
    QCheckBox *chckOpcode;
    QCheckBox *chckJmp;
    QCheckBox *chckCall;
    QCheckBox *chckIRQ;
    QCheckBox *chckRet;
    QCheckBox *chckReti;
    QCheckBox *chckOpset;
    QCheckBox *chckInstUnsup;
    QCheckBox *chckInsIgnored;
    QCheckBox *chckPcOver;
    QCheckBox *chckPcUnder;

    void setupUi(QWidget *InterfaceCfg_SimWarnings)
    {
        if (InterfaceCfg_SimWarnings->objectName().isEmpty())
            InterfaceCfg_SimWarnings->setObjectName(QString::fromUtf8("InterfaceCfg_SimWarnings"));
        InterfaceCfg_SimWarnings->resize(400, 298);
        tabWidget = new QTabWidget(InterfaceCfg_SimWarnings);
        tabWidget->setObjectName(QString::fromUtf8("tabWidget"));
        tabWidget->setGeometry(QRect(0, 0, 401, 301));
        tab = new QWidget();
        tab->setObjectName(QString::fromUtf8("tab"));
        chckRdUnimpl = new QCheckBox(tab);
        chckRdUnimpl->setObjectName(QString::fromUtf8("chckRdUnimpl"));
        chckRdUnimpl->setGeometry(QRect(10, 30, 181, 22));
        chckRdAccess = new QCheckBox(tab);
        chckRdAccess->setObjectName(QString::fromUtf8("chckRdAccess"));
        chckRdAccess->setGeometry(QRect(10, 50, 181, 22));
        chckRdUndef = new QCheckBox(tab);
        chckRdUndef->setObjectName(QString::fromUtf8("chckRdUndef"));
        chckRdUndef->setGeometry(QRect(10, 70, 181, 22));
        chckRdDefault = new QCheckBox(tab);
        chckRdDefault->setObjectName(QString::fromUtf8("chckRdDefault"));
        chckRdDefault->setGeometry(QRect(10, 90, 181, 22));
        chckRdWrOnly = new QCheckBox(tab);
        chckRdWrOnly->setObjectName(QString::fromUtf8("chckRdWrOnly"));
        chckRdWrOnly->setGeometry(QRect(10, 110, 171, 22));
        chckRdPtWrOnly = new QCheckBox(tab);
        chckRdPtWrOnly->setObjectName(QString::fromUtf8("chckRdPtWrOnly"));
        chckRdPtWrOnly->setGeometry(QRect(10, 130, 191, 22));
        chckRdNonexist = new QCheckBox(tab);
        chckRdNonexist->setObjectName(QString::fromUtf8("chckRdNonexist"));
        chckRdNonexist->setGeometry(QRect(10, 10, 191, 22));
        chckWrNonexist = new QCheckBox(tab);
        chckWrNonexist->setObjectName(QString::fromUtf8("chckWrNonexist"));
        chckWrNonexist->setGeometry(QRect(210, 10, 171, 22));
        chckWrUnimpl = new QCheckBox(tab);
        chckWrUnimpl->setObjectName(QString::fromUtf8("chckWrUnimpl"));
        chckWrUnimpl->setGeometry(QRect(210, 30, 181, 22));
        chckWrAccess = new QCheckBox(tab);
        chckWrAccess->setObjectName(QString::fromUtf8("chckWrAccess"));
        chckWrAccess->setGeometry(QRect(210, 50, 161, 22));
        chckWrRdOnly = new QCheckBox(tab);
        chckWrRdOnly->setObjectName(QString::fromUtf8("chckWrRdOnly"));
        chckWrRdOnly->setGeometry(QRect(210, 110, 161, 22));
        chckWrPtRdOnly = new QCheckBox(tab);
        chckWrPtRdOnly->setObjectName(QString::fromUtf8("chckWrPtRdOnly"));
        chckWrPtRdOnly->setGeometry(QRect(210, 130, 171, 22));
        chckRdReserved = new QCheckBox(tab);
        chckRdReserved->setObjectName(QString::fromUtf8("chckRdReserved"));
        chckRdReserved->setGeometry(QRect(10, 150, 191, 22));
        chckWrReserved = new QCheckBox(tab);
        chckWrReserved->setObjectName(QString::fromUtf8("chckWrReserved"));
        chckWrReserved->setGeometry(QRect(210, 150, 181, 22));
        tabWidget->addTab(tab, QString());
        tab_2 = new QWidget();
        tab_2->setObjectName(QString::fromUtf8("tab_2"));
        chckStackOver = new QCheckBox(tab_2);
        chckStackOver->setObjectName(QString::fromUtf8("chckStackOver"));
        chckStackOver->setGeometry(QRect(10, 10, 131, 22));
        chckStackUnder = new QCheckBox(tab_2);
        chckStackUnder->setObjectName(QString::fromUtf8("chckStackUnder"));
        chckStackUnder->setGeometry(QRect(10, 30, 141, 22));
        tabWidget->addTab(tab_2, QString());
        tab_3 = new QWidget();
        tab_3->setObjectName(QString::fromUtf8("tab_3"));
        chckOpcode = new QCheckBox(tab_3);
        chckOpcode->setObjectName(QString::fromUtf8("chckOpcode"));
        chckOpcode->setGeometry(QRect(10, 10, 151, 22));
        chckJmp = new QCheckBox(tab_3);
        chckJmp->setObjectName(QString::fromUtf8("chckJmp"));
        chckJmp->setGeometry(QRect(10, 30, 131, 22));
        chckCall = new QCheckBox(tab_3);
        chckCall->setObjectName(QString::fromUtf8("chckCall"));
        chckCall->setGeometry(QRect(10, 50, 131, 22));
        chckIRQ = new QCheckBox(tab_3);
        chckIRQ->setObjectName(QString::fromUtf8("chckIRQ"));
        chckIRQ->setGeometry(QRect(10, 70, 141, 22));
        chckRet = new QCheckBox(tab_3);
        chckRet->setObjectName(QString::fromUtf8("chckRet"));
        chckRet->setGeometry(QRect(10, 90, 131, 22));
        chckReti = new QCheckBox(tab_3);
        chckReti->setObjectName(QString::fromUtf8("chckReti"));
        chckReti->setGeometry(QRect(10, 110, 191, 22));
        chckOpset = new QCheckBox(tab_3);
        chckOpset->setObjectName(QString::fromUtf8("chckOpset"));
        chckOpset->setGeometry(QRect(10, 130, 201, 22));
        chckInstUnsup = new QCheckBox(tab_3);
        chckInstUnsup->setObjectName(QString::fromUtf8("chckInstUnsup"));
        chckInstUnsup->setGeometry(QRect(10, 150, 201, 22));
        chckInsIgnored = new QCheckBox(tab_3);
        chckInsIgnored->setObjectName(QString::fromUtf8("chckInsIgnored"));
        chckInsIgnored->setGeometry(QRect(10, 170, 201, 22));
        chckPcOver = new QCheckBox(tab_3);
        chckPcOver->setObjectName(QString::fromUtf8("chckPcOver"));
        chckPcOver->setGeometry(QRect(10, 190, 191, 22));
        chckPcUnder = new QCheckBox(tab_3);
        chckPcUnder->setObjectName(QString::fromUtf8("chckPcUnder"));
        chckPcUnder->setGeometry(QRect(10, 210, 191, 22));
        tabWidget->addTab(tab_3, QString());

        retranslateUi(InterfaceCfg_SimWarnings);

        tabWidget->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(InterfaceCfg_SimWarnings);
    } // setupUi

    void retranslateUi(QWidget *InterfaceCfg_SimWarnings)
    {
        InterfaceCfg_SimWarnings->setWindowTitle(QApplication::translate("InterfaceCfg_SimWarnings", "Form", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        chckRdUnimpl->setToolTip(QApplication::translate("InterfaceCfg_SimWarnings", "Attempt to read from an unimplemented memory cell", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        chckRdUnimpl->setText(QApplication::translate("InterfaceCfg_SimWarnings", "Read unimplemented", 0, QApplication::UnicodeUTF8));
        chckRdAccess->setText(QApplication::translate("InterfaceCfg_SimWarnings", "Read access denied", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        chckRdUndef->setToolTip(QApplication::translate("InterfaceCfg_SimWarnings", "Undefined (uninitialized) value read.", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        chckRdUndef->setText(QApplication::translate("InterfaceCfg_SimWarnings", "Read undefined", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        chckRdDefault->setToolTip(QApplication::translate("InterfaceCfg_SimWarnings", "Value defined by default condition was read", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        chckRdDefault->setText(QApplication::translate("InterfaceCfg_SimWarnings", "Read default", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        chckRdWrOnly->setToolTip(QApplication::translate("InterfaceCfg_SimWarnings", "Value read from write only memory cell", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        chckRdWrOnly->setText(QApplication::translate("InterfaceCfg_SimWarnings", "Read write-only", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        chckRdPtWrOnly->setToolTip(QApplication::translate("InterfaceCfg_SimWarnings", "Value read from partially write only memory cell", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        chckRdPtWrOnly->setText(QApplication::translate("InterfaceCfg_SimWarnings", "Read part write-only", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        chckRdNonexist->setToolTip(QApplication::translate("InterfaceCfg_SimWarnings", "Attempt to read from nonexistent memory space", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        chckRdNonexist->setText(QApplication::translate("InterfaceCfg_SimWarnings", "Read nonexistent", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        chckWrNonexist->setToolTip(QApplication::translate("InterfaceCfg_SimWarnings", "Attempt to write to a nonexistent memory space", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        chckWrNonexist->setText(QApplication::translate("InterfaceCfg_SimWarnings", "Write nonexistent", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        chckWrUnimpl->setToolTip(QApplication::translate("InterfaceCfg_SimWarnings", "Attempt to write to an unimplemented memory cell", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        chckWrUnimpl->setText(QApplication::translate("InterfaceCfg_SimWarnings", "Write unimplemented", 0, QApplication::UnicodeUTF8));
        chckWrAccess->setText(QApplication::translate("InterfaceCfg_SimWarnings", "Write access denied", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        chckWrRdOnly->setToolTip(QApplication::translate("InterfaceCfg_SimWarnings", "alue written to read only memory cell", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        chckWrRdOnly->setText(QApplication::translate("InterfaceCfg_SimWarnings", "Write read-only", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        chckWrPtRdOnly->setToolTip(QApplication::translate("InterfaceCfg_SimWarnings", "Value written to partially read only memory cell", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        chckWrPtRdOnly->setText(QApplication::translate("InterfaceCfg_SimWarnings", "Write part read-only", 0, QApplication::UnicodeUTF8));
        chckRdReserved->setText(QApplication::translate("InterfaceCfg_SimWarnings", "Read reserved register", 0, QApplication::UnicodeUTF8));
        chckWrReserved->setText(QApplication::translate("InterfaceCfg_SimWarnings", "Write reserved register", 0, QApplication::UnicodeUTF8));
        tabWidget->setTabText(tabWidget->indexOf(tab), QApplication::translate("InterfaceCfg_SimWarnings", "Memory", 0, QApplication::UnicodeUTF8));
        chckStackOver->setText(QApplication::translate("InterfaceCfg_SimWarnings", "Stack overflow", 0, QApplication::UnicodeUTF8));
        chckStackUnder->setText(QApplication::translate("InterfaceCfg_SimWarnings", "Stack underflow", 0, QApplication::UnicodeUTF8));
        tabWidget->setTabText(tabWidget->indexOf(tab_2), QApplication::translate("InterfaceCfg_SimWarnings", "Stack", 0, QApplication::UnicodeUTF8));
        chckOpcode->setText(QApplication::translate("InterfaceCfg_SimWarnings", "Invalid opcode", 0, QApplication::UnicodeUTF8));
        chckJmp->setText(QApplication::translate("InterfaceCfg_SimWarnings", "Invalid jump", 0, QApplication::UnicodeUTF8));
        chckCall->setText(QApplication::translate("InterfaceCfg_SimWarnings", "Invalid call", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        chckIRQ->setToolTip(QApplication::translate("InterfaceCfg_SimWarnings", "Invalid interrupt, i.e. nonsense IRQ (Interrupt ReQuest)", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        chckIRQ->setText(QApplication::translate("InterfaceCfg_SimWarnings", "Invalid IRQ", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        chckRet->setToolTip(QApplication::translate("InterfaceCfg_SimWarnings", "There is no active subprogram to return from", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        chckRet->setText(QApplication::translate("InterfaceCfg_SimWarnings", "Invalid return", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        chckReti->setToolTip(QApplication::translate("InterfaceCfg_SimWarnings", "There is no active interrupt to return from", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        chckReti->setText(QApplication::translate("InterfaceCfg_SimWarnings", "Invalid interrupt return", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        chckOpset->setToolTip(QApplication::translate("InterfaceCfg_SimWarnings", "Undefined result of the operation due to invalid set of operands", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        chckOpset->setText(QApplication::translate("InterfaceCfg_SimWarnings", "Invalid set of operands", 0, QApplication::UnicodeUTF8));
        chckInstUnsup->setText(QApplication::translate("InterfaceCfg_SimWarnings", "Instruction unsupported", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        chckInsIgnored->setToolTip(QApplication::translate("InterfaceCfg_SimWarnings", "Instruction ignored due to settings", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        chckInsIgnored->setText(QApplication::translate("InterfaceCfg_SimWarnings", "Instruction ignored", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        chckPcOver->setToolTip(QApplication::translate("InterfaceCfg_SimWarnings", "Program counter overflow", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        chckPcOver->setText(QApplication::translate("InterfaceCfg_SimWarnings", "PC overflow", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        chckPcUnder->setToolTip(QApplication::translate("InterfaceCfg_SimWarnings", "Program counter underflow", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        chckPcUnder->setText(QApplication::translate("InterfaceCfg_SimWarnings", "PC underflow", 0, QApplication::UnicodeUTF8));
        tabWidget->setTabText(tabWidget->indexOf(tab_3), QApplication::translate("InterfaceCfg_SimWarnings", "CPU", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class InterfaceCfg_SimWarnings: public Ui_InterfaceCfg_SimWarnings {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_INTERFACECFG_SIMWARNINGS_H
