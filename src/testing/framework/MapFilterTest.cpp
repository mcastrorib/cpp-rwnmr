#include "MapFilterTest.h"

MapFilterTest::MapFilterTest(string proot) : TestSuite(proot)
{
	// add test cases to suite vector
    (*this).addTest(&MapFilterTest::dumbTest);
}

vector<TestResult> MapFilterTest::run()
{
	vector<TestResult> results;
	
	for(int t = 0; t < this->testCases.size(); t++)
	{
		(*this).beforeEach();
		results.push_back((this->*testCases[t])());
		(*this).afterEach();
	}
	
	return results;
}

TestResult MapFilterTest::dumbTest()
{
	TestResult result("MapFilter::dumbTest()");
	result.setSuccess(Assert::assertTrue(false));  
	
    return result;
}

