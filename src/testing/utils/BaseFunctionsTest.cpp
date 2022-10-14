#include "BaseFunctionsTest.h"

BaseFunctionsTest::BaseFunctionsTest(string proot) : TestSuite(proot)
{
	// add test cases to suite vector
	(*this).addTest(&BaseFunctionsTest::concatenateVectorsTest_Type);
	(*this).addTest(&BaseFunctionsTest::concatenateVectorsTest_Class);
	(*this).addTest(&BaseFunctionsTest::concatenateVectorsTest_OneIsEmpty);
	(*this).addTest(&BaseFunctionsTest::concatenateVectorsTest_BothAreEmpty);
}

vector<TestResult> BaseFunctionsTest::run()
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

TestResult BaseFunctionsTest::concatenateVectorsTest_Type()
{
	TestResult result("BaseFunctions::concatenateVectors(type)");
    vector<int> a = {1,0,3};
    vector<int> b = {-3,0,-1};
    vector<int> out = BaseFunctions::concatenateVectors(a,b);
    vector<int> exp = {1,0,3,-3,0,-1};
	result.setSuccess(Assert::assertVectorEquals(out,exp));
	return result;
}

TestResult BaseFunctionsTest::concatenateVectorsTest_Class()
{
	TestResult result("BaseFunctions::concatenateVectors(class)");
    
    vector<Vector3d> a;
    a.push_back(Vector3d(0.0,0.0,0.0));
    a.push_back(Vector3d(1.0,1.0,1.0));

    vector<Vector3d> b;
    b.push_back(Vector3d(2.0,2.0,2.0));
    
    vector<Vector3d> out = BaseFunctions::concatenateVectors(a,b);

    vector<Vector3d> exp;
    exp.push_back(Vector3d(0.0,0.0,0.0));
    exp.push_back(Vector3d(1.0,1.0,1.0));
    exp.push_back(Vector3d(2.0,2.0,2.0));
    
    result.setSuccess(Assert::assertVectorEquals(out,exp));
	return result;
}
	
TestResult BaseFunctionsTest::concatenateVectorsTest_OneIsEmpty()
{
	TestResult result("BaseFunctions::concatenateVectors(one empty)");
	vector<int> a = {1,0,3};
    vector<int> b;
    vector<int> out = BaseFunctions::concatenateVectors(a,b);
    vector<int> exp = {1,0,3};
	result.setSuccess(Assert::assertVectorEquals(out,exp));
	return result;
}

TestResult BaseFunctionsTest::concatenateVectorsTest_BothAreEmpty()
{
	TestResult result("BaseFunctions::concatenateVectors(both empty)");
	vector<Vector3d> a;
    vector<Vector3d> b;
    vector<Vector3d> out = BaseFunctions::concatenateVectors(a,b);
    vector<Vector3d> exp = {};
	result.setSuccess(Assert::assertVectorEquals(out,exp));
	return result;
}