// =============================================================================
/**
 * @brief
 * C++ Implementation: ...
 *
 * ...
 *
 * (C) copyright 2015 Moravia Microsystems, s.r.o.
 *
 * @author Martin Ošmera <martin.osmera@moravia-microsystems.com>
 * @ingroup SimCmdLine
 * @file main.cxx
 */
// =============================================================================

#include "SimCmdLine.h"

/**
 * @brief Program main function.
 * @param[in] argc Number of arguments.
 * @param[in] argv Array of arguments.
 * @return Program exit status, member of ExitCode enumeration.
 */
int main ( int argc, char ** argv )
{
    SimCmdLine cmdLine;
    return cmdLine.main(argc, argv);
}

/*

== CLI options ==
 -a, --architecture
 -d, --device
 -g, --debug-file
 -c, --code-file
 -t. --code-file-type
 -h, --help
 -V, --version
 -s, --silent

<exec> [OPTIONS]

== Commands ==
set
 - pc <address>
 - flag <flag> <value>
 - memory <space> <address> <value>
 - size <what> <value>
 - breakpoint <file:line> <value>
get
 - pc
 - flag <flag>
 - memory <space> <address> [ <end-address>]
 - size <what>
 - cycles // machine cycles
 - breakpoints
 - locations
file
 - load <space> <type> <file>
 - save <space> <type> <file>

sim
 - step [<steps>]
 - animate
 - run
 - halt
 - reset

exit [<code>]
help [<command>]

 */
