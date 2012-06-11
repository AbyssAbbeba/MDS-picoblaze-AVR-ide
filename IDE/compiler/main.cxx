
#include "Compiler.h"

int main(int argc, char ** argv) {
	Compiler * compiler = new Compiler();

	if ( true == compiler->compile("test_file_mcs51_0.asm") ) {
		std::cout << "SUCCESS\n";
	} else {
		std::cout << "FAILURE\n";
	}

	delete compiler;
	return 0;
}
