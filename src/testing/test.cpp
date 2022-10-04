// include C++ standard libraries
#include <iostream>
#include <unistd.h>

// include CMake Configuration file
#include "config.h"

// include interface header files
#include "testing.h"

using namespace std;

// Main program
int main(int argc, char *argv[])
{   	
    
    UnitTest testApp(PROJECT_ROOT_DIR);
    
    if(argc == 1) testApp.runAll();
	else 
	{
		for(int t = 1; t < argc; t++)
			testApp.runTest(argv[t]);
	}

	return 0;
}
