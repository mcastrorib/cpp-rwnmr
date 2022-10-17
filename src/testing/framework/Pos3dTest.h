#ifndef POS3D_TEST_H_
#define POS3D_TEST_H_

#include "Includes.h"

class Pos3dTest : public TestSuite
{
public:
	typedef TestResult (Pos3dTest::*mptr)();

private:
	vector<mptr> testCases;

public:
	Pos3dTest(string proot);
	Pos3dTest(const Pos3dTest& otherTest) : TestSuite(otherTest.projectRoot)
	{
		this->testCases = otherTest.testCases;
	}

	virtual ~Pos3dTest(){}

	void beforeEach(){}

	void afterEach(){}

	virtual vector<TestResult> run();

	TestResult isPoreTest_ZerosMat2d();	
	TestResult isPoreTest_OnesMat2d();	
	TestResult isPoreTest_EyeMat2d();	
    TestResult isPoreTest_ZerosMat3d();	
	TestResult isPoreTest_OnesMat3d();	
	TestResult isPoreTest_EyeMat3d();	

	void setTestCases(vector<mptr> _tc){ this->testCases = _tc; }
	void setTestCases(mptr _t, int i){ this->testCases[i] = _t; }
	void addTest(mptr _t){ this->testCases.push_back(_t); }
	void clearTestCases(){ this->testCases.clear(); }

	vector<mptr> getTestCases(){ return this->testCases; }
	mptr getTestCase(int i){ return this->testCases[i]; }
};

#endif