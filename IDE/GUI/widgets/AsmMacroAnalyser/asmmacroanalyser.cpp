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
#include "../Editor/codeedit.h"
#include "../Editor/wtextedit.h"

AsmMacroAnalyser::AsmMacroAnalyser(QWidget *parent)
    : QTreeWidget(parent)
{
    QStringList labels;
    labels << "Name" << "Args" << "Line" << "File";
    this->setHeaderLabels(labels);
    QFontMetrics fontMetrics(this->font());
    this->header()->resizeSection(0, fontMetrics.width("0000000000"));
    this->header()->resizeSection(1, fontMetrics.width("0000000000"));
    this->header()->resizeSection(2, fontMetrics.width("00000"));
    
    this->setSortingEnabled(false);
    //macroEditRegExp.setPattern("[\\s]*([\\w]+)[\\s]+MACRO([\\s]+[\\w]+([,][\\s]*[\\w]+)*)?((\\r\\n)|(\\n)).*((\\r\\n)|(\\n))ENDM");
    macroEditRegExp.setPattern("(\\w+)\\s+MACRO(\\s+\\w+(?:,\\s*\\w+)*)?(?:\\s;.*)?\\r?\\n.*\\r?\\nENDM");
    macroEditRegExp.setCaseSensitivity(Qt::CaseInsensitive);
    macroEditRegExp.setMinimal(true);

    this->popupMenu = new QMenu(this);
    QAction *refreshAct = new QAction("Refresh", this->popupMenu);
    popupMenu->addAction(refreshAct);
    connect(refreshAct,
            SIGNAL(triggered()),
            this,
            SLOT(requestRefresh())
           );
    connect(this,
            SIGNAL(itemDoubleClicked(QTreeWidgetItem*, int)),
            this,
            SLOT(macroClickedSlot(QTreeWidgetItem*, int))
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
            QTextCursor cur(editList.at(i)->getTextEdit()->document());
            while ((position = macroEditRegExp.indexIn(source, position)) != -1)
            {
                matchedList = macroEditRegExp.capturedTexts();
                cur.setPosition(position);
                //qDebug() << "AsmMacroAnalyser: captured strings" << matchedList.at(0);
                /*for (int j = 0; j < matchedList.count(); j++)
                {
                    qDebug() << "AsmMacroAnalyser: captured strings" << j << matchedList.at(j);
                }*/
                
                QTreeWidgetItem *newItem = new QTreeWidgetItem(this);
                newItem->setText(0, matchedList.at(1));
                
                //newItem->setText(1, matchedList.at(2));
                if (matchedList.at(2) != "")
                {
                    QString args = matchedList.at(2);
                    args.remove(QRegExp("\\s"));
                    QStringList argsList = args.split(',', QString::SkipEmptyParts);
                    if (args.count() > 0)
                    {
                        newItem->setText(1, argsList.at(0));
                        for (int j = 1; j < argsList.count(); j++)
                        {
                            newItem->setText(1, newItem->text(1) + "," + argsList.at(j));
                        }
                    }
                }
                newItem->setText(2, QString::number(cur.blockNumber()+1));
                //newItem->setText(1, breakpointList->at(i).first.section('/', -1));
                newItem->setText(3, editList.at(i)->getName());
                newItem->setToolTip(3, editList.at(i)->getPath());
                this->addTopLevelItem(newItem);
                
                position += macroEditRegExp.matchedLength();
            }
        }
        this->header()->setResizeMode(QHeaderView::ResizeToContents);
        //this->header()->setResizeMode(QHeaderView::Interactive);
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


void AsmMacroAnalyser::macroClickedSlot(QTreeWidgetItem *item, int /*column*/)
{
    emit macroClicked(item->toolTip(3), item->text(2).toInt());
}