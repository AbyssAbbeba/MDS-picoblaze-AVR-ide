#ifndef HIGHLIGHTER_H  
#define HIGHLIGHTER_H

#include <QTextDocument>
#include <QSyntaxHighlighter>


typedef enum SourceType
{PLAIN = 0, C, CPP, AVRASM, PICASM, PICOBLAZEASM} SourceType;


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

        SourceType type;
 };

#endif
