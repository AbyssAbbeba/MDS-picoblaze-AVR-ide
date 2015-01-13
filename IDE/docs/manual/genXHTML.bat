@echo off

%1 %4 "xhtml,charset=utf-8,next,index=2,5" "-utf8" > %5
%2 "\def\filename{{MDS_manual}{idx}{4dx}{ind}} \input idxmake.4ht" > %5
%3 -o %6 %7 > %5
%1 %4 "xhtml,charset=utf-8,next,index=2,5" "-utf8" > %5
