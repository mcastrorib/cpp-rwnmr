#include "MemAllocatorTest.h"

MemAllocatorTest::MemAllocatorTest(string proot) : TestSuite(proot)
{
	// add test cases to suite vector
	(*this).addTest(&MemAllocatorTest::checkAllocationTest_True);
	(*this).addTest(&MemAllocatorTest::checkAllocationTest_False);
}

vector<TestResult> MemAllocatorTest::run()
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

TestResult MemAllocatorTest::checkAllocationTest_True()
{
	TestResult result("MemAllocator::checkAllocation(true)");
    int *a = new int[2];
	result.setSuccess(Assert::assertTrue(MemAllocator::checkAllocation(a,false)));
	return result;
}

TestResult MemAllocatorTest::checkAllocationTest_False()
{
	TestResult result("MemAllocator::checkAllocation(false)");
    int *a = NULL;
	result.setSuccess(Assert::assertFalse(MemAllocator::checkAllocation(a,false)));
	return result;
}