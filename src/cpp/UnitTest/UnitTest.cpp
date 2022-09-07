// include C++ standard libraries
#include <iostream>
#include <vector>
#include <string>

#include "UnitTest.h"

using namespace std;

void UnitTest::runTest(string tag)
{
	if(tag == "all") (*this).runAll();
	else cout << "running test " << tag << endl; 
}

void UnitTest::runAll()
{
	cout << "running all the tests" << endl;
}