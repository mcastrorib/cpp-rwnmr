#ifndef MULTITAU_CONFIG_TEST_H_
#define MULTITAU_CONFIG_TEST_H_

#include "Includes.h"

class MultitauConfigTest : public TestSuite
{
public:
	typedef TestResult (MultitauConfigTest::*mptr)();

private:
	vector<mptr> testCases;
	MultitauConfig *config;

public:
	MultitauConfigTest(string proot);
	
	MultitauConfigTest(const MultitauConfigTest& otherTest) : TestSuite(otherTest.projectRoot)
	{
		this->testCases = otherTest.testCases; 
		this->config = otherTest.config;
	}

	virtual ~MultitauConfigTest(){
		if(this->config != NULL){
			delete this->config;
			this->config = NULL;
		}
	}

	void beforeEach()
	{
		if(this->config != NULL) delete this->config;
		this->config = new MultitauConfig();		
	}

	void afterEach()
	{
		if(this->config != NULL) delete this->config;
		this->config = NULL;
	}

	virtual vector<TestResult> run();
	
	TestResult readConfigFileTest();
	TestResult checkConfigTest_True();
	TestResult checkConfigTest_False1();
	TestResult checkConfigTest_False2();
	TestResult checkConfigTest_False3();
	

	void setTestCases(vector<mptr> _tc){ this->testCases = _tc; }
	void setTestCases(mptr _t, int i){ this->testCases[i] = _t; }
	void addTest(mptr _t){ this->testCases.push_back(_t); }
	void clearTestCases(){ this->testCases.clear(); }
	void setConfig(MultitauConfig *_conf){ this->config = _conf; }

	vector<mptr> getTestCases(){ return this->testCases; }
	mptr getTestCase(int i){ return this->testCases[i]; }
	MultitauConfig *getConfig(){ return this->config; }
};

#endif