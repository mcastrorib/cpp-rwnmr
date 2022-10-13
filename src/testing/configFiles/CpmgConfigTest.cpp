#include "CpmgConfigTest.h"

CpmgConfigTest::CpmgConfigTest(string proot) : TestSuite(proot), config(NULL)
{
	// add test cases to suite vector
	(*this).addTest(&CpmgConfigTest::readConfigFileTest);
	(*this).addTest(&CpmgConfigTest::checkConfigTest_True);
	(*this).addTest(&CpmgConfigTest::checkConfigTest_False);
	(*this).addTest(&CpmgConfigTest::readApplyBulkTest_True);
	(*this).addTest(&CpmgConfigTest::readApplyBulkTest_False);
	(*this).addTest(&CpmgConfigTest::readApplyBulkTest_Unknown);
	(*this).addTest(&CpmgConfigTest::readTimeVerboseTest);
	(*this).addTest(&CpmgConfigTest::readApplyBulkTest_Unknown);		
	(*this).addTest(&CpmgConfigTest::readObservationTimeTest);
	(*this).addTest(&CpmgConfigTest::readMethodTest_ImageBased);
	(*this).addTest(&CpmgConfigTest::readMethodTest_Histogram);
	(*this).addTest(&CpmgConfigTest::readMethodTest_Unknown);
	(*this).addTest(&CpmgConfigTest::readResidualFieldTest_None);
	(*this).addTest(&CpmgConfigTest::readResidualFieldTest_Uniform);
	(*this).addTest(&CpmgConfigTest::readResidualFieldTest_Import);
	(*this).addTest(&CpmgConfigTest::readResidualFieldTest_Unknown);
	(*this).addTest(&CpmgConfigTest::readGradientValueTest);
	(*this).addTest(&CpmgConfigTest::readGradientDirectionTest_0);
	(*this).addTest(&CpmgConfigTest::readGradientDirectionTest_1);
	(*this).addTest(&CpmgConfigTest::readGradientDirectionTest_2);
	(*this).addTest(&CpmgConfigTest::readGradientDirectionTest_Unknown);
	(*this).addTest(&CpmgConfigTest::readPathToFieldTest_AbsolutePath);
	(*this).addTest(&CpmgConfigTest::readPathToFieldTest_RelativePath);
	(*this).addTest(&CpmgConfigTest::readMinT2Test);
	(*this).addTest(&CpmgConfigTest::readMaxT2Test);
	(*this).addTest(&CpmgConfigTest::readUseT2LogspaceTest_True);
	(*this).addTest(&CpmgConfigTest::readUseT2LogspaceTest_False);
	(*this).addTest(&CpmgConfigTest::readUseT2LogspaceTest_Unknown);
	(*this).addTest(&CpmgConfigTest::readNumT2BinsTest);
	(*this).addTest(&CpmgConfigTest::readMinLambdaTest);
	(*this).addTest(&CpmgConfigTest::readMaxLambdaTest);
	(*this).addTest(&CpmgConfigTest::readNumLambdasTest);
	(*this).addTest(&CpmgConfigTest::readPruneNumTest);
	(*this).addTest(&CpmgConfigTest::readNoiseAmpTest); 
	(*this).addTest(&CpmgConfigTest::readSaveModeTest_True);
	(*this).addTest(&CpmgConfigTest::readSaveModeTest_False);
	(*this).addTest(&CpmgConfigTest::readSaveModeTest_Unknown);
	(*this).addTest(&CpmgConfigTest::readSaveWalkersTest_True);
	(*this).addTest(&CpmgConfigTest::readSaveWalkersTest_False);
	(*this).addTest(&CpmgConfigTest::readSaveWalkersTest_Unknown);
	(*this).addTest(&CpmgConfigTest::readSaveDecayTest_True);
	(*this).addTest(&CpmgConfigTest::readSaveDecayTest_False);
	(*this).addTest(&CpmgConfigTest::readSaveHistogramTest_True);
	(*this).addTest(&CpmgConfigTest::readSaveHistogramTest_False);
	(*this).addTest(&CpmgConfigTest::readSaveHistogramListTest_True);
	(*this).addTest(&CpmgConfigTest::readSaveT2Test_True);
	(*this).addTest(&CpmgConfigTest::readSaveT2Test_False);	
}

vector<TestResult> CpmgConfigTest::run()
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

TestResult CpmgConfigTest::readConfigFileTest()
{	
	TestResult result;
	result.setMessage("CpmgConfig::readConfigFile()");

	string config_path = "tcpmg.config";	
	this->config->readConfigFile((*this).getInputDir() + config_path);	
	if(Assert::assertEquals(this->config->getMethod(), (string)"image-based") and
	   Assert::assertEquals(this->config->getObservationTime(), (double) 500.0) and
	   Assert::assertEquals(this->config->getApplyBulk(), (bool) false) and 
	   Assert::assertEquals(this->config->getTimeVerbose(), (bool) false) and
	   Assert::assertEquals(this->config->getResidualField(), (string)"uniform") and 
	   Assert::assertEquals(this->config->getGradientValue(), (double) 1.0) and
	   Assert::assertEquals(this->config->getGradientDirection(), (int) 2) and 
	   Assert::assertEquals(this->config->getMinT2(), (double) 0.1) and
	   Assert::assertEquals(this->config->getMaxT2(), (double) 10000.0) and
	   Assert::assertEquals(this->config->getUseT2Logspace(), (bool) true) and 
	   Assert::assertEquals(this->config->getNumT2Bins(), (int) 256) and
	   Assert::assertEquals(this->config->getMinLambda(), (double) -4.0) and 
	   Assert::assertEquals(this->config->getMaxLambda(), (double) 2.0) and
	   Assert::assertEquals(this->config->getNumLambdas(), (int) 512) and
	   Assert::assertEquals(this->config->getPruneNum(), (int) 512) and
	   Assert::assertEquals(this->config->getNoiseAmp(), (double) 0.0) and 
	   Assert::assertEquals(this->config->getSaveMode(), (bool) true) and
	   Assert::assertEquals(this->config->getSaveDecay(), (bool) false) and 
	   Assert::assertEquals(this->config->getSaveT2(), (bool) false) and
	   Assert::assertEquals(this->config->getSaveWalkers(), (bool) false) and 
	   Assert::assertEquals(this->config->getSaveHistogram(), (bool) false) and 
	   Assert::assertEquals(this->config->getSaveHistogramList(), (bool) false)
	) result.setSuccess(true);
	else result.setSuccess(false); 

	return result;
}

TestResult CpmgConfigTest::checkConfigTest_True()
{	
	TestResult result;
	result.setMessage("CpmgConfig::checkConfig(true)");

	string config_path = "tcpmg.config";	
	this->config->readConfigFile((*this).getInputDir() + config_path);
	vector<string> output = this->config->checkConfig();
	vector<string> expected = {};
	if(Assert::assertVectorEquals(output, expected) and 
	   Assert::assertTrue(this->config->getReady())
	) result.setSuccess(true);
	else {
		result.setSuccess(false);
		for(int i = 0; i < output.size(); i++) cout << output[i] << endl;
	}
	return result;
}

TestResult CpmgConfigTest::checkConfigTest_False()
{	
	TestResult result;
	result.setMessage("CpmgConfig::checkConfig(false)");

	string config_path = "tcpmg.config";	
	this->config->readConfigFile((*this).getInputDir() + config_path);
	this->config->setObservationTime(-10.0);
	this->config->setMethod("other");
	this->config->setResidualField("other");
	this->config->setGradientDirection(4);
	this->config->setMinT2(-10.0);
	this->config->setMaxT2(-1.0);
	this->config->setNumT2Bins(-5);
	this->config->setNumLambdas(0);
	this->config->setPruneNum(-1);
	
	vector<string> output = this->config->checkConfig();
	vector<string> expected = {"OBS_TIME",
							   "METHOD", 
							   "RESIDUAL_FIELD", 
							   "GRADIENT_DIRECTION", 
							   "MIN_T2", 
							   "MAX_T2", 
							   "NUM_T2_BINS",
							   "NUM_LAMBDAS",
							   "PRUNE_NUM"};
	if(Assert::assertVectorEquals(output, expected) and 
	   Assert::assertFalse(this->config->getReady())
	) result.setSuccess(true);
	else result.setSuccess(false);
	return result;
}

TestResult CpmgConfigTest::readApplyBulkTest_True()
{
	TestResult result("CpmgConfig::readApplyBulk(true)");
	string token = "true";
	this->config->readApplyBulk(token);
	result.setSuccess(Assert::assertTrue(this->config->getApplyBulk()));
	return result;
}

TestResult CpmgConfigTest::readApplyBulkTest_False()
{
	TestResult result("CpmgConfig::readApplyBulk(false)");
	string token = "false";
	this->config->readApplyBulk(token);
	result.setSuccess(Assert::assertFalse(this->config->getApplyBulk()));
	return result;
}

TestResult CpmgConfigTest::readApplyBulkTest_Unknown()
{
	TestResult result("CpmgConfig::Testing readApplyBulk(unknown)");
	string token = "da5s1d6";
	this->config->readApplyBulk(token);
	result.setSuccess(Assert::assertFalse(this->config->getApplyBulk()));
	return result;
}

TestResult CpmgConfigTest::readTimeVerboseTest()
{
	TestResult result("CpmgConfig::readApplyBulk()");
	string token = "true";
	this->config->readTimeVerbose(token);
	result.setSuccess(Assert::assertEquals(this->config->getTimeVerbose(), (bool) true));
	return result;
}

TestResult CpmgConfigTest::CpmgConfigTest::readObservationTimeTest()
{
	TestResult result("CpmgConfig::readObservationTime()");
	string token = "1000.0";
	this->config->readObservationTime(token);
	result.setSuccess(Assert::assertEquals(this->config->getObservationTime(), (double) 1000.0));
	return result;
}

TestResult CpmgConfigTest::readMethodTest_ImageBased()
{
	TestResult result("CpmgConfig::readMethod(image-based)");
	string token = "image-based";
	this->config->readMethod(token);
	result.setSuccess(Assert::assertEquals(this->config->getMethod(), (string) "image-based"));
	return result;
}

TestResult CpmgConfigTest::readMethodTest_Histogram()
{
	TestResult result("CpmgConfig::readMethod(histogram)");
	string token = "histogram";
	this->config->readMethod(token);
	result.setSuccess(Assert::assertEquals(this->config->getMethod(), (string) "histogram"));
	return result;
}

TestResult CpmgConfigTest::readMethodTest_Unknown()
{
	TestResult result("CpmgConfig::readMethod(unknown)");
	string token = "xxx";
	this->config->readMethod(token);
	result.setSuccess(Assert::assertEquals(this->config->getMethod(), (string) "image-based"));
	return result;
}

TestResult CpmgConfigTest::readResidualFieldTest_None()
{
	TestResult result("CpmgConfig::readResidualField(none)");
	string token = "none";
	this->config->readResidualField(token);
	result.setSuccess(Assert::assertEquals(this->config->getResidualField(), (string) "none"));
	return result;
}

TestResult CpmgConfigTest::readResidualFieldTest_Uniform()
{
	TestResult result("CpmgConfig::readResidualField(uniform)");
	string token = "uniform";
	this->config->readResidualField(token);
	result.setSuccess(Assert::assertEquals(this->config->getResidualField(), (string) "uniform"));
	return result;
}

TestResult CpmgConfigTest::readResidualFieldTest_Import()
{
	TestResult result("CpmgConfig::readResidualField(import)");
	string token = "import";
	this->config->readResidualField(token);
	result.setSuccess(Assert::assertEquals(this->config->getResidualField(), (string) "import"));
	return result;
}

TestResult CpmgConfigTest::readResidualFieldTest_Unknown()
{
	TestResult result("CpmgConfig::readResidualField(unknown)");
	string token = "dsasd";
	this->config->readResidualField(token);
	result.setSuccess(Assert::assertEquals(this->config->getResidualField(), (string) "none"));
	return result;
}

TestResult CpmgConfigTest::readGradientValueTest()
{
	TestResult result("CpmgConfig::readGradientValue()");
	string token = "1.0";
	this->config->readGradientValue(token);
	result.setSuccess(Assert::assertEquals(this->config->getGradientValue(), (double) 1.0));
	return result;
}

TestResult CpmgConfigTest::readGradientDirectionTest_0()
{
	TestResult result("CpmgConfig::readGradientDirection(0)");
	string token = "0";
	this->config->readGradientDirection(token);
	result.setSuccess(Assert::assertEquals(this->config->getGradientDirection(), (int) 0));
	return result;	
}

TestResult CpmgConfigTest::readGradientDirectionTest_1()
{
	TestResult result("CpmgConfig::readGradientDirection(1)");
	string token = "1";
	this->config->readGradientDirection(token);
	result.setSuccess(Assert::assertEquals(this->config->getGradientDirection(), (int) 1));
	return result;
}

TestResult CpmgConfigTest::readGradientDirectionTest_2()
{
	TestResult result("CpmgConfig::readGradientDirection(2)");
	string token = "2";
	this->config->readGradientDirection(token);
	result.setSuccess(Assert::assertEquals(this->config->getGradientDirection(), (int) 2));
	return result;	
}

TestResult CpmgConfigTest::readGradientDirectionTest_Unknown()
{
	TestResult result("CpmgConfig::readGradientDirection(Unknown)");
	string token = "xx x";
	this->config->readGradientDirection(token);
	result.setSuccess(Assert::assertEquals(this->config->getGradientDirection(), (int) 2));
	return result;	
}

TestResult CpmgConfigTest::readPathToFieldTest_AbsolutePath()
{
	TestResult result("uct_config::readPathToField(absolute)");
	string token = "/this/is/my/test/absolute-path";
	this->config->readPathToField(token);
	result.setSuccess(Assert::assertEquals(this->config->getPathToField(), token));
	return result;
}

TestResult CpmgConfigTest::readPathToFieldTest_RelativePath()
{
	TestResult result("uct_config::readPathToField(relative)");
	string projRoot = "/this/is/";
	this->config->setProjectRoot(projRoot);
	string token = "./my/test/relative-path";
	this->config->readPathToField(token);
	string expected = projRoot + token;
	result.setSuccess(Assert::assertEquals(this->config->getPathToField(), expected));
	return result;
}

TestResult CpmgConfigTest::readMinT2Test()
{
	TestResult result("CpmgConfig::readMinT2()");
	string token = "0.1";
	this->config->readMinT2(token);
	result.setSuccess(Assert::assertEquals(this->config->getMinT2(), (double) 0.1));
	return result;		
}

TestResult CpmgConfigTest::readMaxT2Test()
{
	TestResult result("CpmgConfig::readMaxT2()");
	string token = "10";
	this->config->readMaxT2(token);
	result.setSuccess(Assert::assertEquals(this->config->getMaxT2(), (double) 10.0));
	return result;			
}

TestResult CpmgConfigTest::readUseT2LogspaceTest_True()
{
	TestResult result("CpmgConfig::readUseT2Logspace(true)");
	string token = "true";
	this->config->readUseT2Logspace(token);
	result.setSuccess(Assert::assertTrue(this->config->getUseT2Logspace()));
	return result;		
}

TestResult CpmgConfigTest::readUseT2LogspaceTest_False()
{
	TestResult result("CpmgConfig::readUseT2Logspace(false)");
	string token = "false";
	this->config->readUseT2Logspace(token);
	result.setSuccess(Assert::assertFalse(this->config->getUseT2Logspace()));
	return result;		
}

TestResult CpmgConfigTest::readUseT2LogspaceTest_Unknown()
{
	TestResult result("CpmgConfig::readUseT2Logspace(true)");
	string token = "nnn";
	this->config->readUseT2Logspace(token);
	result.setSuccess(Assert::assertFalse(this->config->getUseT2Logspace()));
	return result;		
}

TestResult CpmgConfigTest::readNumT2BinsTest()
{
	TestResult result("CpmgConfig::readNumT2Bins()");
	string token = "10";
	this->config->readNumT2Bins(token);
	result.setSuccess(Assert::assertEquals(this->config->getNumT2Bins(), (int) 10));
	return result;
}

TestResult CpmgConfigTest::readMinLambdaTest()
{
	TestResult result("CpmgConfig::readMinLambda()");
	string token = "1e-3";
	this->config->readMinLambda(token);
	result.setSuccess(Assert::assertEquals(this->config->getMinLambda(), (double) 0.001));
	return result;		
}

TestResult CpmgConfigTest::readMaxLambdaTest()
{
	TestResult result("CpmgConfig::readMinLambda()");
	string token = "10";
	this->config->readMaxLambda(token);
	result.setSuccess(Assert::assertEquals(this->config->getMaxLambda(), (double) 1e1));
	return result;			
}

TestResult CpmgConfigTest::readNumLambdasTest()
{
	TestResult result("CpmgConfig::readNumLambdas()");
	string token = "256.0";
	this->config->readNumLambdas(token);
	result.setSuccess(Assert::assertEquals(this->config->getNumLambdas(), (int) 256));
	return result;
}

TestResult CpmgConfigTest::readPruneNumTest()
{
	TestResult result("CpmgConfig::readPruneNum()");
	string token = "111.02";
	this->config->readPruneNum(token);
	result.setSuccess(Assert::assertEquals(this->config->getPruneNum(), (int) 111));
	return result;
}

TestResult CpmgConfigTest::readNoiseAmpTest()
{
	TestResult result("CpmgConfig::readNoiseAmp()");
	string token = "0.05";
	this->config->readNoiseAmp(token);
	result.setSuccess(Assert::assertEquals(this->config->getNoiseAmp(), (double) 0.05));
	return result;
}

TestResult CpmgConfigTest::readSaveModeTest_True()
{
	TestResult result("CpmgConfig::readSaveMode(true)");
	string token = "true";
	this->config->readSaveMode(token);
	result.setSuccess(Assert::assertTrue(this->config->getSaveMode()));
	return result;	
}

TestResult CpmgConfigTest::readSaveModeTest_False()
{
	TestResult result("CpmgConfig::readSaveMode(false)");
	string token = "false";
	this->config->readSaveMode(token);
	result.setSuccess(Assert::assertFalse(this->config->getSaveMode()));
	return result;	
}

TestResult CpmgConfigTest::readSaveModeTest_Unknown()
{
	TestResult result("CpmgConfig::readSaveMode(unknown)");
	string token = "ttt";
	this->config->readSaveMode(token);
	result.setSuccess(Assert::assertFalse(this->config->getSaveMode()));
	return result;	
}

TestResult CpmgConfigTest::readSaveWalkersTest_True()
{
	TestResult result("CpmgConfig::readSaveWalkers(true)");
	string token = "true";
	this->config->readSaveWalkers(token);
	result.setSuccess(Assert::assertTrue(this->config->getSaveWalkers()));
	return result;
}

TestResult CpmgConfigTest::readSaveWalkersTest_False()
{
	TestResult result("CpmgConfig::readSaveWalkers(false)");
	string token = "false";
	this->config->readSaveWalkers(token);
	result.setSuccess(Assert::assertFalse(this->config->getSaveWalkers()));
	return result;
}

TestResult CpmgConfigTest::readSaveWalkersTest_Unknown()
{
	TestResult result("CpmgConfig::readSaveWalkers(unknown)");
	string token = "bbb";
	this->config->readSaveWalkers(token);
	result.setSuccess(Assert::assertFalse(this->config->getSaveWalkers()));
	return result;
}

TestResult CpmgConfigTest::readSaveDecayTest_True()
{
	TestResult result("CpmgConfig::readSaveDecay(true)");
	string token = "true";
	this->config->readSaveDecay(token);
	result.setSuccess(Assert::assertTrue(this->config->getSaveDecay()));
	return result;
}

TestResult CpmgConfigTest::readSaveDecayTest_False()
{
	TestResult result("CpmgConfig::readSaveDecay(false)");
	string token = "das";
	this->config->readSaveDecay(token);
	result.setSuccess(Assert::assertFalse(this->config->getSaveDecay()));
	return result;
}


TestResult CpmgConfigTest::readSaveHistogramTest_True()
{
	TestResult result("CpmgConfig::readSaveHistogram(true)");
	string token = "true";
	this->config->readSaveHistogram(token);
	result.setSuccess(Assert::assertTrue(this->config->getSaveHistogram()));
	return result;
}

TestResult CpmgConfigTest::readSaveHistogramTest_False()
{
	TestResult result("CpmgConfig::readSaveHistogram(false)");
	string token = "FFF";
	this->config->readSaveHistogram(token);
	result.setSuccess(Assert::assertFalse(this->config->getSaveHistogram()));
	return result;
}

TestResult CpmgConfigTest::readSaveHistogramListTest_True()
{
	TestResult result("CpmgConfig::readSaveHistogramList(true)");
	string token = "true";
	this->config->readSaveHistogramList(token);
	result.setSuccess(Assert::assertTrue(this->config->getSaveHistogramList()));
	return result;
}   

TestResult CpmgConfigTest::readSaveT2Test_True()
{
	TestResult result("CpmgConfig::readSaveT2(true)");
	string token = "true";
	this->config->readSaveT2(token);
	result.setSuccess(Assert::assertTrue(this->config->getSaveT2()));
	return result;
}

TestResult CpmgConfigTest::readSaveT2Test_False()
{
	TestResult result("CpmgConfig::readSaveHistogram(true)");
	string token = "jae";
	this->config->readSaveT2(token);
	result.setSuccess(Assert::assertFalse(this->config->getSaveT2()));
	return result;
}
