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
 * @file highlighter.h
 */


#ifndef HIGHLIGHTER_H  
#define HIGHLIGHTER_H

#include <QTextDocument>
#include <QSyntaxHighlighter>
#include "../../enums/enums.h"




class Highlighter : public QSyntaxHighlighter
{
    Q_OBJECT

    public:
        Highlighter(QTextDocument *parent, SourceType type);

    protected:
        void highlightBlock(const QString &text);

    private:
        struct HighlightingRule
        {
            QRegExp pattern;
            QTextCharFormat format;
            QString tag;
        };
        QVector<HighlightingRule> highlightingRules;

        QRegExp commentStartExpression;
        QRegExp commentEndExpression;

        QTextCharFormat labelFormat;
        //QTextCharFormat classFormat;
        QTextCharFormat singleLineCommentFormat;
        QTextCharFormat multiLineCommentFormat;
        QTextCharFormat quotationFormat;
        QTextCharFormat simpleQuotationFormat;
        //directive
        QTextCharFormat keywordFormat;
        //instruction
        QTextCharFormat functionFormat;
        QTextCharFormat specFunctionFormat;
        //macro
        QTextCharFormat macroFormat;
        QTextCharFormat operandsFormat;
        QTextCharFormat operatorsFormat;
        //numbers
        QTextCharFormat hexaNumbersFormat;
        QTextCharFormat octaNumbersFormat;
        QTextCharFormat decNumbersFormat;
        QTextCharFormat binNumbersFormat;

        SourceType type;
 };

#endif
