#ifndef VECTOR3D_TEST_H_
#define VECTOR3D_TEST_H_

#include "Includes.h"

class Vector3DTest : public TestSuite
{
public:
	typedef TestResult (Vector3DTest::*mptr)();

private:
	vector<mptr> testCases;

public:
	Vector3DTest(string proot);
	Vector3DTest(const Vector3DTest& otherTest) : TestSuite(otherTest.projectRoot)
	{
		this->testCases = otherTest.testCases;
	}

	virtual ~Vector3DTest(){}

	void beforeEach(){}

	void afterEach(){}

	virtual vector<TestResult> run();

	TestResult initVecTest();

	void setTestCases(vector<mptr> _tc){ this->testCases = _tc; }
	void setTestCases(mptr _t, int i){ this->testCases[i] = _t; }
	void addTest(mptr _t){ this->testCases.push_back(_t); }
	void clearTestCases(){ this->testCases.clear(); }

	vector<mptr> getTestCases(){ return this->testCases; }
	mptr getTestCase(int i){ return this->testCases[i]; }
};

#endif