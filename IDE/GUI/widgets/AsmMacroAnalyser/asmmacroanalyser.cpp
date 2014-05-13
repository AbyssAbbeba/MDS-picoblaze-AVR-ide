/**
 * @brief
 * C++ Implementation: ...
 *
 * ...
 *
 * (C) copyright 2014 Moravia Microsystems, s.r.o.
 *
 * @author Erik Chalupa <erik.chalupa@moravia-microsystems.com>
 * @ingroup AsmMacroAnalyser
 * @file asmmacroanalyser.cpp
 */


#include "asmmacroanalyser.h"
#include <QtGui>

AsmMacroAnalyser::AsmMacroAnalyser(QWidget *parent)
    : QTreeWidget(parent)
{
    
}


void AsmMacroAnalyser::reload(QList<CodeEdit*> editList)
{
    this->clear();
    if (editList.count() > 0)
    {
        //set qregexp case insensitive
        QRegExp macroRegExp("^[\\s]*[\\w]+[\\s]+MACRO([\\s]+[\\w]+([,][\\s]*[\\w]+)*)?((\\r\\n)|(\\n)).*((\\r\\n)|(\\n))ENDM$");
        macroRegExp.setCaseSensitivity(Qt::CaseInsensitive);
        for (int i = 0; i < editList.count(); i++)
        {
            int position = 0;
            QString source = editList.at(i)->getTextEdit()->toPlainText();
            QStringList matchedList;
            while ((position = macroRegExp.indexIn(source, position)) != -1)
            {
                matchedList = macroRegExp.capturedTexts();
                qDebug() << "AsmMacroAnalyser: captured string" << matchedList.at(0);
                position += macroRegExp.matchedLength();
            }
        }
    }
}


void AsmMacroAnalyser::reload(QString mtblFilePath)
{
    this->clear();
}