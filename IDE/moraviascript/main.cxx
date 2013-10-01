#include "MScriptCore.h"
#include "MScriptStrategy.h"
#include <iostream>

int main ( int argc, char ** argv )
{
    std::string script =
//         "a = ( b * c + d );\n"          //  1
//         "a = b * c;\n"          //  1
        "a = ( b * c + d );\n"          //  1
        "a = ( b * c + d );\n"          //  1
        "a = b = c = d;\n"          //  1
        "a += ( b -= c );\n"          //  1
        "a = ( b + x() ) || ( c + y() );\n"          //  1
        "( b + x() ) || ( c + y() );\n"          //  1
        "a = ( b++ + x() ) && ( ++c + y() );\n"          //  1
        "x = a ? b : c;\n"          //  1
        "x = (a, b, c);\n"          //  1
        "x = a ( b ( c ) );\n"          //  1
        "x = ( 1 + 2 ) * ( 4 - 1 ) / ( 9 - 6);\n"          //  1
        "if ( 2 == a )\n"               //  1
        "{\n"                           //  2
        "    a = ( b * c + d );\n"      //  3
        "    b = 4 - d;\n"              //  4
        "}\n"                           //  5
        "else\n"                        //  6
        "{\n"                           //  7
        "    d -= c % 9;\n"             //  8
        "}\n"                           //  9
        "\n"                            // 10
        "for ( i = 0; i < 10; i++ )\n"  // 11
        "{\n"                           // 12
        "    x = i + 5;\n"              // 13
        "}\n"                           // 14
        "switch ( xxx )\n"              // 15
        "{\n"                           // 16
        "    case 1:\n"                 // 17
        "    case 2:\n"                 // 18
        "        xxx += 4;\n"           // 19
        "        break;\n"              // 20
        "    default:\n"                // 21
        "        break;\n"              // 22
        "}\n"                           // 23
        "func testFunc ( a, b, c = 0, d = \"x\" )\n" // 24
        "{\n"                           // 25
        "    return ( a * b );\n"       // 26
        "}\n";                          // 27

    MScriptStrategy strategy;
    MScriptCore core(&strategy);

    for ( std::vector<std::string>::const_iterator msg = core.getMessages().cbegin();
          msg != core.getMessages().cend();
          msg++ )
    {
        std::cout << *msg << std::endl;
    }
    core.clearMessages();

    core.loadScript(script);
    core.executeRun();

    for ( std::vector<std::string>::const_iterator msg = core.getMessages().cbegin();
          msg != core.getMessages().cend();
          msg++ )
    {
        std::cout << *msg << std::endl;
    }
    core.clearMessages();

    return 0;
}
