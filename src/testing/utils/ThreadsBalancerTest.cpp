#include "ThreadsBalancerTest.h"

ThreadsBalancerTest::ThreadsBalancerTest(string proot) : TestSuite(proot)
{
	// add test cases to suite vector
	(*this).addTest(&ThreadsBalancerTest::checkThreadsBalancer_EvenTid0);
	(*this).addTest(&ThreadsBalancerTest::checkThreadsBalancer_EvenTid1);
	(*this).addTest(&ThreadsBalancerTest::checkThreadsBalancer_OddTid0);
	(*this).addTest(&ThreadsBalancerTest::checkThreadsBalancer_OddTid1);
	(*this).addTest(&ThreadsBalancerTest::checkThreadsBalancer_OddTid2);
}

vector<TestResult> ThreadsBalancerTest::run()
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

TestResult ThreadsBalancerTest::checkThreadsBalancer_EvenTid0()
{
	TestResult result("ThreadsBalancer::init(s=even_tid=0)");
    const int threadId = 0;
    const int numThreads = 4;
    const int loopSize = 10;
    ThreadsBalancer b(threadId, numThreads, loopSize);
    vector<int> out = {b.getStart(), b.getFinish()};
	vector<int> exp = {0, 2};
	result.setSuccess(Assert::assertVectorEquals(out,exp));
	return result;
}

TestResult ThreadsBalancerTest::checkThreadsBalancer_EvenTid1()
{
	TestResult result("ThreadsBalancer::init(s=even_tid=1)");
    const int threadId = 1;
    const int numThreads = 4;
    const int loopSize = 10;
    ThreadsBalancer b(threadId, numThreads, loopSize);
    vector<int> out = {b.getStart(), b.getFinish()};
	vector<int> exp = {2, 4};
	result.setSuccess(Assert::assertVectorEquals(out,exp));
	return result;
}

TestResult ThreadsBalancerTest::checkThreadsBalancer_EvenTid2()
{
	TestResult result("ThreadsBalancer::init(s=even_tid=2)");
    const int threadId = 2;
    const int numThreads = 4;
    const int loopSize = 10;
    ThreadsBalancer b(threadId, numThreads, loopSize);
    vector<int> out = {b.getStart(), b.getFinish()};
	vector<int> exp = {4, 6};
	result.setSuccess(Assert::assertVectorEquals(out,exp));
	return result;
}

TestResult ThreadsBalancerTest::checkThreadsBalancer_EvenTid3()
{
	TestResult result("ThreadsBalancer::init(s=even_tid=3)");
    const int threadId = 3;
    const int numThreads = 4;
    const int loopSize = 10;
    ThreadsBalancer b(threadId, numThreads, loopSize);
    vector<int> out = {b.getStart(), b.getFinish()};
	vector<int> exp = {6, 10};
	result.setSuccess(Assert::assertVectorEquals(out,exp));
	return result;
}

TestResult ThreadsBalancerTest::checkThreadsBalancer_OddTid0()
{
	TestResult result("ThreadsBalancer::init(s=odd_tid=0)");
    const int threadId = 0;
    const int numThreads = 3;
    const int loopSize = 7;
    ThreadsBalancer b(threadId, numThreads, loopSize);
    vector<int> out = {b.getStart(), b.getFinish()};
	vector<int> exp = {0, 2};
	result.setSuccess(Assert::assertVectorEquals(out,exp));
	return result;
}

TestResult ThreadsBalancerTest::checkThreadsBalancer_OddTid1()
{
	TestResult result("ThreadsBalancer::init(s=odd_tid=1)");
    const int threadId = 1;
    const int numThreads = 3;
    const int loopSize = 7;
    ThreadsBalancer b(threadId, numThreads, loopSize);
    vector<int> out = {b.getStart(), b.getFinish()};
	vector<int> exp = {2, 4};
	result.setSuccess(Assert::assertVectorEquals(out,exp));
	return result;
}

TestResult ThreadsBalancerTest::checkThreadsBalancer_OddTid2()
{
	TestResult result("ThreadsBalancer::init(s=odd_tid=2)");
    const int threadId = 2;
    const int numThreads = 3;
    const int loopSize = 7;
    ThreadsBalancer b(threadId, numThreads, loopSize);
    vector<int> out = {b.getStart(), b.getFinish()};
	vector<int> exp = {4, 7};
	result.setSuccess(Assert::assertVectorEquals(out,exp));
	return result;
}