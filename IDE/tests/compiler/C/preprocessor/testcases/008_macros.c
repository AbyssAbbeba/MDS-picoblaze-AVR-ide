/*
 * Random check from the ISO/IEC 9899:1999 standard.
 */

#define str(s) # s
#define xstr(s) str(s)
#define debug(s, t) printf("x" # s "= %d, x" # t "= %s", \
x ## s, x ## t)
#define INCFILE(n) vers ## n
#define glue(a, b) a ## b
#define xglue(a, b) glue(a, b)
#define HIGHLOW "hello"
#define LOW LOW ", world"

debug(1, 2);
fputs(str(strncmp("abc\0d", "abc", '\4') == 0) str(: @\n), s);
include xstr(INCFILE(2).h)
glue(HIGH, LOW);
xglue(HIGH, LOW)

// Result is supposed to be:
/*
printf("x" "1" "= %d, x" "2" "= %s", x1, x2);
fputs("strncmp(\"abc\\0d\", \"abc\", '\\4') == 0" ": @\n", s);
include "vers2.h"
"hello";
"hello" ", world"
*/

// Explanation for glue and xglue:
/*
glue(HIGH, LOW);
--> HIGH ## LOW
--> HIGHLOW
--> "hello"

xglue(HIGH, LOW)
--> glue(HIGH, LOW ", world")
--> HIGH ## LOW ", world"
--> HIGHLOW ", world"
--> "hello" ", world"
*/
