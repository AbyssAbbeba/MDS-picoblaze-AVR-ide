// Check arithmetic expressions #1.
#if 7 == 0b1+'\03'*(1<<1)
    #include "include_0.h"
#elif 1 == 1
    error A0
#elif '\0'
    error A1
#else
    error A2
#endif

// Check arithmetic expressions #1.
#if 1 << 2 * 3 != 0x40
    error B0
#elif 5 % 3 > 2
    error B1
#elif \
( 64 == 0100 ) == ( 0x40 == '@' )
    #include "include_1.h"
#else
    error B2
#end\
if

// Check the `sizeof' unary operator.
#if 1 != sizeof(char)
    1 != sizeof(char)
#else
    1 == sizeof(char)
#endif
#if 1 != sizeof(short)
    1 != sizeof(short)
#else
    1 == sizeof(short)
#endif
#if 2 != sizeof(int)
    2 != sizeof(int)
#else
    2 == sizeof(int)
#endif
#if 2 != sizeof(unsigned char *)
    2 != sizeof(unsigned char *)
#else
    2 == sizeof(unsigned char *)
#endif
#if 4 != sizeof(float)
    4 != sizeof(float)
#else
    4 == sizeof(float)
#endif
