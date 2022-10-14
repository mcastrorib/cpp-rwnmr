#ifndef VECTOR3D_TEST_H_
#define VECTOR3D_TEST_H_

#include "Includes.h"

class Vector3dTest : public TestSuite
{
public:
	typedef TestResult (Vector3dTest::*mptr)();

private:
	vector<mptr> testCases;

public:
	Vector3dTest(string proot);
	Vector3dTest(const Vector3dTest& otherTest) : TestSuite(otherTest.projectRoot)
	{
		this->testCases = otherTest.testCases;
	}

	virtual ~Vector3dTest(){}

	void beforeEach(){}

	void afterEach(){}

	virtual vector<TestResult> run();

	TestResult initVecTest();
	TestResult operatorEqualsTest_True();
	TestResult operatorEqualsTest_False();
	TestResult operatorNotEqualsTest_True();
	TestResult operatorNotEqualsTest_False();
	TestResult getNormTest();
	TestResult addVectorTest();
	TestResult subtractVectorTest();
	TestResult multiplyVectorTest();
	TestResult dotProductTest();
	TestResult crossProductTest();	

	void setTestCases(vector<mptr> _tc){ this->testCases = _tc; }
	void setTestCases(mptr _t, int i){ this->testCases[i] = _t; }
	void addTest(mptr _t){ this->testCases.push_back(_t); }
	void clearTestCases(){ this->testCases.clear(); }

	vector<mptr> getTestCases(){ return this->testCases; }
	mptr getTestCase(int i){ return this->testCases[i]; }
};

#endif