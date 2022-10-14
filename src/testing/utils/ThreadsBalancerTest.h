#ifndef THREADS_BALANCER_TEST_H_
#define THREADS_BALANCER_TEST_H_

#include "Includes.h"

class ThreadsBalancerTest : public TestSuite
{
public:
	typedef TestResult (ThreadsBalancerTest::*mptr)();

private:
	vector<mptr> testCases;

public:
	ThreadsBalancerTest(string proot);
	ThreadsBalancerTest(const ThreadsBalancerTest& otherTest) : TestSuite(otherTest.projectRoot)
	{
		this->testCases = otherTest.testCases;
	}
	virtual ~ThreadsBalancerTest(){}

	void beforeEach(){}
	void afterEach(){}

	virtual vector<TestResult> run();

	TestResult checkThreadsBalancer_EvenTid0();
	TestResult checkThreadsBalancer_EvenTid1();
	TestResult checkThreadsBalancer_EvenTid2();
	TestResult checkThreadsBalancer_EvenTid3();
	TestResult checkThreadsBalancer_OddTid0();
	TestResult checkThreadsBalancer_OddTid1();
	TestResult checkThreadsBalancer_OddTid2();

	void setTestCases(vector<mptr> _tc){ this->testCases = _tc; }
	void setTestCases(mptr _t, int i){ this->testCases[i] = _t; }
	void addTest(mptr _t){ this->testCases.push_back(_t); }
	void clearTestCases(){ this->testCases.clear(); }

	vector<mptr> getTestCases(){ return this->testCases; }
	mptr getTestCase(int i){ return this->testCases[i]; }
};

#endif