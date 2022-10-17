#ifndef LSA_TEST_H_
#define LSA_TEST_H_

#include "Includes.h"

class LeastSquareAdjustTest : public TestSuite
{
public:
	typedef TestResult (LeastSquareAdjustTest::*mptr)();

private:
	vector<mptr> testCases;

public:
	LeastSquareAdjustTest(string proot);
	LeastSquareAdjustTest(const LeastSquareAdjustTest& otherTest) : TestSuite(otherTest.projectRoot)
	{
		this->testCases = otherTest.testCases;
	}

	virtual ~LeastSquareAdjustTest(){}

	void beforeEach(){}

	void afterEach(){}

	virtual vector<TestResult> run();

	TestResult lsaTest_interceptTrue();
	TestResult lsaTest_interceptFalse();

	void setTestCases(vector<mptr> _tc){ this->testCases = _tc; }
	void setTestCases(mptr _t, int i){ this->testCases[i] = _t; }
	void addTest(mptr _t){ this->testCases.push_back(_t); }
	void clearTestCases(){ this->testCases.clear(); }

	vector<mptr> getTestCases(){ return this->testCases; }
	mptr getTestCase(int i){ return this->testCases[i]; }
};

#endif