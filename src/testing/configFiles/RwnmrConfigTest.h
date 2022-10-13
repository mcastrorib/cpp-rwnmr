#ifndef RWNMR_CONFIG_TEST_H_
#define RWNMR_CONFIG_TEST_H_

#include "Includes.h"

class RwnmrConfigTest : public TestSuite
{
public:
	typedef TestResult (RwnmrConfigTest::*mptr)();

private:
	vector<mptr> testCases;
	RwnmrConfig *config;

public:
	RwnmrConfigTest(string proot);
	
	RwnmrConfigTest(const RwnmrConfigTest& otherTest) : TestSuite(otherTest.projectRoot)
	{
		this->testCases = otherTest.testCases; 
		this->config = otherTest.config;
	}

	virtual ~RwnmrConfigTest(){
		if(this->config != NULL){
			delete this->config;
			this->config = NULL;
		}
	}

	void beforeEach()
	{
		if(this->config != NULL) delete this->config;
		this->config = new RwnmrConfig();		
	}

	void afterEach()
	{
		if(this->config != NULL) delete this->config;
		this->config = NULL;
	}

	virtual vector<TestResult> run();
	
	TestResult readConfigFileTest();
	TestResult checkConfigTest_True();
	TestResult checkConfigTest_False();
	

	void setTestCases(vector<mptr> _tc){ this->testCases = _tc; }
	void setTestCases(mptr _t, int i){ this->testCases[i] = _t; }
	void addTest(mptr _t){ this->testCases.push_back(_t); }
	void clearTestCases(){ this->testCases.clear(); }
	void setConfig(RwnmrConfig *_conf){ this->config = _conf; }

	vector<mptr> getTestCases(){ return this->testCases; }
	mptr getTestCase(int i){ return this->testCases[i]; }
	RwnmrConfig *getConfig(){ return this->config; }
};

#endif