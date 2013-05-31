#include "MScriptCore.h"
#include "MScriptStrategy.h"
#include <iostream>

int main ( int argc, char ** argv )
{
    std::string script =
        "if ( 2 == a )"                 // 1
        "{"                             // 2
        "    a = ( b * c + d );"        // 3
        "    b = 4 - d;"                // 4
        "}"                             // 5
        "else"                          // 6
        "{"                             // 7
        "    d -= c % 9;"               // 8
        "}";                            // 9

    MScriptStrategy strategy;
    MScriptCore core(&strategy);

    core.loadScript(script);
    core.executeRun();

    for ( std::vector<std::string>::const_iterator msg = core.getMessages().cbegin();
          msg != core.getMessages().cend();
          msg++ )
    {
        std::cout << *msg << std::endl;
    }

    return 0;
}
