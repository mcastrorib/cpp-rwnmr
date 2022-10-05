#ifndef CPMG_CONFIG_TEST_H_
#define CPMG_CONFIG_TEST_H_

#include "Includes.h"

class cpmg_configTest
{
	typedef void (cpmg_configTest::*mptr)();
private:
	vector<mptr> testSuite;
	string projectRoot;
	string inputDir;
	bool sucessResult;
	cpmg_config *config;
public:
	cpmg_configTest(string proot) : projectRoot(proot), config(NULL)
	{
		// vector<mptr> testSuite();
		this->config = new cpmg_config();
		(*this).setInputDir(proot + "/input/testing/");

		// add test cases to suite vector
		this->testSuite.push_back(&cpmg_configTest::readConfigFileTest);
		this->testSuite.push_back(&cpmg_configTest::readD0Test);
		this->testSuite.push_back(&cpmg_configTest::readApplyBulkTest_True);
		this->testSuite.push_back(&cpmg_configTest::readApplyBulkTest_False);
		this->testSuite.push_back(&cpmg_configTest::readApplyBulkTest_Unknown);
		this->testSuite.push_back(&cpmg_configTest::readTimeVerboseTest);
		this->testSuite.push_back(&cpmg_configTest::readApplyBulkTest_Unknown);		
    	this->testSuite.push_back(&cpmg_configTest::readObservationTimeTest);
    	this->testSuite.push_back(&cpmg_configTest::readMethodTest_ImageBased);
		this->testSuite.push_back(&cpmg_configTest::readMethodTest_Histogram);
		this->testSuite.push_back(&cpmg_configTest::readMethodTest_Unknown);
    	this->testSuite.push_back(&cpmg_configTest::readResidualFieldTest_None);
    	this->testSuite.push_back(&cpmg_configTest::readResidualFieldTest_Uniform);
    	this->testSuite.push_back(&cpmg_configTest::readResidualFieldTest_Import);
    	this->testSuite.push_back(&cpmg_configTest::readResidualFieldTest_Unknown);
    	this->testSuite.push_back(&cpmg_configTest::readGradientValueTest);
    	this->testSuite.push_back(&cpmg_configTest::readGradientDirectionTest_0);
    	this->testSuite.push_back(&cpmg_configTest::readGradientDirectionTest_1);
    	this->testSuite.push_back(&cpmg_configTest::readGradientDirectionTest_2);
    	this->testSuite.push_back(&cpmg_configTest::readGradientDirectionTest_Unknown);
    	this->testSuite.push_back(&cpmg_configTest::readPathToFieldTest);
    	this->testSuite.push_back(&cpmg_configTest::readMinT2Test);
    	this->testSuite.push_back(&cpmg_configTest::readMaxT2Test);
    	this->testSuite.push_back(&cpmg_configTest::readUseT2LogspaceTest_True);
    	this->testSuite.push_back(&cpmg_configTest::readUseT2LogspaceTest_False);
    	this->testSuite.push_back(&cpmg_configTest::readUseT2LogspaceTest_Unknown);
    	this->testSuite.push_back(&cpmg_configTest::readNumT2BinsTest);
    	this->testSuite.push_back(&cpmg_configTest::readMinLambdaTest);
    	this->testSuite.push_back(&cpmg_configTest::readMaxLambdaTest);
    	this->testSuite.push_back(&cpmg_configTest::readNumLambdasTest);
    	this->testSuite.push_back(&cpmg_configTest::readPruneNumTest);
    	this->testSuite.push_back(&cpmg_configTest::readNoiseAmpTest); 
    	this->testSuite.push_back(&cpmg_configTest::readSaveModeTest_True);
    	this->testSuite.push_back(&cpmg_configTest::readSaveModeTest_False);
    	this->testSuite.push_back(&cpmg_configTest::readSaveModeTest_Unknown);
    	this->testSuite.push_back(&cpmg_configTest::readSaveWalkersTest_True);
    	this->testSuite.push_back(&cpmg_configTest::readSaveWalkersTest_False);
    	this->testSuite.push_back(&cpmg_configTest::readSaveWalkersTest_Unknown);
    	this->testSuite.push_back(&cpmg_configTest::readSaveDecayTest_True);
		this->testSuite.push_back(&cpmg_configTest::readSaveDecayTest_False);
		this->testSuite.push_back(&cpmg_configTest::readSaveHistogramTest_True);
		this->testSuite.push_back(&cpmg_configTest::readSaveHistogramTest_False);
		this->testSuite.push_back(&cpmg_configTest::readSaveHistogramListTest_True);
    	this->testSuite.push_back(&cpmg_configTest::readSaveT2Test_True);
		this->testSuite.push_back(&cpmg_configTest::readSaveT2Test_False);	

		// run tests 
		(*this).run();
	}

	cpmg_configTest(const cpmg_configTest& otherTest)
	{
		this->projectRoot = otherTest.projectRoot;
		this->inputDir = otherTest.inputDir;
		this->sucessResult = otherTest.sucessResult;
	}

	virtual ~cpmg_configTest(){
		if(this->config != NULL){
			delete this->config;
			this->config = NULL;
		}
	}

	void beforeEach()
	{
		if(this->config != NULL) delete this->config;
		this->config = new cpmg_config();		
	}

	void afterEach()
	{
		if(this->config != NULL) delete this->config;
		this->config = NULL;
	}

	void run();
	
	void readConfigFileTest();
	void readD0Test();
	void readApplyBulkTest_True();
	void readApplyBulkTest_False();
	void readApplyBulkTest_Unknown();
    void readTimeVerboseTest();
    void readObservationTimeTest();
    void readMethodTest_ImageBased();
	void readMethodTest_Histogram();
	void readMethodTest_Unknown();
    void readResidualFieldTest_None();
    void readResidualFieldTest_Uniform();
    void readResidualFieldTest_Import();
    void readResidualFieldTest_Unknown();
    void readGradientValueTest();
    void readGradientDirectionTest_0();
    void readGradientDirectionTest_1();
    void readGradientDirectionTest_2();
    void readGradientDirectionTest_Unknown();
    void readPathToFieldTest();
    void readMinT2Test();
    void readMaxT2Test();
    void readUseT2LogspaceTest_True();
    void readUseT2LogspaceTest_False();
    void readUseT2LogspaceTest_Unknown();
    void readNumT2BinsTest();
    void readMinLambdaTest();
    void readMaxLambdaTest();
    void readNumLambdasTest();
    void readPruneNumTest();
    void readNoiseAmpTest(); 
    void readSaveModeTest_True(); 
    void readSaveModeTest_False(); 
    void readSaveModeTest_Unknown();
    void readSaveWalkersTest_True();
    void readSaveWalkersTest_False();
    void readSaveWalkersTest_Unknown();
    void readSaveDecayTest_True();
	void readSaveDecayTest_False();
    void readSaveHistogramTest_True();
    void readSaveHistogramTest_False();
    void readSaveHistogramListTest_True();   
    void readSaveT2Test_True(); 
	void readSaveT2Test_False(); 


	void setProjectRoot(string proot){ this->projectRoot = proot; }
	string getProjectRoot(){ return this->projectRoot; }
	void setSuccessResult(bool result){ this->sucessResult = result; }
	bool getSuccessResult(){ return this->sucessResult; }
	void setInputDir(string _path){ this->inputDir = _path; }
	string getInputDir(){ return this->inputDir; }
};

#endif