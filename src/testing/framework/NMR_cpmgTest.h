#ifndef NMR_CPMG_TEST_H_
#define NMR_CPMG_TEST_H_

#include "Includes.h"

class NMR_cpmgTest : public TestSuite
{
public:
	typedef TestResult (NMR_cpmgTest::*mptr)();

private:
	vector<mptr> testCases;

public:
	NMR_cpmgTest(string proot);
	NMR_cpmgTest(const NMR_cpmgTest& otherTest) : TestSuite(otherTest.projectRoot)
	{
		this->testCases = otherTest.testCases;
	}

	virtual ~NMR_cpmgTest(){}

	void beforeEach(){}

	void afterEach(){}

	virtual vector<TestResult> run();

	TestResult triLinInterpTest();
    TestResult triLinInterpTest_withSubDivision();	
	TestResult bcMapsTest_Periodic();
	TestResult bcMapsTest_Mirror();
	TestResult bcMapsTest_ExectimePeriodic();
	TestResult bcMapsTest_ExectimeMirror();

	void setTestCases(vector<mptr> _tc){ this->testCases = _tc; }
	void setTestCases(mptr _t, int i){ this->testCases[i] = _t; }
	void addTest(mptr _t){ this->testCases.push_back(_t); }
	void clearTestCases(){ this->testCases.clear(); }

	vector<mptr> getTestCases(){ return this->testCases; }
	mptr getTestCase(int i){ return this->testCases[i]; }
};

#endif