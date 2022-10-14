#ifndef BASEFUNCTIONS_TEST_H_
#define BASEFUNCTIONS_TEST_H_

#include "Includes.h"

class BaseFunctionsTest : public TestSuite
{
public:
	typedef TestResult (BaseFunctionsTest::*mptr)();

private:
	vector<mptr> testCases;

public:
	BaseFunctionsTest(string proot);
	BaseFunctionsTest(const BaseFunctionsTest& otherTest) : TestSuite(otherTest.projectRoot)
	{
		this->testCases = otherTest.testCases;
	}

	virtual ~BaseFunctionsTest(){}

	void beforeEach(){}

	void afterEach(){}

	virtual vector<TestResult> run();

	TestResult concatenateVectorsTest_Type();
	TestResult concatenateVectorsTest_Class();
	TestResult concatenateVectorsTest_OneIsEmpty();
	TestResult concatenateVectorsTest_BothAreEmpty();

	void setTestCases(vector<mptr> _tc){ this->testCases = _tc; }
	void setTestCases(mptr _t, int i){ this->testCases[i] = _t; }
	void addTest(mptr _t){ this->testCases.push_back(_t); }
	void clearTestCases(){ this->testCases.clear(); }

	vector<mptr> getTestCases(){ return this->testCases; }
	mptr getTestCase(int i){ return this->testCases[i]; }
};

#endif