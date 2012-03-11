== Introduction ==
Every directory should contain a readme.txt file describing its content; and, if
possible, also a CMakeLists.txt file to compile whatever is in the directory.


== Instructions ==
 * cmake .	<-- generate Makefile(s); on Windows, it is 'cmake -G "MSYS Makefiles"'
 * make		<-- compile the source codes
 * make doc	<-- use doxygen to generate API documentation
 * make test	<-- run automated tests
 * make package	<-- generate installation packages


== Common conventions ==
(this should be discussed on some meeting...)


== Required software ==

=== On Windows ===
 - MinGW (+MSYS)
 - CMake
 - gitk (when installing, choose "Run Git and included Unix tools from the Windows command prompt")
 - Doxygen
