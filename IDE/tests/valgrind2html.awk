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
    print "        span.fatal {"
    print "            color: #FFFFFF;"
    print "            font-weight: bold;"
    print "            background-color: #FF0000;"
    print "        }"
    print "        span.header {"
    print "            font-weight: bold;"
    print "        }"
    print "        span.address {"
    print "            color: #0088DD;"
    print "        }"
    print "        span.source {"
    print "            color: #0000FF;"
    print "            background-color: #F0F0FF;"
    print "        }"
    print "        span.lib {"
    print "            color: #00CC00;"
    print "            font-style: italic;"
    print "        }"
    print "        span.firstCol {"
    print "            color: #888888;"
    print "        }"
    print "    </style>"
    print "</head>"
    print "<body style=\"overflow-x:scroll;\">"
    print "    <div style=\"text-align: center\">"
    print "        <a href=\"index.html\"><img src=\"logo.png\" width=\"309\" height=\"83\" alt=\"\"/></a>"
    print "        <br/><i>Moravia Microsystems, s.r.o.</i><br/><br/>"
    print "    </div>"
    print "    <div style=\"font-size: 14px; font-family: DejaVu Sans Mono, Monospace, Courier New;\">"

    summaryDiv = 0
}

END {
    print "    </div>"
    print "    <div style=\"text-align: center\">"
    print "        <br/><br/>"
    print "        <hr style=\"width: 90%; margin: 0 auto\"/><br/>"
    print "        Generated by valgrind2html.awk on " strftime ( "%c", systime() ) "."
    print "    </div>"
    print "</body>"
    print "</html>"
}

/^==[0-9]+== (HEAP SUMMARY:|LEAK SUMMARY:)$/ {
    print "    <div style=\"font-weight: bold; border: 1px solid #000000\">"
    summaryDiv++
    if ( $0 ~ /LEAK SUMMARY:/ ) {
        summaryDiv++
    }
}

( summaryDiv ) && /^==[0-9]+== *$/ {
    summaryDiv--
    if ( 0 == summaryDiv ) {
        print "    </div>"
    }
}

{
    gsub ( /&/,  "\\&amp;",   $0 )
    gsub ( />/,  "\\&gt;",    $0 )
    gsub ( /</,  "\\&lt;",    $0 )
    gsub ( / /, "\\&nbsp;", $0 )

    sub ( /^==[0-9]+==/, "<span class=\"firstCol\">&</span>", $0 )
    sub ( /((by)|(at))&nbsp;0[xX][0-9A-Fa-f]+:/, "<span class=\"address\">&</span>", $0 )
    sub ( /\([^\)]+:[0-9]+\)$/, "<span class=\"source\">&</span>", $0 )
    sub ( /\(in&nbsp;[^\)]+\)$/, "<span class=\"lib\">&</span>", $0 )
    sub ( /Conditional&nbsp;jump&nbsp;or&nbsp;move&nbsp;depends&nbsp;on&nbsp;uninitialised&nbsp;value\(s\)$/, "<span class=\"header\">&</span>", $0 )
    sub ( /Use&nbsp;of&nbsp;uninitialised&nbsp;value&nbsp;of&nbsp;size&nbsp;[0-9]+$/, "<span class=\"header\">&</span>", $0 )
    sub ( /Invalid&nbsp;((read)|(write))&nbsp;of&nbsp;size&nbsp;[0-9]+$/, "<span class=\"header\">&</span>", $0 )
    sub ( /Uninitialised&nbsp;value&nbsp;was&nbsp;created&nbsp;by&nbsp;a&nbsp;heap&nbsp;allocation$/, "<span class=\"header\">&</span>", $0 )
    sub ( /[0-9]+&nbsp;([0-9]+&nbsp;direct,&nbsp;[0-9]+&nbsp;indirect)&nbsp;bytes&nbsp;in&nbsp;[0-9]+&nbsp;blocks&nbsp;are&nbsp;[[:alpha:]]+&nbsp;lost&nbsp;in&nbsp;loss&nbsp;record&nbsp;[0-9]+&nbsp;of&nbsp;[0-9]+/, "<span class=\"header\">&</span>", $0 )
    sub ( /[0-9]+&nbsp;bytes&nbsp;in&nbsp;[0-9]+&nbsp;blocks&nbsp;are&nbsp;[[:alpha:]]+&nbsp;lost&nbsp;in&nbsp;loss&nbsp;record&nbsp;[0-9]+&nbsp;of&nbsp;[0-9]+$/, "<span class=\"header\">&</span>", $0 )
    sub ( /Process&nbsp;terminating&nbsp;with&nbsp;default&nbsp;action&nbsp;of&nbsp;signal&nbsp;[0-9]+&nbsp;\([[:alpha:]]+\)$/, "<span class=\"fatal\">&</span>", $0 )


    print ( $0 "<br/>" )
}
