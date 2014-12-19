// Random check from the ISO/IEC 9899:1999 standard.
#define VERSION 2
#if VERSION == 1
    #define INCFILE "include_0.h"
#elif VERSION == 2
    #define INCFILE "include_1.h"
#else
    #define INCFILE "no-file.h"
#endif
#include INCFILE