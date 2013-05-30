#include "MScriptCore.h"
#include "MScriptStrategy.h"

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
//*/
    MScriptStrategy strategy;
    MScriptCore core(&strategy);

    core.loadScript(script);
    core.executeRun();
}
