#! /usr/bin/gawk -f

BEGIN {
    print "<?xml version='1.0' encoding='utf-8'?>"
    print "<!DOCTYPE html PUBLIC \"-//W3C//DTD XHTML 1.1//EN\""
    print "        \"http://www.w3.org/TR/xhtml11/DTD/xhtml11.dtd\">"
    print "<html xmlns=\"http://www.w3.org/1999/xhtml\" xml:lang=\"en\">"
    print "<head>"
    print "    <meta http-equiv=\"Content-Type\" content=\"application/xhtml+xml; charset=utf-8\"/>"
    print "    <meta name=\"content-language\" content=\"en\"/>"
    print "    <link rel=\"shortcut icon\" href=\"favicon.png\" type=\"image/png\"/>"
    print "    <title>" TITLE "</title>"
    print "    <style type=\"text/css\">"
    print "        span.error {"
    print "            color: #FFFFFF;"
    print "            background-color: #F82121;"
    print "        }"
    print "        span.warning {"
    print "            color: #FFFFFF;"
    print "            background-color: #E87310;"
    print "        }"
    print "        span.note {"
    print "            color: #EE8833;"
    print "        }"
    print "        span.filename {"
    print "            font-style: italic;"
    print "            background-color: #EEEEEE;"
    print "        }"
    print "        span.pointer {"
    print "            color: #FFFFFF;"
    print "            font-weight: bold;"
    print "            background-color: #000000;"
    print "        }"
    print "        span.scanning {"
    print "            color: #BF51DD;"
    print "        }"
    print "        span.building {"
    print "            color: #12B218;"
    print "        }"
    print "        span.linking {"
    print "            color: #BF524B;"
    print "        }"
    print "        span.custom {"
    print "            color: #3F52E3;"
    print "        }"
    print "        span.status {"
    print "            color: #555555;"
    print "        }"
    print "        span.progress {"
    print "            color: #555555;"
    print "            font-weight: bold;"
    print "        }"
    print "        span.mds_err {"
    print "            color: #DD0000;"
    print "        }"
    print "        span.mds_ok {"
    print "            color: #00AA00;"
    print "        }"
    print "    </style>"
    print "</head>"
    print "<body>"
    print "    <div style=\"text-align: center\">"
    print "        <a href=\"index.html\"><img src=\"logo.png\" width=\"309\" height=\"83\" alt=\"\"/></a>"
    print "        <br/><i>Moravia Microsystems, s.r.o.</i><br/><br/>"
    print "    </div>"
    print "    <div style=\"font-size: 14px; font-family: DejaVu Sans Mono, Monospace, Courier New\">"
}

END {
    print "    </div>"
    print "    <div style=\"text-align: center\">"
    print "        <br/><br/>"
    print "        <hr style=\"width: 90%; margin: 0 auto\"/><br/>"
    print "        Generated by BuildLog2html.awk on " strftime ( "%c", systime() ) "."
    print "    </div>"
    print "</body>"
    print "</html>"
}

{
    gsub ( /&/, "\\&amp;", $0 )
    gsub ( />/, "\\&gt;",  $0 )
    gsub ( /</, "\\&lt;",  $0 )
}

/^ *\^$/ {
    $0 = substr ( $0, 2, length($0) - 2 ) " <span class=\"pointer\">" substr ( $0, length($0), 1 ) "</span>"
}

{
    sub ( / fatal error: /,"<span class=\"error\">&</span> ",   $0 )
    sub ( / [Ee]rror: /,   "<span class=\"error\">&</span> ",   $0 )
    sub ( / [Ww]arning: /, "<span class=\"warning\">&</span> ", $0 )
    sub ( / note: /,       "<span class=\"note\">&</span> ",    $0 )
    sub ( /^[^ :]+(:[\.[:digit:]]+((:|-)[\.[:digit:]]+)?)?:/, "<span class=\"filename\">&</span>", $0 )

    sub ( /Scanning dependencies of target [[:alpha:]]+$/, "<span class=\"scanning\">&</span>", $0 )
    sub ( /Building CXX object .*$/, "<span class=\"building\">&</span>", $0 )
    sub ( /Linking CXX [ [:alpha:]]+.*$/, "<span class=\"linking\">&</span>", $0 )
    sub ( /\[((FLEX)|(BISON))\].*$/,  "<span class=\"custom\">&</span>", $0 )
    sub ( /^--.*$/,  "<span class=\"status\">&</span>", $0 )
    gsub ( /\[ *[[:digit:]]+%\]/,  "<span class=\"progress\">&</span>", $0 )


    sub ( /\[ERROR\]/, "<span class=\"mds_err\">&</span> ", $0 )
    sub ( /\[OK\]/,    "<span class=\"mds_ok\">&</span> ",  $0 )

    gsub ( /  /, " \\&nbsp;", $0 )
}

{
    print ( $0 "<br/>" )
}
