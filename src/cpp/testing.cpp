// include C++ standard libraries
#include <iostream>
#include <unistd.h>

// include CMake Configuration file
#include "RWNMR_Config.h"

// include interface header files
#include "UnitTest.h"

using namespace std;

// Main program
int main(int argc, char *argv[])
{    	
    UnitTest *testApp = new UnitTest(PROJECT_ROOT_DIR);
    
    if(argc == 1) testApp->runAll();
	else 
	{
		for(int t = 1; t < argc; t++)
			testApp->runTest(argv[t]);
	}

    delete testApp;
    testApp = NULL; 
	return 0;
}
