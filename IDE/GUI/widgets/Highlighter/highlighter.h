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

        QTextCharFormat keywordFormat;
        QTextCharFormat labelFormat;
        //QTextCharFormat classFormat;
        QTextCharFormat singleLineCommentFormat;
        QTextCharFormat multiLineCommentFormat;
        QTextCharFormat quotationFormat;
        QTextCharFormat functionFormat;
        QTextCharFormat operandsFormat;
        QTextCharFormat operatorsFormat;

        SourceType type;
 };

#endif
