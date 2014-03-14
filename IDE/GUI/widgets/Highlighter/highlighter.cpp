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
                        << "(\\.)?messg"
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
                        << "(\\.)?include";
                        
        QStringList keywordPatternsFinal;
        foreach (const QString &pattern, keywordPatterns)
        {
            keywordPatternsFinal << "\\b" + pattern + "\\b";
            keywordPatternsFinal << "\\b" + pattern.toUpper() + "\\b";
        }

                        
        /*<< "\\bequ\\b" <<"#endw"        "\\bEQU\\b"
            << "\\badd\\b" << "\\baddcy\\b" << "\\band\\b"
            << "\\bcall\\b" << "\\bcompare\\b" << "\\bcomparecy\\b"
            << "\\bdisable interrupt\\b" << "\\benable interrupt\\b" << "\\bfetch\\b"
            << "\\bhwbuild\\b" << "\\binput\\b" << "\\bjump\\b"
            << "\\bload\\b" << "\\bload&return\\b" << "\\bor\\b"
            << "\\boutput\\b" << "\\bregbank\\b"
            << "\\breturn\\b" << "\\breturni disable\\b" << "\\breturni enable\\b"
            << "\\brl\\b" << "\\brr\\b" << "\\bsla\\b"
            << "\\bslx\\b" << "\\bsl0\\b" << "\\bsl1\\b"
            << "\\bsra\\b" << "\\bsrx\\b" << "\\bsr0\\b"
            << "\\bsr1\\b" << "\\bstar\\b" << "\\bstore\\b"
            << "\\bsub\\b" << "\\bsubcy\\b" << "\\btest\\b"
            << "\\btestcy\\b" << "\\bxor\\b"
            << "\\bADD\\b" << "\\bADDCY\\b" << "\\bAND\\b"
            << "\\bCALL\\b" << "\\bCOMPARE\\b" << "\\bCOMPARECY\\b"
            << "\\bDISABLE INTERRUPT\\b" << "\\bENABLE INTERRUPT\\b" << "\\bFETCH\\b"
            << "\\bHWBUILD\\b" << "\\bINPUT\\b" << "\\bJUMP\\b"
            << "\\bLOAD\\b" << "\\bLOAD&RETURN\\b" << "\\bOR\\b"
            << "\\bOUTPUT\\b" << "\\bREGBANK\\b"
            << "\\bRETURN\\b" << "\\bRETURNI DISABLE\\b" << "\\bRETURNI ENABLE\\b"
            << "\\bRL\\b" << "\\bRR\\b" << "\\bSLA\\b"
            << "\\bSLX\\b" << "\\bSL0\\b" << "\\bSL1\\b"
            << "\\bSRA\\b" << "\\bSRX\\b" << "\\bSR0\\b"
            << "\\bSR1\\b" << "\\bSTAR\\b" << "\\bSTORE\\b"
            << "\\bSUB\\b" << "\\bSUBCY\\b" << "\\bTEST\\b"
            << "\\bTESTCY\\b" << "\\bXOR\\b"
            << "\\bend\\b" << "\\bEND\\b"
            << "\\bena\\b" << "\\bENA\\b"
            << "\\bdis\\b" << "\\bDIS\\b"
            << "\\bretie\\b" << "\\bRETIE\\b"
            << "\\bretid\\b" << "\\bRETID\\b"
            << "\\bret\\b" << "\\bRET\\b"
            << "\\bcmp\\b" << "\\bCMP\\b"
            << "\\bin\\b" << "\\bIN\\b"
            << "\\bout\\b" << "\\bOUT\\b"
            << "\\bld\\b" << "\\bLD\\b"
            << "\\brt_if\\b" << "\\bRT_IF\\b"
            << "\\brt_else\\b" << "\\bRT_ELSE\\b"
            << "\\brt_elsif\\b" << "\\bRT_ELSIF\\b"
            << "\\brt_endif\\b" << "\\bRT_ENDIF\\b"
            << "\\brt_while\\b" << "\\bRT_WHILE\\b"
            << "\\brt_endw\\b" << "\\bRT_ENDW\\b"
            << "\\brt_for\\b" << "\\bRT_FOR\\b"
            << "\\brt_endf\\b" << "\\bRT_ENDF\\b"
            << "\\bRETURNI_ENABLE_INST\\b" << "\\breturni_enable_inst\\b"
            << "\\bRETURNI_DISABLE_INST\\b" << "\\breturni_disable_inst\\b"
            << "\\bENA_INT_INST\\b" << "\\bena_int_inst\\b"
            << "\\bDIS_INT_INST\\b" << "\\bdis_int_inst\\b"
            << "\\bOUTPUTK\\b" << "\\boutputk\\b"
            << "\\bLIMIT\\b" << "\\blimit\\b"
            << "\\bDEVICE\\b" << "\\bdevice\\b"
            << "\\bREG\\b" << "\\breg\\b"
            << "\\bNAMEREG\\b" << "\\bnamereg\\b"
            << "\\bADDRESS\\b" << "\\baddress\\b"
            << "\\bORG\\b" << "\\borg\\b"
            << "\\bDEFINE\\b" << "\\bdefine\\b"
            << "\\bUNDEFINE\\b" << "\\bundefine\\b"
            << "\\bUNDEF\\b" << "\\bundef\\b"
            << "\\bCONSTANT\\b" << "\\bconstant\\b"
            << "\\bSET\\b" << "\\bset\\b"
            << "\\bVARIABLE\\b" << "\\bvariable\\b"
            << "\\bWHILE\\b" << "\\bwhile\\b"
            << "\\bENDWHILE\\b" << "\\bendwhile\\b"
            << "\\bENDW\\b" << "\\bendw\\b"
            << "\\bPORT\\b" << "\\bport\\b"
            << "\\bDATA\\b" << "\\bdata\\b"
            << "\\bCODE\\b" << "\\bcode\\b"
            << "\\bDB\\b" << "\\bdb\\b"
            << "\\bERROR\\b" << "\\berror\\b"
            << "\\bWARNING\\b" << "\\bwarning\\b"
            << "\\bLIST\\b" << "\\blist\\b"
            << "\\bMESSG\\b" << "\\bmessg\\b"
            << "\\bNOLIST\\b" << "\\bnolist\\b"
            << "\\bSKIP\\b" << "\\bskip\\b"
            << "\\bTITLE\\b" << "\\btitle\\b"
            << "\\bEXPAND\\b" << "\\bexpand\\b"
            << "\\bNOEXPAND\\b" << "\\bnoexpand\\b"
            << "\\bIF\\b" << "\\bif\\b"
            << "\\bIFN\\b" << "\\bifn\\b"
            << "\\bIFDEF\\b" << "\\bifdef\\b"
            << "\\bIFNDEF\\b" << "\\bifndef\\b"
            << "\\bELSEIFB\\b" << "\\belseifb\\b"
            << "\\bELSEIFNB\\b" << "\\belseifnb\\b"
            << "\\bELSE\\b" << "\\belse\\b"
            << "\\bELSEIF\\b" << "\\belseif\\b"
            << "\\bELSEIFN\\b" << "\\belseifn\\b"
            << "\\bELSEIFDEF\\b" << "\\belseifdef\\b"
            << "\\bELSEIFNDEF\\b" << "\\belseifndef\\b"
            << "\\bENDIF\\b" << "\\bendif\\b"
            << "\\bLOCAL\\b" << "\\blocal\\b"
            << "\\bIFNB\\b" << "\\bifnb\\b"
            << "\\bIFB\\b" << "\\bifb\\b"
            << "\\bENDMACRO\\b" << "\\bendmacro\\b"
            << "\\bENDM\\b" << "\\bendm\\b"
            << "\\bEXITM\\b" << "\\bexitm\\b"
            << "\\bREPEAT\\b" << "\\brepeat\\b"
            << "\\bREPT\\b" << "\\brept\\b"
            << "\\bENDREPEAT\\b" << "\\bendrepeat\\b"
            << "\\bENDR\\b" << "\\bendr\\b"
            << "\\bAUTOREG\\b" << "\\bautoreg\\b"
            << "\\bAUTOSPR\\b" << "\\bautospr\\b"
            << "\\bMACRO\\b" << "\\bmacro\\b"
            << "\\bORGSPR\\b" << "\\borgspr\\b"
            << "\\bINITSPR\\b" << "\\binitspr\\b"
            << "\\bMERGESPR\\b" << "\\bmergespr\\b"
            << "\\bINCLUDE\\b" << "\\binclude\\b";*/
            //vsechny s {WS}
            //RETURNI_ENABLE_INST predelat na returni{WS}enable
            //ENA_INT_INST predelat na enable{WS}interrupt

        keywordFormat.setForeground(Qt::darkGreen);
        keywordFormat.setFontWeight(QFont::Bold);

        foreach (const QString &pattern, keywordPatternsFinal)
        {
            rule.pattern = QRegExp(pattern);
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
                         << "disable([ \\t]|\\u00A0)+interrupt";

        QStringList functionPatternsFinal;
        foreach (const QString &pattern, functionPatterns)
        {
            functionPatternsFinal << "\\b" + pattern + "\\b";
            functionPatternsFinal << "\\b" + pattern.toUpper() + "\\b";
        }
        
        functionFormat.setForeground(Qt::darkBlue);
        functionFormat.setFontWeight(QFont::Bold);

        foreach (const QString &pattern, functionPatternsFinal)
        {
            rule.pattern = QRegExp(pattern);
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
                             << "ijnz";

        QStringList specFunctionPatternsFinal;
        foreach (const QString &pattern, specFunctionPatterns)
        {
            specFunctionPatternsFinal << "\\b" + pattern + "\\b";
            specFunctionPatternsFinal << "\\b" + pattern.toUpper() + "\\b";
        }

                             
                             
        specFunctionFormat.setForeground(Qt::darkBlue);
        specFunctionFormat.setFontWeight(QFont::Bold);

        foreach (const QString &pattern, specFunctionPatternsFinal)
        {
            rule.pattern = QRegExp(pattern);
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
            macroPatternsFinal << "\\b" + pattern.toUpper() + "\\b";
        }
                      
        macroFormat.setForeground(Qt::darkRed);
        macroFormat.setFontWeight(QFont::Bold);

        foreach (const QString &pattern, macroPatternsFinal)
        {
            rule.pattern = QRegExp(pattern);
            rule.format = macroFormat;
            rule.tag = "macro";
            highlightingRules.append(rule);
            //qDebug() << "Added " << pattern;
        }



        QStringList operandsPatterns;
        //pridat do keywords s {WS}
        operandsPatterns << "\\bS[0-9A-F]+\\b"
                         << "\\bs[0-9A-F]+\\b";

        operandsFormat.setFontWeight(QFont::Bold);
        operandsFormat.setForeground(Qt::darkBlue);

        foreach (const QString &pattern, operandsPatterns)
        {
            rule.pattern = QRegExp(pattern);
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
                          << "/"
                          << "\\-"
                          << "\\*"
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
                commentLength = endIndex - startIndex + commentEndExpression.matchedLength();
            }
            setFormat(startIndex, commentLength, multiLineCommentFormat);
            startIndex = commentStartExpression.indexIn(text, startIndex + commentLength);
            //qDebug() << "Startindex:" << startIndex;
        }

        //qDebug() << "Highlighter: while1";
        if (currentBlockState() == 0)
        {
            foreach (const HighlightingRule &rule, highlightingRules)
            {
                QRegExp expression(rule.pattern);
                int index = expression.indexIn(text);
                while (index >= 0)
                {
                    int length = expression.matchedLength();
                    setFormat(index, length, rule.format);
                    index = expression.indexIn(text, index + length);
                }
            }
        }
    }
    //qDebug() << "Highlighter: return highlightBlock()";
}
