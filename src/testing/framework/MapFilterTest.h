#ifndef MAP_FILTER_TEST_H_
#define MAP_FILTER_TEST_H_

#include "Includes.h"

class MapFilterTest : public TestSuite
{
public:
	typedef TestResult (MapFilterTest::*mptr)();

private:
	vector<mptr> testCases;

public:
	MapFilterTest(string proot);
	MapFilterTest(const MapFilterTest& otherTest) : TestSuite(otherTest.projectRoot)
	{
		this->testCases = otherTest.testCases;
	}

	virtual ~MapFilterTest(){}
	void beforeEach(){}
	void afterEach(){}

	virtual vector<TestResult> run();

	TestResult dumbTest();

	void setTestCases(vector<mptr> _tc){ this->testCases = _tc; }
	void setTestCases(mptr _t, int i){ this->testCases[i] = _t; }
	void addTest(mptr _t){ this->testCases.push_back(_t); }
	void clearTestCases(){ this->testCases.clear(); }

	vector<mptr> getTestCases(){ return this->testCases; }
	mptr getTestCase(int i){ return this->testCases[i]; }

    void printWalkerInfo(Walker w)
    {
        cout << w.getInitialPositionX() << ", " << w.getInitialPositionY() << ", " << w.getInitialPositionZ();
        cout << ", " << w.getCurrentPositionX() << ", " << w.getCurrentPositionY() << ", " << w.getCurrentPositionZ();
        cout << ", " << w.getXiRate() << ", " << w.getCollisions() << ", " << w.getInitialSeed() << endl; 
    }
};

#endif