#ifndef INTERFACECFG_SYNTAXASM_H
#define INTERFACECFG_SYNTAXASM_H

#include <QWidget>
#include "ui_interfacecfg_syntaxasm.h"


class InterfaceCfg_SyntaxAsm : public QWidget
{
    Q_OBJECT
    public:
        InterfaceCfg_SyntaxAsm(QWidget *parent);

    private:
        int type;
        Ui_InterfaceCfg_SyntaxAsm ui;

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