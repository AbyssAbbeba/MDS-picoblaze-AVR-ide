#include <QtGui>
#include "highlighter.h" 


Highlighter::Highlighter(QTextDocument *parent, SourceType type)
     : QSyntaxHighlighter(parent)
{
    HighlightingRule rule;
    if (type == C)
    {
        QStringList keywordPatterns;
        keywordPatterns << "\\bauto\\b" << "\\bbreak\\b" << "\\bchar\\b"
            << "\\bconst\\b" << "\\bcontinue\\b" << "\\bdefault\\b"
            << "\\bdo\\b" << "\\bdouble\\b" << "\\belse\\b"
            << "\\benum\\b" << "\\bextern\\b" << "\\bfloat\\b"
            << "\\bfor\\b" << "\\bgoto\\b" << "\\binline\\b"
            << "\\bif\\b" << "\\bint\\b" << "\\blong\\b"
            << "\\bregister\\b" << "\\breturn\\b" << "\\bshort\\b"
            << "\\bsigned\\b" << "\\bsizeof\\b" << "\\bstatic\\b"
            << "\\bstruct\\b" << "\\bswitch\\b" << "\\btypedef\\b"
            << "\\bunion\\b" << "\\bunsigned\\b" << "\\bvoid\\b"
            << "\\bvolatile\\b" << "\\bwhile\\b";

        keywordFormat.setForeground(Qt::darkBlue);
        keywordFormat.setFontWeight(QFont::Bold);

        foreach (const QString &pattern, keywordPatterns) {
            rule.pattern = QRegExp(pattern);
            rule.format = keywordFormat;
            highlightingRules.append(rule);
        }

        quotationFormat.setForeground(Qt::darkRed);
        rule.pattern = QRegExp("\".*\"");
        rule.format = quotationFormat;
        rule.tag = "quotation";
        highlightingRules.append(rule);

        functionFormat.setFontItalic(true);
        functionFormat.setForeground(Qt::blue);
        rule.pattern = QRegExp("\\b[A-Za-z0-9_]+(?=\\()");
        rule.format = functionFormat;
        rule.tag = "function";
        highlightingRules.append(rule);

        singleLineCommentFormat.setForeground(Qt::gray);
        rule.pattern = QRegExp("//[^\n]*");
        rule.format = singleLineCommentFormat;
        rule.tag = "lineComment";
        highlightingRules.append(rule);

        multiLineCommentFormat.setForeground(Qt::gray);
        commentStartExpression = QRegExp("/\\*");
        commentEndExpression = QRegExp("\\*/");
    }
    else if (type == CPP)
    {
        QStringList keywordPatterns;
        keywordPatterns << "\\bauto\\b" << "\\bbreak\\b" << "\\bchar\\b"
            << "\\bconst\\b" << "\\bcontinue\\b" << "\\bdefault\\b"
            << "\\bdo\\b" << "\\bdouble\\b" << "\\belse\\b"
            << "\\benum\\b" << "\\bextern\\b" << "\\bfloat\\b"
            << "\\bfor\\b" << "\\bgoto\\b" << "\\binline\\b"
            << "\\bif\\b" << "\\bint\\b" << "\\blong\\b"
            << "\\bregister\\b" << "\\breturn\\b" << "\\bshort\\b"
            << "\\bsigned\\b" << "\\bsizeof\\b" << "\\bstatic\\b"
            << "\\bstruct\\b" << "\\bswitch\\b" << "\\btypedef\\b"
            << "\\bunion\\b" << "\\bunsigned\\b" << "\\bvoid\\b"
            << "\\bvolatile\\b" << "\\bwhile\\b";

        keywordFormat.setForeground(Qt::darkBlue);
        keywordFormat.setFontWeight(QFont::Bold);

        foreach (const QString &pattern, keywordPatterns) {
            rule.pattern = QRegExp(pattern);
            rule.format = keywordFormat;
            highlightingRules.append(rule);
        }
        
        quotationFormat.setForeground(Qt::darkRed);
        rule.pattern = QRegExp("\".*\"");
        rule.format = quotationFormat;
        rule.tag = "quotation";
        highlightingRules.append(rule);

        functionFormat.setFontItalic(true);
        functionFormat.setForeground(Qt::blue);
        rule.pattern = QRegExp("\\b[A-Za-z0-9_]+(?=\\()");
        rule.format = functionFormat;
        rule.tag = "function";
        highlightingRules.append(rule);

        singleLineCommentFormat.setForeground(Qt::gray);
        rule.pattern = QRegExp("//[^\n]*");
        rule.format = singleLineCommentFormat;
        rule.tag = "lineComment";
        highlightingRules.append(rule);

        multiLineCommentFormat.setForeground(Qt::gray);
        commentStartExpression = QRegExp("/\\*");
        commentEndExpression = QRegExp("\\*/");
    }
    else if (type == AVRASM)
    {
        QStringList keywordPatterns;

        keywordPatterns << "\\bbyte\\b" << "\\bcseg\\b" << "\\bdb\\b"
            << "\\bdef\\b" << "\\bdevice\\b" << "\\bdw\\b"
            << "\\beseg\\b" << "\\bequ\\b" << "\\belse\\b"
            << "\\belseif\\b" << "\\belseifb\\b" << "\\belseifdef\\b"
            << "\\belseifn\\b" << "\\belseifnb\\b" << "\\belseifndef\\b"
            << "\\bend\\b" << "\\bendif\\b" << "\\bendm\\b"
            << "\\bendmacro\\b" << "\\bexit\\b" << "\\bexitm\\b"
            << "\\bif\\b" << "\\bifb\\b" << "\\bifdef\\b"
            << "\\bifn\\b" << "\\befnb\\b" << "\\bifndef\\b"
            << "\\blist\\b" << "\\blistmac\\b" << "\\blocal\\b"
            << "\\bmacro\\b" << "\\bnolist\\b" << "\\borg\\b"
            << "\\brecp\\b" << "\\bset\\b" << "\\bskip\\b";

        keywordFormat.setForeground(Qt::darkBlue);
        keywordFormat.setFontWeight(QFont::Bold);
            
        foreach (const QString &pattern, keywordPatterns) {
            rule.pattern = QRegExp(pattern);
            rule.format = keywordFormat;
            rule.tag = "keyword";
            highlightingRules.append(rule);
        }

        quotationFormat.setForeground(Qt::darkRed);
        rule.pattern = QRegExp("\".*\"");
        rule.format = quotationFormat;
        rule.tag = "quotation";
        highlightingRules.append(rule);

        QStringList functionPatterns;
        functionPatterns << "\\badc\\b" << "\\badd\\b" << "\\badiw\\b"
            << "\\band\\b" << "\\bandi\\b" << "\\basr\\b"
            << "\\bbclr\\b" << "\\bbrbc\\b" << "\\bbrbs\\b"
            << "\\bbrcc\\b" << "\\bbrcs\\b" << "\\bbreq\\b"
            << "\\bbrge\\b" << "\\bbrhc\\b" << "\\bbhrs\\b"
            << "\\bbld\\b" << "\\bbrid\\b" << "\\bbrie\\b"
            << "\\bbrlo\\b" << "\\bbrlt\\b" << "\\bbrmi\\b"
            << "\\bbrne\\b" << "\\bbrpl\\b" << "\\bbrsh\\b"
            << "\\bbrtc\\b" << "\\bbrts\\b" << "\\bbrvc\\b"
            << "\\bbrvs\\b" << "\\bbset\\b" << "\\bbst\\b"
            << "\\bcall\\b" << "\\bcbi\\b" << "\\bcbr\\b"
            << "\\bclc\\b" << "\\bclh\\b" << "\\bcli\\b"
            << "\\bcln\\b" << "\\bclr\\b" << "\\bcls\\b"
            << "\\bclt\\b" << "\\bclv\\b" << "\\bclz\\b"
            << "\\bcp\\b" << "\\bcpc\\b" << "\\bcpi\\b"
            << "\\bcpse\\b" << "\\bcom\\b" << "\\bdec\\b"
            << "\\beor\\b" << "\\bicall\\b" << "\\bijmp\\b"
            << "\\bin\\b" << "\\binc\\b" << "\\bjmp\\b"
            << "\\bld\\b" << "\\bldd\\b" << "\\bldi\\b"
            << "\\blds\\b" << "\\blpm\\b" << "\\blsl\\b"
            << "\\blsr\\b" << "\\bmov\\b" << "\\bmul\\b"
            << "\\bneg\\b" << "\\bnop\\b" << "\\bor\\b"
            << "\\bori\\b" << "\\bout\\b" << "\\bpop\\b"
            << "\\bpush\\b" << "\\brcall\\b" << "\\bret\\b"
            << "\\breti\\b" << "\\brjmp\\b" << "\\brol\\b"
            << "\\bror\\b" << "\\bsbc\\b" << "\\bsbci\\b"
            << "\\bsbi\\b" << "\\bsbic\\b" << "\\bsbis\\b"
            << "\\bsbiw\\b" << "\\bsbr\\b" << "\\bsbrc\\b"
            << "\\bsbrs\\b" << "\\bsec\\b" << "\\bseh\\b"
            << "\\bsei\\b" << "\\bsen\\b" << "\\bses\\b"
            << "\\bset\\b" << "\\bsev\\b" << "\\bser\\b"
            << "\\bsez\\b" << "\\bsleep\\b" << "\\bst\\b"
            << "\\bstd\\b" << "\\bsts\\b" << "\\bsub\\b"
            << "\\bsubi\\b" << "\\bswap\\b" << "\\btst\\b"
            << "\\bwdr\\b";
        
        functionFormat.setFontItalic(true);
        functionFormat.setForeground(Qt::blue);
        
        foreach (const QString &pattern, functionPatterns) {
            rule.pattern = QRegExp(pattern);
            rule.format = functionFormat;
            rule.tag = "function";
            highlightingRules.append(rule);
        }

        singleLineCommentFormat.setForeground(Qt::gray);
        rule.pattern = QRegExp(";[^\n]*");
        rule.format = singleLineCommentFormat;
        rule.tag = "lineComment";
        highlightingRules.append(rule);

        multiLineCommentFormat.setForeground(Qt::gray);
        commentStartExpression = QRegExp("/\\*");
        commentEndExpression = QRegExp("\\*/");
    }
    else if (type == PICASM)
    {
        QStringList keywordPatterns;
        keywordPatterns << "\\baccess_ovr\\b" << "\\b__badram\\b" << "\\b__badrom\\b"
            << "\\bbankisel\\b" << "\\bbanksel\\b" << "\\bcblock\\b"
            << "\\bcode_pack\\b" << "\\bcode\\b" << "\\bconfig\\b"
            << "\\b__config\\b" << "\\bconstant\\b" << "\\bda\\b"
            << "\\bdata\\b" << "\\bdb\\b" << "\\bdefine\\b"
            << "\\bde\\b" << "\\bdt\\b" << "\\bdw\\b"
            << "\\belseifb\\b" << "\\belseifdef\\b" << "\\belseifnb\\b"
            << "\\belseifndef\\b" << "\\belseifn\\b" << "\\belseif\\b"
            << "\\belse\\b" << "\\bendc\\b" << "\\bendif\\b"
            << "\\bendmacro\\b" << "\\bendm\\b" << "\\bendw\\b"
            << "\\bequ\\b" << "\\berrorlevel\\b" << "\\berror\\b"
            << "\\bexitm\\b" << "\\bexpand\\b" << "\\bextern\\b"
            << "\\bfill\\b" << "\\bglobal\\b" << "\\bidata_acs\\b"
            << "\\bidata\\b" << "\\b__idlocs\\b" << "\\bifb\\b"
            << "\\bifdef\\b" << "\\bifnb\\b" << "\\bifndef\\b"
            << "\\bifn\\b" << "\\bif\\b" << "\\blist\\b"
            << "\\blocal\\b" << "\\bmacro\\b" << "\\b__maxram\\b"
            << "\\b__maxrom\\b" << "\\bmessg\\b" << "\\bnoexpand\\b"
            << "\\bnolist\\b" << "\\borg\\b" << "\\bpage\\b"
            << "\\bpagesel\\b" << "\\bpageselw\\b" << "\\bprocessor\\b"
            << "\\bradix\\b" << "\\brept\\b" << "\\bres\\b"
            << "\\bset\\b" << "\\bskip\\b" << "\\bspace\\b"
            << "\\bsubtitle\\b" << "\\btitle\\b" << "\\budata_acs\\b"
            << "\\budata_ovr\\b" << "\\budata\\b" << "\\budata_shr\\b"
            << "\\bundefine\\b" << "\\bvariable\\b" << "\\bwhile\\b";

        keywordFormat.setForeground(Qt::darkBlue);
        keywordFormat.setFontWeight(QFont::Bold);

        foreach (const QString &pattern, keywordPatterns) {
            rule.pattern = QRegExp(pattern);
            rule.format = keywordFormat;
            highlightingRules.append(rule);
        }

        quotationFormat.setForeground(Qt::darkRed);
        rule.pattern = QRegExp("\".*\"");
        rule.format = quotationFormat;
        rule.tag = "quotation";
        highlightingRules.append(rule);
        
        QStringList functionPatterns;
        functionPatterns << "\\baddlw\\b" << "\\baddwf\\b" << "\\bandlw\\b"
            << "\\bandwf\\b" << "\\bbcf\\b" << "\\bbsf\\b"
            << "\\bbtfsc\\b" << "\\bbtfss\\b" << "\\bcall\\b"
            << "\\bclrf\\b" << "\\bclrwdt\\b" << "\\bclrw\\b"
            << "\\bcomf\\b" << "\\bdecf\\b" << "\\bdecfsz\\b"
            << "\\bgoto\\b" << "\\bincf\\b" << "\\bincfsz\\b"
            << "\\biorlw\\b" << "\\biorwf\\b" << "\\bmovf\\b"
            << "\\bmovlw\\b" << "\\bmovwf\\b" << "\\bnop\\b"
            << "\\boption\\b" << "\\bretfie\\b" << "\\bretlw\\b"
            << "\\breturn\\b" << "\\brlf\\b" << "\\brrf\\b"
            << "\\bsleep\\b" << "\\bsublw\\b" << "\\bsubwf\\b"
            << "\\bswapf\\b" << "\\btris\\b" << "\\bxorlw\\b"
            << "\\bxorwf\\b";

        functionFormat.setFontItalic(true);
        functionFormat.setForeground(Qt::blue);

        foreach (const QString &pattern, functionPatterns) {
            rule.pattern = QRegExp(pattern);
            rule.format = functionFormat;
            rule.tag = "function";
            highlightingRules.append(rule);
        }

        singleLineCommentFormat.setForeground(Qt::gray);
        rule.pattern = QRegExp(";[^\n]*");
        rule.format = singleLineCommentFormat;
        rule.tag = "lineComment";
        highlightingRules.append(rule);

        multiLineCommentFormat.setForeground(Qt::gray);
        commentStartExpression = QRegExp("/\\*");
        commentEndExpression = QRegExp("\\*/");
    }
    else
    {
        //NOTHING
    }

    /* C++ only! Keep it here for future usage.
    classFormat.setFontWeight(QFont::Bold);
    classFormat.setForeground(Qt::darkMagenta);
    rule.pattern = QRegExp("\\bQ[A-Za-z]+\\b");
    rule.format = classFormat;
    highlightingRules.append(rule);*/


}


void Highlighter::highlightBlock(const QString &text)
{
    setCurrentBlockState(0);

    int startIndex = 0;
    if (previousBlockState() != 1)
        startIndex = commentStartExpression.indexIn(text);

    while (startIndex >= 0)
    {
        int endIndex = commentEndExpression.indexIn(text, startIndex);
        int commentLength;
        if (endIndex == -1) {
            setCurrentBlockState(1);
            commentLength = text.length() - startIndex;
        } else {
            commentLength = endIndex - startIndex
                            + commentEndExpression.matchedLength();
        }
        setFormat(startIndex, commentLength, multiLineCommentFormat);
        startIndex = commentStartExpression.indexIn(text, startIndex + commentLength);
    }

    if (currentBlockState() == 0)
    {
        foreach (const HighlightingRule &rule, highlightingRules)
        {
            QRegExp expression(rule.pattern);
            int index = expression.indexIn(text);
            while (index >= 0) {
                int length = expression.matchedLength();
                setFormat(index, length, rule.format);
                index = expression.indexIn(text, index + length);
            }
        }
    }

}
