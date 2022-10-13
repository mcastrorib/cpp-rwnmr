#ifndef CPMG_CONFIG_TEST_H_
#define CPMG_CONFIG_TEST_H_

#include "Includes.h"

class CpmgConfigTest : public TestSuite
{
public:
	typedef TestResult (CpmgConfigTest::*mptr)();

private:
	vector<mptr> testCases;
	CpmgConfig *config;

public:
	CpmgConfigTest(string proot);
	
	CpmgConfigTest(const CpmgConfigTest& otherTest) : TestSuite(otherTest.projectRoot)
	{
		this->testCases = otherTest.testCases; 
		this->config = otherTest.config;
	}

	virtual ~CpmgConfigTest(){
		if(this->config != NULL){
			delete this->config;
			this->config = NULL;
		}
	}

	void beforeEach()
	{
		if(this->config != NULL) delete this->config;
		this->config = new CpmgConfig();		
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
	TestResult readApplyBulkTest_True();
	TestResult readApplyBulkTest_False();
	TestResult readApplyBulkTest_Unknown();
    TestResult readTimeVerboseTest();
    TestResult readObservationTimeTest();
    TestResult readMethodTest_ImageBased();
	TestResult readMethodTest_Histogram();
	TestResult readMethodTest_Unknown();
    TestResult readResidualFieldTest_None();
    TestResult readResidualFieldTest_Uniform();
    TestResult readResidualFieldTest_Import();
    TestResult readResidualFieldTest_Unknown();
    TestResult readGradientValueTest();
    TestResult readGradientDirectionTest_0();
    TestResult readGradientDirectionTest_1();
    TestResult readGradientDirectionTest_2();
    TestResult readGradientDirectionTest_Unknown();
    TestResult readPathToFieldTest_AbsolutePath();
	TestResult readPathToFieldTest_RelativePath();
    TestResult readMinT2Test();
    TestResult readMaxT2Test();
    TestResult readUseT2LogspaceTest_True();
    TestResult readUseT2LogspaceTest_False();
    TestResult readUseT2LogspaceTest_Unknown();
    TestResult readNumT2BinsTest();
    TestResult readMinLambdaTest();
    TestResult readMaxLambdaTest();
    TestResult readNumLambdasTest();
    TestResult readPruneNumTest();
    TestResult readNoiseAmpTest(); 
    TestResult readSaveModeTest_True(); 
    TestResult readSaveModeTest_False(); 
    TestResult readSaveModeTest_Unknown();
    TestResult readSaveWalkersTest_True();
    TestResult readSaveWalkersTest_False();
    TestResult readSaveWalkersTest_Unknown();
    TestResult readSaveDecayTest_True();
	TestResult readSaveDecayTest_False();
    TestResult readSaveHistogramTest_True();
    TestResult readSaveHistogramTest_False();
    TestResult readSaveHistogramListTest_True();   
    TestResult readSaveT2Test_True(); 
	TestResult readSaveT2Test_False(); 

	void setTestCases(vector<mptr> _tc){ this->testCases = _tc; }
	void setTestCases(mptr _t, int i){ this->testCases[i] = _t; }
	void addTest(mptr _t){ this->testCases.push_back(_t); }
	void clearTestCases(){ this->testCases.clear(); }
	void setConfig(CpmgConfig *_conf){ this->config = _conf; }

	vector<mptr> getTestCases(){ return this->testCases; }
	mptr getTestCase(int i){ return this->testCases[i]; }
	CpmgConfig *getConfig(){ return this->config; }
};

#endif