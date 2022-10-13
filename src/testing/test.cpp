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
    
    TestApp app(PROJECT_ROOT_DIR);
    
    if(argc == 1) app.noArgsMessage();
	else if((string) argv[1] == "-h") app.help();
	else 
	{
		for(int t = 1; t < argc; t++)
			app.exec(argv[t]);
	}

	return 0;
}
