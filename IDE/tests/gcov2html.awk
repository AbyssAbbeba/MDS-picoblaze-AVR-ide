#! /usr/bin/gawk -f

BEGIN {
    print "<?xml version='1.0' encoding='utf-8'?>"
    print "<!DOCTYPE html PUBLIC \"-//W3C//DTD XHTML 1.1//EN\""
    print "        \"http://www.w3.org/TR/xhtml11/DTD/xhtml11.dtd\">"
    print "<html xmlns=\"http://www.w3.org/1999/xhtml\" xml:lang=\"en\">"
    print "<head>"
    print "    <meta http-equiv=\"Content-Type\" content=\"application/xhtml+xml; charset=utf-8\" />"
    print "    <meta name=\"content-language\" content=\"en\"/>"
    print "    <link rel=\"shortcut icon\" href=\"favicon.png\" type=\"image/png\"/>"
    print "    <title>" TITLE "</title>"
    print "    <style type=\"text/css\">"
    print "        td.exec {"
    print "            text-align: right;"
    print "            padding-right: 5px;"
    print "        }"
    print "        td.lineno {"
    print "            text-align: right;"
    print "            font-weight: bold;"
    print "            padding-right: 5px;"
    print "        }"
    print "        td.srcline {"
    print "            text-align: left;"
    print "            padding-left: 5px;"
    print "        }"
    print "        span.comment {"
    print "            font-style: italic;"
    print "            color: #888888;"
    print "        }"
    print "        span.doygen {"
    print "            font-style: italic;"
    print "            color: #8888DD;"
    print "        }"
    print "        span.doxytag {"
    print "            font-weight: bold;"
    print "            color: #CA60CA;"
    print "        }"
    print "        span.directive {"
    print "            color: #006E28;"
    print "        }"
    print "        span.string {"
    print "            color: #C00000;"
    print "        }"
    print "        span.char {"
    print "            color: #FF80E0;"
    print "        }"
    print "        span.char span.symbol {"
    print "            color: #FF80E0;"
    print "        }"
    print "        span.symbol {"
    print "            color: #008000;"
    print "        }"
    print "        span.dt {"
    print "            color: #0066FF;"
    print "        }"
    print "        span.kw {"
    print "            font-weight: bold;"
    print "        }"
    print "        span.number {"
    print "            color: #FF0000;"
    print "        }"
    print "    </style>"
    print "</head>"
    print "<body style=\"text-align: center\">"
    print "    <div>"
    print "        <a href=\"" INDEX "\"><img src=\"logo.png\" width=\"309\" height=\"83\" alt=\"\"/></a>"
    print "        <br/><i>Moravia Microsystems, s.r.o.</i><br/><br/>"
    print "    </div>"
    print "    <table style=\"background-color: #F0F0F0; width: 90%; margin: 0 auto; font-size: 14px; font-family: Monospace, Courier New;\">"
    print "        <col style=\"background-color: #90EE90;\"/>"
    print "        <col style=\"background-color: #90EE90;\"/>"
    print "        <col style=\"background-color: #FFFFFF;\"/>"
    print "        <tr style=\"background-color: skyblue\">"
    print "                <th colspan=\"3\">" TITLE "</th>"
    print "        </tr>"
    print "        <tr style=\"background-color: #ffffc0\">"
    print "            <th style=\"width: 5%\"> Executed </th>"
    print "            <th style=\"width: 5%\"> Line </th>"
    print "            <th> Source code </th>"
    print "        </tr>"

    mode = 0
    fc = 0
}

END {
    print "    </table>"
    print "    <div>"
    print "        <br/><br/>"
    print "        <hr style=\"width: 90%; margin: 0 auto; color: maroon\"/><br/>"
    print "        Generated by gcov2html.awk on " strftime ( "%c", systime() ) "."
    print "    </div>"
    print "</body>"
    print "</html>"
}

function syntaxHg ( line           ,token, token0, token1 ) {
    fc = 0
    if ( 0 == mode ) {
        if ( match(line, "\"[^\"]*\"") ) {
            token = substr(line, RSTART, RLENGTH)
            token1 = substr(line, RSTART + RLENGTH)
            token0 = syntaxHg(substr(line, 1, RSTART - 1))
            if ( 1 == fc ) {
                line = token0 "<span class=\"comment\">" xmlEntities(token token1) "</span>"
            } else if ( 2 == fc ) {
                line = token0 syntaxHg(token) syntaxHg(token1)
            } else if ( 3 == fc ) {
                line = token0 "<span class=\"doygen\">" syntaxHgDoxytags(xmlEntities(token token1)) "</span>"
            } else {
                line = token0 "<span class=\"string\">" xmlEntities(token) "</span>" syntaxHg(token1)
            }
        } else if ( match(line, "\"[^\"]*$") ) {
            token = substr(line, RSTART, RLENGTH)
            token0 = syntaxHg(substr(line, 1, RSTART - 1))
            if ( 1 == fc ) {
                line = token0 "<span class=\"comment\">" xmlEntities(token) "</span>"
            } else if ( 2 == fc ) {
                line = token0 syntaxHg(token)
            } else if ( 3 == fc ) {
                line = token0 "<span class=\"doygen\">" syntaxHgDoxytags(xmlEntities(token)) "</span>"
            } else {
                line = token0 "<span class=\"string\">" xmlEntities(token) "</span>"
                if ( 0 == mode ) {
                    mode = 3
                }
            }
        } else if ( match(line, /\/\/.*$/) ) {
            token = xmlEntities(substr(line, RSTART))
            token1 = substr(line, RSTART + 2, 1)
            token0 = syntaxHg(substr(line, 1, RSTART - 1))
            if ( 0 == mode ) {
                if ( "/" == token1 ) {
                    fc = 3
                    class = "doygen"
                    token = syntaxHgDoxytags(token)
                } else {
                    fc = 1
                    class = "comment"
                }
            }
            line = token0  "<span class=\"" class "\">" token "</span>"
        } else if ( match(line, /\/\*.*$/) ) {
            token = substr(line, RSTART, RLENGTH)
            token1 = substr(line, RSTART + 2, 1)
            token0 = syntaxHg(substr(line, 1, RSTART - 1))
            if ( "*" == token1 ) {
                if ( 0 == mode ) {
                    mode = 2
                }
            } else if ( 0 == mode ) {
                mode = 1
            }
            line = token0 syntaxHg(token)
            if ( ( 1 == mode ) || ( 2 == mode ) ) {
                fc = 2
            }
        } else if ( match(line, /^[[:space:]]*#.*$/) ) {
            line = "<span class=\"directive\">" xmlEntities(line) "</span>"
        } else {
            line = syntaxHgMinor(line)
        }

    # In multiline comment, or doxygen comment.
    } else if ( ( 1 == mode ) || ( 2 == mode ) ) {
        if ( match(line, /\*\//) ) {
            token1 = substr(line, RSTART + 2)
            token = xmlEntities(substr(line, 1, RSTART + 1))
            if ( 1 == mode ) {
                class = "comment"
            } else {
                class = "doygen"
                token = syntaxHgDoxytags(token)
            }
            mode = 0
            line = "<span class=\"" class "\">" token "</span>" syntaxHg(token1)
        } else if ( 1 == mode ) {
            line = "<span class=\"comment\">" xmlEntities(line) "</span>"
        } else if ( 2 == mode ) {
            line = "<span class=\"doygen\">" syntaxHgDoxytags(xmlEntities(line)) "</span>"
        }
    } else if ( 3 == mode ) {
        if ( match(line, "\"") ) {
            mode = 0
            line = "<span class=\"string\">" xmlEntities(substr(line, 1, RSTART)) "</span>" \
                   syntaxHg(substr(line, RSTART + 1))
        } else {
            line = "<span class=\"string\">" xmlEntities(line) "</span>"
        }
    }

    return line
}

function syntaxHgDoxytags ( string ) {
    if ( 0 == length(string) ) {
        return ""
    }

    buffer = ""
    string = "<span>" string "</span>"
    while ( match ( string, /[;&>]@(return|class|see|sa|arg|li|nosubgrouping|subpage|interface|(param(\[[^\[\]]*\])?)|defgroup|addtogroup|weakgroup|ref|page|struct|union|enum|def|file|namespace|package|brief|ingroup|name|mainpage|fn|var|typedef|author|authors)[<;&]/ ) ) {
        buffer = buffer substr(string, 1, RSTART) "<span class=\"doxytag\">" substr(string, RSTART + 1, RLENGTH - 2) "</span>"
        string = substr(string, RSTART + RLENGTH - 1)
    }
    string = buffer string
    return string
}

function syntaxHgMinor ( string ) {
    if ( 0 == length(string) ) {
        return ""
    }

    gsub ( /</,  "\x01", string )
    gsub ( />/,  "\x02", string )
    gsub ( / /,  "\x03", string )
    gsub ( /;/,  "\x04", string )
    gsub ( /\//, "\x05", string )
    gsub ( /=/,  "<span class=\"symbol\">=</span>",        string )
    gsub ( /&/,  "<span class=\"symbol\">\\&amp;</span>",  string )
    gsub ( /\x01/, "<span class=\"symbol\">\\&lt;</span>", string )
    gsub ( /\x02/, "<span class=\"symbol\">\\&gt;</span>", string )
    gsub ( /\x03/, "\\&nbsp;",                             string )
    gsub ( /\x04/, "<span class=\"symbol\">;</span>",      string )
    gsub ( /\x05/, "<span class=\"symbol\">/</span>",      string )

    gsub ( /[\+\-\*\|\^\:\(\)\[\]\{\}\,\\\%\!\~\?]/, "<span class=\"symbol\">&</span>", string )

    gsub ( /'[^']*'/, "<span class=\"char\">&</span>", string )

    string = "<span>" string "</span>"

    buffer = ""
    while ( match ( string, /[;&>](if|else|while|do|for|continue|break|switch|case|default|namespace|struct|union|return|enum|sizeof|using|export|typedef|class|friend|catch|try|throw|new|delete|virtual|constexpr|inline|goto|public:?|private:?|protected:?)[<;&]/ ) ) {
        buffer = buffer substr(string, 1, RSTART) "<span class=\"kw\">" substr(string, RSTART+1, RLENGTH-2) "</span>"
        string = substr(string, RSTART + RLENGTH - 1)
    }
    string = buffer string

    buffer = ""
    while ( match ( string, /[;&>](void|bool|char|short|int|long|float|double|const|static|auto|signed|unsigned|volatile|register|extern|mutable)[<;&]/ ) ) {
        buffer = buffer substr(string, 1, RSTART) "<span class=\"dt\">" substr(string, RSTART + 1, RLENGTH - 2) "</span>"
        string = substr(string, RSTART + RLENGTH - 1)
    }
    string = buffer string

    buffer = ""
    while ( match ( string, /[;&>]((0[xX][0-9a-fA-F]+)|([0-9]+)|(0[bB][01]+))[<;&]/ ) ) {
        buffer = buffer substr(string, 1, RSTART) "<span class=\"number\">" substr(string, RSTART + 1, RLENGTH - 2) "</span>"
        string = substr(string, RSTART + RLENGTH - 1)
    }
    string = buffer string

    return string
}

function xmlEntities ( string ) {
    gsub ( /&/,  "\\&amp;",  string )
    gsub ( /'/,  "\\&apos;", string )
    gsub ( "\"", "\\&quot;", string )
    gsub ( /</,  "\\&lt;",   string )
    gsub ( />/,  "\\&gt;",   string )
    gsub ( / /,  "\\&nbsp;", string )

    return string
}

( 1 != NR ) {
    execSumStr = substr ( $0, 1,  9 )
    lineNumStr = substr ( $0, 11, 5 )
    sourceLine = substr ( $0, 17 )

    gsub ( /^ +/, "", execSumStr )
    gsub ( /^ +/, "", lineNumStr )

    if ( "#####" == execSumStr ) {
        execStyle = "background-color: #ff8888"
        execSumStr="0"
    } else if ( "-" == execSumStr ) {
        execStyle = "background-color: #90EE90"
        execSumStr=""
    } else {
        execStyle = "background-color: #50ff50"
    }

    print "        <tr>"
    print "            <td class=\"exec\" style=\"" execStyle "\">" execSumStr "</td>"
    print "            <td class=\"lineno\">" lineNumStr "</td>"
    print "            <td class=\"srcline\">" syntaxHg(sourceLine) "</td>"
    print "        </tr>"
}