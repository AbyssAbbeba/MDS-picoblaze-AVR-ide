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
    QStringList labels;
    labels << "Name" << "Line" << "File";
    this->setHeaderLabels(labels);
    this->setSortingEnabled(false);
    macroEditRegExp.setPattern("[\\s]*([\\w]+)[\\s]+MACRO([\\s]+[\\w]+([,][\\s]*[\\w]+)*)?((\\r\\n)|(\\n)).*((\\r\\n)|(\\n))ENDM");
    macroEditRegExp.setCaseSensitivity(Qt::CaseInsensitive);

    this->popupMenu = new QMenu(this);
    QAction *refreshAct = new QAction("Refresh", this->popupMenu);
    popupMenu->addAction(refreshAct);
    connect(refreshAct,
            SIGNAL(triggered()),
            this,
            SLOT(requestRefresh())
           );
}


void AsmMacroAnalyser::reload(QList<CodeEdit*> editList)
{
    this->clear();
    if (editList.count() > 0)
    {
        for (int i = 0; i < editList.count(); i++)
        {
            int position = 0;
            QString source = editList.at(i)->getTextEdit()->toPlainText();
            QStringList matchedList;
            while ((position = macroEditRegExp.indexIn(source, position)) != -1)
            {
                matchedList = macroEditRegExp.capturedTexts();
                qDebug() << "AsmMacroAnalyser: captured string" << matchedList.at(0);
                
                QTreeWidgetItem *newItem = new QTreeWidgetItem(this);
                newItem->setText(0, matchedList.at(1));
                //newItem->setText(1, breakpointList->at(i).first.section('/', -1));
                newItem->setText(2, editList.at(i)->getName());
                newItem->setToolTip(2, editList.at(i)->getPath());
                this->addTopLevelItem(newItem);
                
                position += macroEditRegExp.matchedLength();
            }
        }
    }
}


void AsmMacroAnalyser::reload(QString mtblFilePath)
{
    this->clear();
    QFile mtblFile;
}


void AsmMacroAnalyser::contextMenuEvent(QContextMenuEvent *event)
{
    popupMenu->popup(event->globalPos());
}


void AsmMacroAnalyser::requestRefresh()
{
    emit requestCodeEdits();
}