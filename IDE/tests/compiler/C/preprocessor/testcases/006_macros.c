// Random check from the ISO/IEC 9899:1999 standard.
#define hash_hash # ## #
#define mkstr(a) # a
#define in_between(a) mkstr(a)
#define join(c, d) in_between(c hash_hash d)
char p[] = join(x, y); // --> "char p[] = "x ## y";"
