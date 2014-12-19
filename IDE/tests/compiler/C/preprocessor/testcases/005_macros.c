// Check function and variadic macros, and `#' and `##' operators.
#define mc0(a,b,c) a + b + c | "a + b + c"
#define mc1( a , b , c ) a + #b + # c + # + #d | "a + #b + # c + # + #d"
#define mc2(...) __VA_ARGS__ "__VA_ARGS__"
#define mc3( ... ) #__VA_ARGS__ "#__VA_ARGS__"
#define mc4(a, b, ...) (#a, b, __VA_ARGS__) (a, #b,   ##  __VA_ARGS__) (a, #b  |##__VA_ARGS__) "(#a, b, __VA_ARGS__) (a, #b,   ##  __VA_ARGS__) (a, #b  |##__VA_ARGS__)"
#define mc5(a, b, c...) (a, b, c) (#a, b, ##c) "(a, b, c) (#a, b, ##c)"
#define mc6(abc,def,ghi) abc ## def ## ghi "abc ## def ## ghi"
#define mc7( abc , def , ghi )  abc##def   ##   ghi ## 123 ## ## yyy xxx "abc##def   ##   ghi ## 123 ## ## yyy xxx"
#define mc8(   abc  ,   def , ghi       ) (abc,def,ghi) "(abc,def,ghi)"
mc0(1,2,3)
mc1(1,2,3)
mc2(1,2,3)
mc3(1, 2, 3)
mc4(1, 2, 3, 4)
mc4(1, 2)
mc5(1, 2, 3, 4)
mc5(1, 2)
mc6(ghi,def,abc)
mc7(   abc  ,   def , ghi       )
mc8(11,22,33)

// Random check from the ISO/IEC 9899:1999 standard.
#define t(x,y,z) x ## y ## z
int j[] = { t(1,2,3), t(,4,5), t(6,,7), t(8,9,), t(10,,), t(,11,), t(,,12), t(,,) };
// --> "int j[] = { 123, 45, 67, 89, 10, 11, 12, };"
