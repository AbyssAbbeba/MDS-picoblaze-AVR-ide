/********************************************************************************
** Form generated from reading UI file 'interfacecfg_editsyntax.ui'
**
** Created: Fri Dec 6 12:48:25 2013
**      by: Qt User Interface Compiler version 4.8.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_INTERFACECFG_EDITSYNTAX_H
#define UI_INTERFACECFG_EDITSYNTAX_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QCheckBox>
#include <QtGui/QFrame>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QScrollArea>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_InterfaceCfg_EditSyntax
{
public:
    QLabel *label;
    QCheckBox *chckHighlight;
    QScrollArea *scrollArea;
    QWidget *scrollAreaWidgetContents;
    QFrame *frame;
    QLabel *lblLbl;
    QLabel *lblKwrd2;
    QLabel *lblKwrd;
    QLabel *lblCmnt;
    QLabel *lblReg;
    QLabel *lblIgnore;
    QLabel *lblNum;
    QLabel *lblIgnore_2;
    QLabel *lblName;
    QLabel *lblI;
    QLabel *lblU;
    QLabel *lblS;
    QLabel *lblB;
    QLabel *lblColor;

    void setupUi(QWidget *InterfaceCfg_EditSyntax)
    {
        if (InterfaceCfg_EditSyntax->objectName().isEmpty())
            InterfaceCfg_EditSyntax->setObjectName(QString::fromUtf8("InterfaceCfg_EditSyntax"));
        InterfaceCfg_EditSyntax->resize(400, 300);
        label = new QLabel(InterfaceCfg_EditSyntax);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(20, 20, 181, 17));
        chckHighlight = new QCheckBox(InterfaceCfg_EditSyntax);
        chckHighlight->setObjectName(QString::fromUtf8("chckHighlight"));
        chckHighlight->setGeometry(QRect(220, 20, 97, 22));
        chckHighlight->setChecked(true);
        scrollArea = new QScrollArea(InterfaceCfg_EditSyntax);
        scrollArea->setObjectName(QString::fromUtf8("scrollArea"));
        scrollArea->setGeometry(QRect(20, 70, 361, 141));
        scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
        scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        scrollArea->setWidgetResizable(true);
        scrollAreaWidgetContents = new QWidget();
        scrollAreaWidgetContents->setObjectName(QString::fromUtf8("scrollAreaWidgetContents"));
        scrollAreaWidgetContents->setGeometry(QRect(0, 0, 359, 139));
        scrollArea->setWidget(scrollAreaWidgetContents);
        frame = new QFrame(InterfaceCfg_EditSyntax);
        frame->setObjectName(QString::fromUtf8("frame"));
        frame->setGeometry(QRect(20, 220, 361, 61));
        QPalette palette;
        QBrush brush(QColor(255, 255, 255, 255));
        brush.setStyle(Qt::SolidPattern);
        palette.setBrush(QPalette::Active, QPalette::Base, brush);
        palette.setBrush(QPalette::Active, QPalette::Window, brush);
        palette.setBrush(QPalette::Inactive, QPalette::Base, brush);
        palette.setBrush(QPalette::Inactive, QPalette::Window, brush);
        palette.setBrush(QPalette::Disabled, QPalette::Base, brush);
        palette.setBrush(QPalette::Disabled, QPalette::Window, brush);
        frame->setPalette(palette);
        frame->setAutoFillBackground(true);
        frame->setFrameShape(QFrame::StyledPanel);
        frame->setFrameShadow(QFrame::Raised);
        lblLbl = new QLabel(frame);
        lblLbl->setObjectName(QString::fromUtf8("lblLbl"));
        lblLbl->setGeometry(QRect(10, 0, 41, 17));
        lblKwrd2 = new QLabel(frame);
        lblKwrd2->setObjectName(QString::fromUtf8("lblKwrd2"));
        lblKwrd2->setGeometry(QRect(60, 40, 31, 17));
        lblKwrd = new QLabel(frame);
        lblKwrd->setObjectName(QString::fromUtf8("lblKwrd"));
        lblKwrd->setGeometry(QRect(60, 0, 31, 17));
        lblCmnt = new QLabel(frame);
        lblCmnt->setObjectName(QString::fromUtf8("lblCmnt"));
        lblCmnt->setGeometry(QRect(60, 20, 91, 17));
        lblReg = new QLabel(frame);
        lblReg->setObjectName(QString::fromUtf8("lblReg"));
        lblReg->setGeometry(QRect(90, 0, 21, 17));
        lblIgnore = new QLabel(frame);
        lblIgnore->setObjectName(QString::fromUtf8("lblIgnore"));
        lblIgnore->setGeometry(QRect(110, 0, 16, 17));
        lblNum = new QLabel(frame);
        lblNum->setObjectName(QString::fromUtf8("lblNum"));
        lblNum->setGeometry(QRect(120, 0, 41, 17));
        lblIgnore_2 = new QLabel(frame);
        lblIgnore_2->setObjectName(QString::fromUtf8("lblIgnore_2"));
        lblIgnore_2->setGeometry(QRect(90, 40, 66, 17));
        lblName = new QLabel(InterfaceCfg_EditSyntax);
        lblName->setObjectName(QString::fromUtf8("lblName"));
        lblName->setGeometry(QRect(30, 50, 66, 17));
        lblI = new QLabel(InterfaceCfg_EditSyntax);
        lblI->setObjectName(QString::fromUtf8("lblI"));
        lblI->setGeometry(QRect(170, 50, 16, 17));
        lblU = new QLabel(InterfaceCfg_EditSyntax);
        lblU->setObjectName(QString::fromUtf8("lblU"));
        lblU->setGeometry(QRect(200, 50, 16, 17));
        lblS = new QLabel(InterfaceCfg_EditSyntax);
        lblS->setObjectName(QString::fromUtf8("lblS"));
        lblS->setGeometry(QRect(230, 50, 16, 17));
        lblB = new QLabel(InterfaceCfg_EditSyntax);
        lblB->setObjectName(QString::fromUtf8("lblB"));
        lblB->setGeometry(QRect(260, 50, 16, 17));
        lblColor = new QLabel(InterfaceCfg_EditSyntax);
        lblColor->setObjectName(QString::fromUtf8("lblColor"));
        lblColor->setGeometry(QRect(310, 50, 66, 17));

        retranslateUi(InterfaceCfg_EditSyntax);

        QMetaObject::connectSlotsByName(InterfaceCfg_EditSyntax);
    } // setupUi

    void retranslateUi(QWidget *InterfaceCfg_EditSyntax)
    {
        InterfaceCfg_EditSyntax->setWindowTitle(QApplication::translate("InterfaceCfg_EditSyntax", "Form", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("InterfaceCfg_EditSyntax", "Enable syntax highlight", 0, QApplication::UnicodeUTF8));
        chckHighlight->setText(QString());
        lblLbl->setText(QApplication::translate("InterfaceCfg_EditSyntax", "main:", 0, QApplication::UnicodeUTF8));
        lblKwrd2->setText(QApplication::translate("InterfaceCfg_EditSyntax", "jmp", 0, QApplication::UnicodeUTF8));
        lblKwrd->setText(QApplication::translate("InterfaceCfg_EditSyntax", "add", 0, QApplication::UnicodeUTF8));
        lblCmnt->setText(QApplication::translate("InterfaceCfg_EditSyntax", ";ret", 0, QApplication::UnicodeUTF8));
        lblReg->setText(QApplication::translate("InterfaceCfg_EditSyntax", " S2", 0, QApplication::UnicodeUTF8));
        lblIgnore->setText(QApplication::translate("InterfaceCfg_EditSyntax", ",", 0, QApplication::UnicodeUTF8));
        lblNum->setText(QApplication::translate("InterfaceCfg_EditSyntax", "#0x01", 0, QApplication::UnicodeUTF8));
        lblIgnore_2->setText(QApplication::translate("InterfaceCfg_EditSyntax", "main", 0, QApplication::UnicodeUTF8));
        lblName->setText(QApplication::translate("InterfaceCfg_EditSyntax", "Name", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        lblI->setToolTip(QApplication::translate("InterfaceCfg_EditSyntax", "Italic", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        lblI->setText(QApplication::translate("InterfaceCfg_EditSyntax", "I", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        lblU->setToolTip(QApplication::translate("InterfaceCfg_EditSyntax", "Underline", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        lblU->setText(QApplication::translate("InterfaceCfg_EditSyntax", "U", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        lblS->setToolTip(QApplication::translate("InterfaceCfg_EditSyntax", "Strike", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        lblS->setText(QApplication::translate("InterfaceCfg_EditSyntax", "S", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        lblB->setToolTip(QApplication::translate("InterfaceCfg_EditSyntax", "Bold", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        lblB->setText(QApplication::translate("InterfaceCfg_EditSyntax", "B", 0, QApplication::UnicodeUTF8));
        lblColor->setText(QApplication::translate("InterfaceCfg_EditSyntax", "Color", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class InterfaceCfg_EditSyntax: public Ui_InterfaceCfg_EditSyntax {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_INTERFACECFG_EDITSYNTAX_H
