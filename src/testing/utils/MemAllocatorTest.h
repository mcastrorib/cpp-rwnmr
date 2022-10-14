#ifndef MEMALLOCATOR_TEST_H_
#define MEMALLOCATOR_TEST_H_

#include "Includes.h"

class MemAllocatorTest : public TestSuite
{
public:
	typedef TestResult (MemAllocatorTest::*mptr)();

private:
	vector<mptr> testCases;

public:
	MemAllocatorTest(string proot);
	MemAllocatorTest(const MemAllocatorTest& otherTest) : TestSuite(otherTest.projectRoot)
	{
		this->testCases = otherTest.testCases;
	}

	virtual ~MemAllocatorTest(){}

	void beforeEach(){}

	void afterEach(){}

	virtual vector<TestResult> run();

	TestResult checkAllocationTest_True();
	TestResult checkAllocationTest_False();

	void setTestCases(vector<mptr> _tc){ this->testCases = _tc; }
	void setTestCases(mptr _t, int i){ this->testCases[i] = _t; }
	void addTest(mptr _t){ this->testCases.push_back(_t); }
	void clearTestCases(){ this->testCases.clear(); }

	vector<mptr> getTestCases(){ return this->testCases; }
	mptr getTestCase(int i){ return this->testCases[i]; }
};

#endif