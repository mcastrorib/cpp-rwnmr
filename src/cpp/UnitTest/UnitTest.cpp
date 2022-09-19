// include C++ standard libraries
#include <iostream>
#include <sstream>
#include <cstdint>
#include <vector>
#include <string>

// include project files
#include "../Utils/ArgsParser.h"
#include "../App/rwnmrApp.h"


#include "ConfigFilesTest.h"
#include "ArgsParserTest.h"
#include "UnitTest.h"

UnitTest::UnitTest(string proot) : project_root(proot), sucessResult(true)
{}

void UnitTest::runTest(string tag)
{
	if(tag == "all") (*this).runAll();
	else if(tag == "argsparser") ArgsParserTest((*this).getProjectRoot());
	else if(tag == "configfiles") ConfigFilesTest((*this).getProjectRoot());  
}

void UnitTest::runAll()
{
	cout << "running all the tests" << endl;
}