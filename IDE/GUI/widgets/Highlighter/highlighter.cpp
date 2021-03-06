/**
 * @brief
 * C++ Implementation: ...
 *
 * ...
 *
 * (C) copyright 2013, 2014 Moravia Microsystems, s.r.o.
 *
 * @author Erik Chalupa <erik.chalupa@moravia-microsystems.com>
 * @ingroup Highlighter
 * @file highlighter.cpp
 */


#include <QtGui>
#include "highlighter.h" 


Highlighter::Highlighter(QTextDocument *parent, SourceType type)
     : QSyntaxHighlighter(parent)
{
    //qDebug() << "Highlighter: Highlighter()";
    HighlightingRule rule;
    this->type = type;
/*    if (type == C)
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

        //functionFormat.setFontItalic(true);
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
        commentStartExpression = QRegExp("/\\*");*/
//        commentEndExpression = QRegExp("\\*/");
/*    }
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

        foreach (const QString &pattern, keywordPatterns)
        {
            rule.pattern = QRegExp(pattern);
            rule.format = keywordFormat;
            highlightingRules.append(rule);
        }
        
        quotationFormat.setForeground(Qt::darkRed);
        rule.pattern = QRegExp("\".*\"");
        rule.format = quotationFormat;
        rule.tag = "quotation";
        highlightingRules.append(rule);

        //functionFormat.setFontItalic(true);
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
        commentStartExpression = QRegExp("/\\*");*/
//        commentEndExpression = QRegExp("\\*/");
/*    }
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
            
        foreach (const QString &pattern, keywordPatterns)
        {
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
        
        //functionFormat.setFontItalic(true);
        functionFormat.setForeground(Qt::blue);
        
        foreach (const QString &pattern, functionPatterns)
        {
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
        commentStartExpression = QRegExp("/\\*");*/
//        commentEndExpression = QRegExp("\\*/");
/*    }
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

        //functionFormat.setFontItalic(true);
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
        commentStartExpression = QRegExp("/\\*");*/
//        commentEndExpression = QRegExp("\\*/");
/*    }
    else */if (type == PICOBLAZEASM)
    {
        //qDebug() << "PICOBLAZEASM-----------------------------";
        QStringList keywordPatterns;
        keywordPatterns << "#if"
                        << "#ifn"
                        << "#ifdef"
                        << "#ifndef"
                        << "#elseifb"
                        << "#elseifnb"
                        << "#else"       
                        << "#elseif"
                        << "#elseifn"
                        << "#elseifdef"
                        << "#elseifndef"
                        << "#endif"
                        << "#ifnb"
                        << "#ifb"
                        << "#while"
                        << "#endwhile"
                        << "(\\.)?failjmp"
                        << "(\\.)?default_jump"
                        << "(\\.)?device"
                        << "(\\.)?limit"
                        << "(\\.)?reg"
                        << "(\\.)?namereg"
                        << "(\\.)?address"
                        << "(\\.)?org"
                        << "(\\.)?define"
                        << "(\\.)?undefine"
                        << "(\\.)?undef"
                        << "(\\.)?equ"
                        << "(\\.)?constant"
                        << "(\\.)?set"
                        << "(\\.)?variable"
                        << "(\\.)?port"
                        << "(\\.)?data"
                        << "(\\.)?code"
                        << "(\\.)?db"
                        << "(\\.)?error"
                        << "(\\.)?warning"
                        << "(\\.)?list"
                        << "(\\.)?message"
                        << "(\\.)?nolist"
                        << "(\\.)?skip"
                        << "(\\.)?title"
                        << "(\\.)?expand"
                        << "(\\.)?noexpand"
                        << "(\\.)?local"
                        << "(\\.)?endmacro"
                        << "(\\.)?endm"
                        << "(\\.)?exitm"
                        << "(\\.)?repeat"
                        << "(\\.)?rept"
                        << "(\\.)?endrepeat"
                        << "(\\.)?endr"
                        << "(\\.)?autoreg"
                        << "(\\.)?autospr"
                        << "(\\.)?orgspr"
                        << "(\\.)?initspr"
                        << "(\\.)?mergespr"
                        << "(\\.)?macro"
                        << "(\\.)?end"
                        << "(\\.)?include"
                        << "(\\.)?string";
                        
        QStringList keywordPatternsFinal;
        foreach (const QString &pattern, keywordPatterns)
        {
            keywordPatternsFinal << "\\b" + pattern + "\\b";
        }

        keywordFormat.setForeground(Qt::darkGreen);
        keywordFormat.setFontWeight(QFont::Bold);

        foreach (const QString &pattern, keywordPatternsFinal)
        {
            rule.pattern = QRegExp(pattern);
            rule.pattern.setCaseSensitivity(Qt::CaseInsensitive);
            rule.format = keywordFormat;
            rule.tag = "directive";
            highlightingRules.append(rule);
            //qDebug() << "Added " << pattern;
        }

        QStringList functionPatterns;
        functionPatterns << "add"
                         << "addcy"
                         << "sub"
                         << "subcy"
                         << "compare"
                         << "load"
                         << "and"
                         << "or"
                         << "xor"
                         << "test"
                         << "store"
                         << "fetch"
                         << "sr0"
                         << "sr1"
                         << "srx"
                         << "sra"
                         << "rr"
                         << "sl0"
                         << "sl1"
                         << "slx"
                         << "sla"
                         << "rl"
                         << "input"
                         << "output"
                         << "hwbuild"
                         << "star"
                         << "testcy"
                         << "comparecy"
                         << "outputk"
                         << "jump([ \\t]|\\u00A0)+z"
                         << "jump([ \\t]|\\u00A0)+nz"
                         << "jump([ \\t]|\\u00A0)+c"
                         << "jump([ \\t]|\\u00A0)+nc"
                         << "call([ \\t]|\\u00A0)+z"
                         << "call([ \\t]|\\u00A0)+nz"
                         << "call([ \\t]|\\u00A0)+c"
                         << "call([ \\t]|\\u00A0)+nc"
                         << "return([ \\t]|\\u00A0)+z"
                         << "return([ \\t]|\\u00A0)+nz"
                         << "return([ \\t]|\\u00A0)+c"
                         << "return([ \\t]|\\u00A0)+nc"
                         << "return"
                         << "jump"
                         << "call"
                         << "regbank([ \\t]|\\u00A0)+A"
                         << "regbank([ \\t]|\\u00A0)+B"
                         << "returni([ \\t]|\\u00A0)+enable"
                         << "returni([ \\t]|\\u00A0)+disable"
                         << "enable([ \\t]|\\u00A0)+interrupt"
                         << "disable([ \\t]|\\u00A0)+interrupt"
                         << "nop";

        QStringList functionPatternsFinal;
        foreach (const QString &pattern, functionPatterns)
        {
            functionPatternsFinal << "\\b" + pattern + "\\b";
        }
        
        functionFormat.setForeground(Qt::darkBlue);
        functionFormat.setFontWeight(QFont::Bold);

        foreach (const QString &pattern, functionPatternsFinal)
        {
            rule.pattern = QRegExp(pattern);
            rule.pattern.setCaseSensitivity(Qt::CaseInsensitive);
            rule.format = functionFormat;
            rule.tag = "instruction";
            highlightingRules.append(rule);
            //qDebug() << "Added " << pattern;
        }

        
        QStringList specFunctionPatterns;
        specFunctionPatterns << "ldret"
                             << "ena"
                             << "dis"
                             << "retie"
                             << "retid"
                             << "cmp"
                             << "in"
                             << "out"
                             << "outk"
                             << "ld"
                             << "cmpcy"
                             << "st"
                             << "ft"
                             << "rb([ \\t]|\\u00A0)+A"
                             << "rb([ \\t]|\\u00A0)+B"
                             << "ret([ \\t]|\\u00A0)+z"
                             << "ret([ \\t]|\\u00A0)+nz"
                             << "ret([ \\t]|\\u00A0)+c"
                             << "ret([ \\t]|\\u00A0)+nc"
                             << "ret"
                             << "cpl2"
                             << "cpl"
                             << "inc"
                             << "dec"
                             << "setr"
                             << "clrr"
                             << "setb"
                             << "clrb"
                             << "djnz"
                             << "ijnz"
                             << "notb";

        QStringList specFunctionPatternsFinal;
        foreach (const QString &pattern, specFunctionPatterns)
        {
            specFunctionPatternsFinal << "\\b" + pattern + "\\b";
        }

                             
                             
        specFunctionFormat.setForeground(Qt::darkBlue);
        specFunctionFormat.setFontWeight(QFont::Bold);

        foreach (const QString &pattern, specFunctionPatternsFinal)
        {
            rule.pattern = QRegExp(pattern);
            rule.pattern.setCaseSensitivity(Qt::CaseInsensitive);
            rule.format = specFunctionFormat;
            rule.tag = "specinstruction";
            highlightingRules.append(rule);
            //qDebug() << "Added " << pattern;
        }

        QStringList macroPatterns;
        macroPatterns << "([^#]?)if"
                      << "([^#]?)elseif"
                      << "([^#]?)else"
                      << "([^#]?)endif"
                      << "([^#]?)while"
                      << "([^#]?)endw"
                      << "for"
                      << "endf";

        QStringList macroPatternsFinal;
        foreach (const QString &pattern, macroPatterns)
        {
            macroPatternsFinal << "\\b" + pattern + "\\b";
        }
                      
        macroFormat.setForeground(Qt::darkRed);
        macroFormat.setFontWeight(QFont::Bold);

        foreach (const QString &pattern, macroPatternsFinal)
        {
            rule.pattern = QRegExp(pattern);
            rule.pattern.setCaseSensitivity(Qt::CaseInsensitive);
            rule.format = macroFormat;
            rule.tag = "macro";
            highlightingRules.append(rule);
            //qDebug() << "Added " << pattern;
        }



        QStringList operandsPatterns;
        //pridat do keywords s {WS}
        operandsPatterns << "\\bS[0-9A-F]+\\b";

        operandsFormat.setFontWeight(QFont::Bold);
        operandsFormat.setForeground(Qt::darkBlue);

        foreach (const QString &pattern, operandsPatterns)
        {
            rule.pattern = QRegExp(pattern);
            rule.pattern.setCaseSensitivity(Qt::CaseInsensitive);
            rule.format = operandsFormat;
            rule.tag = "operand";
            highlightingRules.append(rule);
        }

        QStringList operatorsPatterns;
        operatorsPatterns << "\\bhigh\\b"
                          << "\\blow\\b"
                          << "\\bat\\b"
                          << "@"
                          << "#"
                          << "\\.\\."
                          << "\\+"
                          << "[^\\*]/[^\\*]"
                          << "\\-"
                          << "[^/]\\*[^/]"
                          << "!"
                          << "%"
                          << "<<"
                          << ">>"
                          << "&&"
                          << "\\|\\|"
                          << "&"
                          << "\\|"
                          << "\\^"
                          << "=="
                          << "<>"
                          << "!="
                          << "<"
                          << "<="
                          << ">"
                          << ">="
                          << "~"
                          << "\\$";

        operatorsFormat.setForeground(Qt::blue);

        foreach (const QString &pattern, operatorsPatterns)
        {
            rule.pattern = QRegExp(pattern);
            rule.pattern.setCaseSensitivity(Qt::CaseInsensitive);
            rule.format = operatorsFormat;
            rule.tag = "operator";
            highlightingRules.append(rule);
        }


        
        hexaNumbersFormat.setForeground(Qt::darkMagenta);
        rule.pattern = QRegExp("\\b(0(x|X)[0-9A-Fa-f]+)\\b|\\b([0-9A-Fa-f]+((h)|(H)))\\b");
        rule.format = hexaNumbersFormat;
        rule.tag = "hexadecimal";
        highlightingRules.append(rule);
        
        octaNumbersFormat.setForeground(Qt::darkMagenta);
        rule.pattern = QRegExp("\\b(0[1-9]*)\\b|\\b([0-9]+((q)|(Q)))\\b");
        rule.format = octaNumbersFormat;
        rule.tag = "octal";
        highlightingRules.append(rule);

        
        decNumbersFormat.setForeground(Qt::darkMagenta);
        rule.pattern = QRegExp("\\b([1-9][0-9]*)\\b|\\b([0-9]+((d)|(D)))\\b");
        rule.format = decNumbersFormat;
        rule.tag = "decimal";
        highlightingRules.append(rule);

        binNumbersFormat.setForeground(Qt::darkMagenta);
        rule.pattern = QRegExp("\\b(0(b|B)[0-1]+)\\b|\\b([0-1]+((b)|(B)))\\b");
        rule.format = binNumbersFormat;
        rule.tag = "binary";
        highlightingRules.append(rule);
            

        labelFormat.setFontItalic(true);
        rule.pattern = QRegExp("[a-zA-Z][0-9a-zA-Z_]*:");
        rule.format = labelFormat;
        rule.tag = "label";
        highlightingRules.append(rule);

        simpleQuotationFormat.setForeground(Qt::darkMagenta);
        rule.pattern = QRegExp("'.*'");
        rule.format = simpleQuotationFormat;
        rule.tag = "simplequotation";
        highlightingRules.append(rule);

        quotationFormat.setForeground(Qt::darkRed);
        rule.pattern = QRegExp("\".*\"");
        rule.format = quotationFormat;
        rule.tag = "quotation";
        highlightingRules.append(rule);
        
        singleLineCommentFormat.setForeground(Qt::gray);
        rule.pattern = QRegExp(";[^\r\n]*");
        rule.format = singleLineCommentFormat;
        rule.tag = "lineComment";
        highlightingRules.append(rule);

        multiLineCommentFormat.setForeground(Qt::gray);
        commentStartExpression = QRegExp("/\\*");
        commentEndExpression = QRegExp("\\*/");
    }
    else
    {
        qDebug() << "Highlighter: failed in given source type";
        qDebug() << "Highlighter: source type: " << type;
        //NOTHING
    }

    /* C++ only! Keep it here for future usage.
    classFormat.setFontWeight(QFont::Bold);
    classFormat.setForeground(Qt::darkMagenta);
    rule.pattern = QRegExp("\\bQ[A-Za-z]+\\b");
    rule.format = classFormat;
    highlightingRules.append(rule);*/

    //qDebug() << "Highlighter: return Highlighter()";

}


void Highlighter::highlightBlock(const QString &text)
{
    //qDebug() << "Highlighter: highlightBlock() : " << text;
    if (text != "")
    {
        setCurrentBlockState(0);

        int startIndex = 0;
        if (previousBlockState() != 1)
        {
            startIndex = commentStartExpression.indexIn(text);
        }
        else
        {
            //qDebug() << "Highlighter: currentBlockState = 1";
            setCurrentBlockState(1);
        }

        //qDebug() << "Highlighter: start" << startIndex << text;
        while (startIndex >= 0)
        {

            int endIndex = commentEndExpression.indexIn(text, startIndex);
            int commentLength;
            if (endIndex == -1)
            {
                setCurrentBlockState(1);
                commentLength = text.length() - startIndex;
            }
            else
            {
                //qDebug() << "Highlighter: currentBlockState = 0";
                commentLength = endIndex - startIndex + commentEndExpression.matchedLength();
                setCurrentBlockState(0);
                //commentLength = commentEndExpression.matchedLength() - startIndex;
            }
            setFormat(startIndex, commentLength, multiLineCommentFormat);
            startIndex = commentStartExpression.indexIn(text, startIndex + commentLength);
            //qDebug() << "Startindex:" << startIndex;
        }

        //qDebug() << "Highlighter: while1";
        if (currentBlockState() == 0)
        {
            //QRegExp expression;
            foreach (const HighlightingRule &rule, highlightingRules)
            {
                //expression.setPattern(rule.pattern.pattern());
                int index = rule.pattern.indexIn(text);
                while (index >= 0)
                {
                    int length = rule.pattern.matchedLength();
                    setFormat(index, length, rule.format);
                    index = rule.pattern.indexIn(text, index + length);
                }
            }
        }
    }
    else
    {
        if (1 == previousBlockState())
        {
            setCurrentBlockState(1);
        }
        else
        {
            setCurrentBlockState(0);
        }
    }
    //qDebug() << "Highlighter: return highlightBlock()";
}
