#ifndef IMAGEPATH_TEST_H_
#define IMAGEPATH_TEST_H_

#include "Includes.h"

class ImagePathTest : public TestSuite
{
public:
	typedef TestResult (ImagePathTest::*mptr)();

private:
	vector<mptr> testCases;
    ImagePath *ip;

public:
	ImagePathTest(string proot);
	ImagePathTest(const ImagePathTest& otherTest) : TestSuite(otherTest.projectRoot)
	{
		this->testCases = otherTest.testCases;
        this->ip = otherTest.ip;
	}

	virtual ~ImagePathTest()
    {
        if(this->ip != NULL) delete this->ip; 
        this->ip = NULL;
    }

	void beforeEach()
    {
        if(this->ip != NULL) delete this->ip;
        this->ip = new ImagePath();
    }

	void afterEach()    
    {
        if(this->ip != NULL) delete this->ip;
        this->ip = NULL;
    }

	virtual vector<TestResult> run();

	TestResult ImagePathConstructorTest();
    TestResult ImagePathConstructorTest_Overflow();
    TestResult convertFileIDToStringTest();
    TestResult updateLastIDTest();
    TestResult createImagesPathListTest();
    TestResult createImagesPathListTest_Overflow();

	void setTestCases(vector<mptr> _tc){ this->testCases = _tc; }
	void setTestCases(mptr _t, int i){ this->testCases[i] = _t; }
	void addTest(mptr _t){ this->testCases.push_back(_t); }
	void clearTestCases(){ this->testCases.clear(); }

	vector<mptr> getTestCases(){ return this->testCases; }
	mptr getTestCase(int i){ return this->testCases[i]; }
};

#endif