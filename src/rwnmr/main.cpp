// include C++ standard libraries
#include <iostream>
#include <unistd.h>

// include CMake Configuration file
#include "config.h"

// include interface header files
#include "rwnmr.h"

using namespace std;

// Main program
int main(int argc, char **argv)
{    	
	rwnmrApp app(argc, argv, PROJECT_ROOT_DIR);
	app.exec();

	return 0;
}
