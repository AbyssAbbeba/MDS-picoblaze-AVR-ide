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

};


#endif