#ifndef GUICFGSTRUCT_H
#define GUICFGSTRUCT_H

#include <QFont>
#include <QColor>

typedef struct GuiCfgStruct
{
    //fonts
    QFont editorFont;
    QFont interfaceFont;

    //interface colors
    QColor simulationColor;
    QColor simWidgetEditedColor;

    //editor colors
    QColor breakpointColor;
    QColor stringColor;
    QColor keywordColor;
    QColor functionColor;
    
} GuiCfgStruct;


#endif