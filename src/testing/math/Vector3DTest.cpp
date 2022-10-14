#include "Vector3DTest.h"

Vector3DTest::Vector3DTest(string proot) : TestSuite(proot)
{
	// add test cases to suite vector
	(*this).addTest(&Vector3DTest::initVecTest);
}

vector<TestResult> Vector3DTest::run()
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

TestResult Vector3DTest::initVecTest()
{
	TestResult result("Vector3D::init()");
	return result;
}

	