#include "MScriptCore.h"
#include "MScriptStrategy.h"
#include <iostream>

int main ( int argc, char ** argv )
{
    std::string script =
        "if ( 2 == a )\n"                 // 1
        "{\n"                             // 2
        "    a = ( b * c + d );\n"        // 3
        "    b = 4 - d;\n"                // 4
        "}\n"                             // 5
        "else\n"                          // 6
        "{\n"                             // 7
        "    d -= c % 9;\n"               // 8
        "}\n";                            // 9

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
