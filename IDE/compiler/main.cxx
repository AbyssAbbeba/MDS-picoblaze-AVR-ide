
#include "Compiler.h"

int main(int argc, char ** argv) {
	Compiler compiler;

	if ( true == compiler.compile("test_file_mcs51_0.asm") ) {
		std::cout << "SUCCESS\n";
	} else {
		std::cout << "FAILURE\n";
	}

	return 0;
}
