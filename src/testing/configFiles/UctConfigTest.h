#ifndef UCT_CONFIG_TEST_H_
#define UCT_CONFIG_TEST_H_

#include "Includes.h"

class UctConfigTest : public TestSuite
{
public:
	typedef TestResult (UctConfigTest::*mptr)();

private:
	vector<mptr> testCases;
	UctConfig *config;

public:
	UctConfigTest(string proot);
	
	UctConfigTest(const UctConfigTest& otherTest) : TestSuite(otherTest.projectRoot)
	{
		this->testCases = otherTest.testCases; 
		this->config = otherTest.config;
	}

	virtual ~UctConfigTest(){
		if(this->config != NULL){
			delete this->config;
			this->config = NULL;
		}
	}

	void beforeEach()
	{
		if(this->config != NULL) delete this->config;
		this->config = new UctConfig();		
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
	TestResult readDirPathTest_AbsolutePath();
	TestResult readDirPathTest_RelativePath();
	

	void setTestCases(vector<mptr> _tc){ this->testCases = _tc; }
	void setTestCases(mptr _t, int i){ this->testCases[i] = _t; }
	void addTest(mptr _t){ this->testCases.push_back(_t); }
	void clearTestCases(){ this->testCases.clear(); }
	void setConfig(UctConfig *_conf){ this->config = _conf; }

	vector<mptr> getTestCases(){ return this->testCases; }
	mptr getTestCase(int i){ return this->testCases[i]; }
	UctConfig *getConfig(){ return this->config; }
};

#endif