/*
 * Random check from the ISO/IEC 9899:1999 standard.
 */
#define debug(...) fprintf(stderr, __VA_ARGS__)
#define showlist(...) puts(#__VA_ARGS__)
#define report(test, ...) ((test)?puts(#test):\
printf(__VA_ARGS__))

debug("Flag");
debug("X = %d\n", x);
showlist(The first, second, and third items.);
report(x>y, "x is %d but y is %d", x, y);

// Result is supposed to be:
/*
fprintf(stderr, "Flag");
fprintf(stderr, "X = %d\n", x);
puts( "The first, second, and third items." );
((x>y)?puts("x>y"):printf("x is %d but y is %d", x, y));
*/