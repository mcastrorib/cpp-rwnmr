// include C++ standard libraries
#include <iostream>
#include <sstream>
#include <cstdint>
#include <vector>
#include <string>

// include project files
#include "UnitTest.h"

UnitTest::UnitTest(string proot) : projectRoot(proot), sucessResult(true)
{}

void UnitTest::runTest(string tag)
{
	if(tag == "all") (*this).runAll();
	else if(tag == "argsparser") ArgsParserTest((*this).getProjectRoot());
	else if(tag == "cpmg_config") cpmg_configTest((*this).getProjectRoot());
	// else if(tag == "configfiles") ConfigFilesTest((*this).getProjectRoot());  
}

void UnitTest::runAll()
{
	cout << "running all the tests" << endl;
}