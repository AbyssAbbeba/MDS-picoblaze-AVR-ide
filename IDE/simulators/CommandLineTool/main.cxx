#include <iostream>

int main ( int argc, char ** argv )
{
    std::string input;
    while ( false == std::cin.eof() )
    {
        std::getline(std::cin, input);
        std::cout << "input='"<<input<<"'\n";
    }
    return 0;
}

/*
CLI options:
--device
--debug-file
--code-file
--code-file-type
--help
--version

Commands:
set
 - pc <address>
 - flag <flag> <value>
 - memory <space> <address> <value>
 - size <what> <value>
 - breakpoint <file:line> <value>
get
 - pc
 - flag <flag>
 - memory <space> <address> [ .. <end-address>]
 - size <what>
 - cycles // machine cycles
file
 - load <space> <type> <file>
 - save <space> <type> <file>

step
// animate
// run
// stop
reset

 */