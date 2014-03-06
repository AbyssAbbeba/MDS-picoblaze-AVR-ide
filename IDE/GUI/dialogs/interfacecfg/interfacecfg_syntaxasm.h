/**
 * @brief
 * C++ Interface: ...
 *
 * ...
 *
 * (C) copyright 2013, 2014 Moravia Microsystems, s.r.o.
 *
 * @author Erik Chalupa <erik.chalupa@moravia-microsystems.com>
 * @ingroup GUI
 * @file interfacecfg_syntaxasm.h
 */


#ifndef INTERFACECFG_SYNTAXASM_H
#define INTERFACECFG_SYNTAXASM_H

#include <QWidget>
#include <QColor>
#include "ui_interfacecfg_syntaxasm.h"


class InterfaceCfg_SyntaxAsm : public QWidget
{
    Q_OBJECT
    
    public:
        InterfaceCfg_SyntaxAsm(QWidget *parent);
        void save();

        typedef enum
        {
            COMMENT = 0,
            LABEL,
            KEYWORD,
            OPERAND,
            OPERATOR,
            QUOTE
        } Types;

    private:
        Types type;
        Ui_InterfaceCfg_SyntaxAsm ui;

    signals:
        void colorChanged(const QColor &color, InterfaceCfg_SyntaxAsm::Types type);
        void italicChanged(bool enabled, InterfaceCfg_SyntaxAsm::Types type);
        void underlineChanged(bool enabled, InterfaceCfg_SyntaxAsm::Types type);
        void strikeChanged(bool enabled, InterfaceCfg_SyntaxAsm::Types type);
        void boldChanged(bool enabled, InterfaceCfg_SyntaxAsm::Types type);

    private slots:
        void commentColor();
        void labelColor();
        void keywordColor();
        void operandColor();
        void operatorColor();
        void quoteColor();
        void colorSelected(const QColor &color);
        
        void checkBoxICmntChanged(int state);
        void checkBoxUCmntChanged(int state);
        void checkBoxSCmntChanged(int state);
        void checkBoxBCmntChanged(int state);
        
        void checkBoxILblChanged(int state);
        void checkBoxULblChanged(int state);
        void checkBoxSLblChanged(int state);
        void checkBoxBLblChanged(int state);

        void checkBoxIKwrdChanged(int state);
        void checkBoxUKwrdChanged(int state);
        void checkBoxSKwrdChanged(int state);
        void checkBoxBKwrdChanged(int state);

        void checkBoxIOpdsChanged(int state);
        void checkBoxUOpdsChanged(int state);
        void checkBoxSOpdsChanged(int state);
        void checkBoxBOpdsChanged(int state);

        void checkBoxIOptsChanged(int state);
        void checkBoxUOptsChanged(int state);
        void checkBoxSOptsChanged(int state);
        void checkBoxBOptsChanged(int state);

        void checkBoxIQuoteChanged(int state);
        void checkBoxUQuoteChanged(int state);
        void checkBoxSQuoteChanged(int state);
        void checkBoxBQuoteChanged(int state);

};


#endif