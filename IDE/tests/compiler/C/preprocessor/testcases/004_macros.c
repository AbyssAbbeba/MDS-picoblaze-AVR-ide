// Check object macro definition obscured by line merges and trigraphs (--> "#define OBJMACRO 1020").
/\
*
*/ %: /*
*/ defi??/
ne OBJ\
MACRO   10??/
20

// Check #ifdef
#ifdef OBJMACRO
    // Check macro expansion within a preprocessor expression.
    #if OBJMACRO == 1020
        // Check `#<white-space>define', empty object macro, and a macro within another macro.
        #  define bar
        #  define xxx OBJMACRO / 2
    #else
        error A0
    #endif
#else
    error B0
#endif

// Check the `defined' unary operator.
#if defined   xxx  //
    // Check function macro.
    #define Func_Macro(a) a + 1
#else
    error C0
#endif

#define x y

// Check #ifndef
#ifndef bar
    error D0
#else
    // Check macro expansions.
    int x = xxx; // --> "int y = 1020 / 2;"
    int x = Func_Macro(xxx); // --> "int y = 1020 / 2 + 1;"
#endif

// Check combination of `defined' operators.
#if !defined(notdefined) && defined(  Func_Macro   )
    Func_Macro;
#else
    error E0
#endif

// Check macro expansion with string and character constants.
char x xor_eq 'x' bitor 0x14; // --> "char y ^= 'x' | 0x14;"
const char * x = "OBJMACRO"; // --> "const char * y = "OBJMACRO";"

// Check direct macro expansion loop.
#define SELF_REPEAT SELF_REPEAT + 1
SELF_REPEAT

// Check in-direct macro expansion loop.
#define SELF_REPEAT_LOOP SELF_REPEAT_LOOP_1 + 1
#define SELF_REPEAT_LOOP_1 SELF_REPEAT_LOOP + 1
SELF_REPEAT_LOOP
