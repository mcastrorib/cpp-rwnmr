#ifndef MATHFUNCTIONS_TEST_H_
#define MATHFUNCTIONS_TEST_H_

#include "Includes.h"

class MathFunctionsTest : public TestSuite
{
public:
	typedef TestResult (MathFunctionsTest::*mptr)();

private:
	vector<mptr> testCases;

public:
	MathFunctionsTest(string proot);
	MathFunctionsTest(const MathFunctionsTest& otherTest) : TestSuite(otherTest.projectRoot)
	{
		this->testCases = otherTest.testCases;
	}

	virtual ~MathFunctionsTest(){}

	void beforeEach(){}

	void afterEach(){}

	virtual vector<TestResult> run();

	TestResult zerosVectorTest();
	TestResult onesVectorTest();
	TestResult addVectorsTest();
	TestResult multiplyVectorsTest_1();
	TestResult multiplyVectorsTest_2();
	TestResult dotProductTest();
	TestResult normTest();
	TestResult euclideanDistanceTest();
	TestResult sumTest_Vec();
	TestResult sumTest_Ptr();
	TestResult meanTest_Vec();
	TestResult meanTest_Ptr();
	TestResult stdDevTest_Vec();
	TestResult stdDevTest_Ptr();
	TestResult linspaceTest();
	TestResult logspaceTest();	

	void setTestCases(vector<mptr> _tc){ this->testCases = _tc; }
	void setTestCases(mptr _t, int i){ this->testCases[i] = _t; }
	void addTest(mptr _t){ this->testCases.push_back(_t); }
	void clearTestCases(){ this->testCases.clear(); }

	vector<mptr> getTestCases(){ return this->testCases; }
	mptr getTestCase(int i){ return this->testCases[i]; }
};

#endif