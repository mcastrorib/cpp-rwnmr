#include "cpmg_configTest.h"

cpmg_configTest::cpmg_configTest(string proot) : TestSuite(proot), config(NULL)
{
	// add test cases to suite vector
	(*this).addTest(&cpmg_configTest::readConfigFileTest);
	(*this).addTest(&cpmg_configTest::readD0Test);
	(*this).addTest(&cpmg_configTest::readApplyBulkTest_True);
	(*this).addTest(&cpmg_configTest::readApplyBulkTest_False);
	(*this).addTest(&cpmg_configTest::readApplyBulkTest_Unknown);
	(*this).addTest(&cpmg_configTest::readTimeVerboseTest);
	(*this).addTest(&cpmg_configTest::readApplyBulkTest_Unknown);		
	(*this).addTest(&cpmg_configTest::readObservationTimeTest);
	(*this).addTest(&cpmg_configTest::readMethodTest_ImageBased);
	(*this).addTest(&cpmg_configTest::readMethodTest_Histogram);
	(*this).addTest(&cpmg_configTest::readMethodTest_Unknown);
	(*this).addTest(&cpmg_configTest::readResidualFieldTest_None);
	(*this).addTest(&cpmg_configTest::readResidualFieldTest_Uniform);
	(*this).addTest(&cpmg_configTest::readResidualFieldTest_Import);
	(*this).addTest(&cpmg_configTest::readResidualFieldTest_Unknown);
	(*this).addTest(&cpmg_configTest::readGradientValueTest);
	(*this).addTest(&cpmg_configTest::readGradientDirectionTest_0);
	(*this).addTest(&cpmg_configTest::readGradientDirectionTest_1);
	(*this).addTest(&cpmg_configTest::readGradientDirectionTest_2);
	(*this).addTest(&cpmg_configTest::readGradientDirectionTest_Unknown);
	(*this).addTest(&cpmg_configTest::readPathToFieldTest);
	(*this).addTest(&cpmg_configTest::readMinT2Test);
	(*this).addTest(&cpmg_configTest::readMaxT2Test);
	(*this).addTest(&cpmg_configTest::readUseT2LogspaceTest_True);
	(*this).addTest(&cpmg_configTest::readUseT2LogspaceTest_False);
	(*this).addTest(&cpmg_configTest::readUseT2LogspaceTest_Unknown);
	(*this).addTest(&cpmg_configTest::readNumT2BinsTest);
	(*this).addTest(&cpmg_configTest::readMinLambdaTest);
	(*this).addTest(&cpmg_configTest::readMaxLambdaTest);
	(*this).addTest(&cpmg_configTest::readNumLambdasTest);
	(*this).addTest(&cpmg_configTest::readPruneNumTest);
	(*this).addTest(&cpmg_configTest::readNoiseAmpTest); 
	(*this).addTest(&cpmg_configTest::readSaveModeTest_True);
	(*this).addTest(&cpmg_configTest::readSaveModeTest_False);
	(*this).addTest(&cpmg_configTest::readSaveModeTest_Unknown);
	(*this).addTest(&cpmg_configTest::readSaveWalkersTest_True);
	(*this).addTest(&cpmg_configTest::readSaveWalkersTest_False);
	(*this).addTest(&cpmg_configTest::readSaveWalkersTest_Unknown);
	(*this).addTest(&cpmg_configTest::readSaveDecayTest_True);
	(*this).addTest(&cpmg_configTest::readSaveDecayTest_False);
	(*this).addTest(&cpmg_configTest::readSaveHistogramTest_True);
	(*this).addTest(&cpmg_configTest::readSaveHistogramTest_False);
	(*this).addTest(&cpmg_configTest::readSaveHistogramListTest_True);
	(*this).addTest(&cpmg_configTest::readSaveT2Test_True);
	(*this).addTest(&cpmg_configTest::readSaveT2Test_False);	
}

vector<TestResult> cpmg_configTest::run()
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

TestResult cpmg_configTest::readConfigFileTest()
{	
	TestResult result;
	result.setMessage("cpmg_config::readConfigFile()");

	string config_path = "tcpmg.config";	
	this->config->readConfigFile((*this).getInputDir() + config_path);	
	if(Assert::assertEquals(this->config->getMethod(), (string)"image-based") and
	   Assert::assertEquals(this->config->getObservationTime(), (double) 1000.0) and
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

TestResult cpmg_configTest::readD0Test()
{
	TestResult result;
	result.setMessage("cpmg_config::readD0()");
	string token = "2.5";
	this->config->readD0(token);
	result.setSuccess(Assert::assertEquals(this->config->getD0(), (double) 2.5));
	return result;
}

TestResult cpmg_configTest::readApplyBulkTest_True()
{
	TestResult result;
	result.setMessage("cpmg_config::readApplyBulk(true)");
	string token = "true";
	this->config->readApplyBulk(token);
	result.setSuccess(Assert::assertTrue(this->config->getApplyBulk()));
	return result;
}

TestResult cpmg_configTest::readApplyBulkTest_False()
{
	TestResult result;
	result.setMessage("cpmg_config::readApplyBulk(false)");
	string token = "false";
	this->config->readApplyBulk(token);
	result.setSuccess(Assert::assertFalse(this->config->getApplyBulk()));
	return result;
}

TestResult cpmg_configTest::readApplyBulkTest_Unknown()
{
	TestResult result;
	result.setMessage("cpmg_config::Testing readApplyBulk(unknown)");
	string token = "da5s1d6";
	this->config->readApplyBulk(token);
	result.setSuccess(Assert::assertFalse(this->config->getApplyBulk()));
	return result;
}

TestResult cpmg_configTest::readTimeVerboseTest()
{
	TestResult result;
	result.setMessage("cpmg_config::readApplyBulk()");
	string token = "true";
	this->config->readTimeVerbose(token);
	result.setSuccess(Assert::assertEquals(this->config->getTimeVerbose(), (bool) true));
	return result;
}

TestResult cpmg_configTest::cpmg_configTest::readObservationTimeTest()
{
	TestResult result;
	result.setMessage("cpmg_config::readObservationTime()");
	string token = "1000.0";
	this->config->readObservationTime(token);
	result.setSuccess(Assert::assertEquals(this->config->getObservationTime(), (double) 1000.0));
	return result;
}

TestResult cpmg_configTest::readMethodTest_ImageBased()
{
	TestResult result;
	result.setMessage("cpmg_config::readMethod(image-based)");
	string token = "image-based";
	this->config->readMethod(token);
	result.setSuccess(Assert::assertEquals(this->config->getMethod(), (string) "image-based"));
	return result;
}

TestResult cpmg_configTest::readMethodTest_Histogram()
{
	TestResult result;
	result.setMessage("cpmg_config::readMethod(histogram)");
	string token = "histogram";
	this->config->readMethod(token);
	result.setSuccess(Assert::assertEquals(this->config->getMethod(), (string) "histogram"));
	return result;
}

TestResult cpmg_configTest::readMethodTest_Unknown()
{
	TestResult result;
	result.setMessage("cpmg_config::readMethod(unknown)");
	string token = "xxx";
	this->config->readMethod(token);
	result.setSuccess(Assert::assertEquals(this->config->getMethod(), (string) "image-based"));
	return result;
}

TestResult cpmg_configTest::readResidualFieldTest_None()
{
	TestResult result;
	result.setMessage("cpmg_config::readResidualField(none)");
	string token = "none";
	this->config->readResidualField(token);
	result.setSuccess(Assert::assertEquals(this->config->getResidualField(), (string) "none"));
	return result;
}

TestResult cpmg_configTest::readResidualFieldTest_Uniform()
{
	TestResult result;
	result.setMessage("cpmg_config::readResidualField(uniform)");
	string token = "uniform";
	this->config->readResidualField(token);
	result.setSuccess(Assert::assertEquals(this->config->getResidualField(), (string) "uniform"));
	return result;
}

TestResult cpmg_configTest::readResidualFieldTest_Import()
{
	TestResult result;
	result.setMessage("cpmg_config::readResidualField(import)");
	string token = "import";
	this->config->readResidualField(token);
	result.setSuccess(Assert::assertEquals(this->config->getResidualField(), (string) "import"));
	return result;
}

TestResult cpmg_configTest::readResidualFieldTest_Unknown()
{
	TestResult result;
	result.setMessage("cpmg_config::readResidualField(unknown)");
	string token = "dsasd";
	this->config->readResidualField(token);
	result.setSuccess(Assert::assertEquals(this->config->getResidualField(), (string) "none"));
	return result;
}

TestResult cpmg_configTest::readGradientValueTest()
{
	TestResult result;
	result.setMessage("cpmg_config::readGradientValue()");
	string token = "1.0";
	this->config->readGradientValue(token);
	result.setSuccess(Assert::assertEquals(this->config->getGradientValue(), (double) 1.0));
	return result;
}

TestResult cpmg_configTest::readGradientDirectionTest_0()
{
	TestResult result;
	result.setMessage("cpmg_config::readGradientDirection(0)");
	string token = "0";
	this->config->readGradientDirection(token);
	result.setSuccess(Assert::assertEquals(this->config->getGradientDirection(), (int) 0));
	return result;	
}

TestResult cpmg_configTest::readGradientDirectionTest_1()
{
	TestResult result;
	result.setMessage("cpmg_config::readGradientDirection(1)");
	string token = "1";
	this->config->readGradientDirection(token);
	result.setSuccess(Assert::assertEquals(this->config->getGradientDirection(), (int) 1));
	return result;
}

TestResult cpmg_configTest::readGradientDirectionTest_2()
{
	TestResult result;
	result.setMessage("cpmg_config::readGradientDirection(2)");
	string token = "2";
	this->config->readGradientDirection(token);
	result.setSuccess(Assert::assertEquals(this->config->getGradientDirection(), (int) 2));
	return result;	
}

TestResult cpmg_configTest::readGradientDirectionTest_Unknown()
{
	TestResult result;
	result.setMessage("cpmg_config::readGradientDirection(Unknown)");
	string token = "xx x";
	this->config->readGradientDirection(token);
	result.setSuccess(Assert::assertEquals(this->config->getGradientDirection(), (int) 2));
	return result;	
}

TestResult cpmg_configTest::readPathToFieldTest()
{
	TestResult result;
	result.setMessage("cpmg_config::readPathToField()");
	string token = "my-path";
	this->config->readPathToField(token);
	result.setSuccess(Assert::assertEquals(this->config->getPathToField(), (string) "my-path"));
	return result;	
}

TestResult cpmg_configTest::readMinT2Test()
{
	TestResult result;
	result.setMessage("cpmg_config::readMinT2()");
	string token = "0.1";
	this->config->readMinT2(token);
	result.setSuccess(Assert::assertEquals(this->config->getMinT2(), (double) 0.1));
	return result;		
}

TestResult cpmg_configTest::readMaxT2Test()
{
	TestResult result;
	result.setMessage("cpmg_config::readMaxT2()");
	string token = "10";
	this->config->readMaxT2(token);
	result.setSuccess(Assert::assertEquals(this->config->getMaxT2(), (double) 10.0));
	return result;			
}

TestResult cpmg_configTest::readUseT2LogspaceTest_True()
{
	TestResult result;
	result.setMessage("cpmg_config::readUseT2Logspace(true)");
	string token = "true";
	this->config->readUseT2Logspace(token);
	result.setSuccess(Assert::assertTrue(this->config->getUseT2Logspace()));
	return result;		
}

TestResult cpmg_configTest::readUseT2LogspaceTest_False()
{
	TestResult result;
	result.setMessage("cpmg_config::readUseT2Logspace(false)");
	string token = "false";
	this->config->readUseT2Logspace(token);
	result.setSuccess(Assert::assertFalse(this->config->getUseT2Logspace()));
	return result;		
}

TestResult cpmg_configTest::readUseT2LogspaceTest_Unknown()
{
	TestResult result;
	result.setMessage("cpmg_config::readUseT2Logspace(true)");
	string token = "nnn";
	this->config->readUseT2Logspace(token);
	result.setSuccess(Assert::assertFalse(this->config->getUseT2Logspace()));
	return result;		
}

TestResult cpmg_configTest::readNumT2BinsTest()
{
	TestResult result;
	result.setMessage("cpmg_config::readNumT2Bins()");
	string token = "10";
	this->config->readNumT2Bins(token);
	result.setSuccess(Assert::assertEquals(this->config->getNumT2Bins(), (int) 10));
	return result;
}

TestResult cpmg_configTest::readMinLambdaTest()
{
	TestResult result;
	result.setMessage("cpmg_config::readMinLambda()");
	string token = "1e-3";
	this->config->readMinLambda(token);
	result.setSuccess(Assert::assertEquals(this->config->getMinLambda(), (double) 0.001));
	return result;		
}

TestResult cpmg_configTest::readMaxLambdaTest()
{
	TestResult result;
	result.setMessage("cpmg_config::readMinLambda()");
	string token = "10";
	this->config->readMaxLambda(token);
	result.setSuccess(Assert::assertEquals(this->config->getMaxLambda(), (double) 1e1));
	return result;			
}

TestResult cpmg_configTest::readNumLambdasTest()
{
	TestResult result;
	result.setMessage("cpmg_config::readNumLambdas()");
	string token = "256.0";
	this->config->readNumLambdas(token);
	result.setSuccess(Assert::assertEquals(this->config->getNumLambdas(), (int) 256));
	return result;
}

TestResult cpmg_configTest::readPruneNumTest()
{
	TestResult result;
	result.setMessage("cpmg_config::readPruneNum()");
	string token = "111.02";
	this->config->readPruneNum(token);
	result.setSuccess(Assert::assertEquals(this->config->getPruneNum(), (int) 111));
	return result;
}

TestResult cpmg_configTest::readNoiseAmpTest()
{
	TestResult result;
	result.setMessage("cpmg_config::readNoiseAmp()");
	string token = "0.05";
	this->config->readNoiseAmp(token);
	result.setSuccess(Assert::assertEquals(this->config->getNoiseAmp(), (double) 0.05));
	return result;
}

TestResult cpmg_configTest::readSaveModeTest_True()
{
	TestResult result;
	result.setMessage("cpmg_config::readSaveMode(true)");
	string token = "true";
	this->config->readSaveMode(token);
	result.setSuccess(Assert::assertTrue(this->config->getSaveMode()));
	return result;	
}

TestResult cpmg_configTest::readSaveModeTest_False()
{
	TestResult result;
	result.setMessage("cpmg_config::readSaveMode(false)");
	string token = "false";
	this->config->readSaveMode(token);
	result.setSuccess(Assert::assertFalse(this->config->getSaveMode()));
	return result;	
}

TestResult cpmg_configTest::readSaveModeTest_Unknown()
{
	TestResult result;
	result.setMessage("cpmg_config::readSaveMode(unknown)");
	string token = "ttt";
	this->config->readSaveMode(token);
	result.setSuccess(Assert::assertFalse(this->config->getSaveMode()));
	return result;	
}

TestResult cpmg_configTest::readSaveWalkersTest_True()
{
	TestResult result;
	result.setMessage("cpmg_config::readSaveWalkers(true)");
	string token = "true";
	this->config->readSaveWalkers(token);
	result.setSuccess(Assert::assertTrue(this->config->getSaveWalkers()));
	return result;
}

TestResult cpmg_configTest::readSaveWalkersTest_False()
{
	TestResult result;
	result.setMessage("cpmg_config::readSaveWalkers(false)");
	string token = "false";
	this->config->readSaveWalkers(token);
	result.setSuccess(Assert::assertFalse(this->config->getSaveWalkers()));
	return result;
}

TestResult cpmg_configTest::readSaveWalkersTest_Unknown()
{
	TestResult result;
	result.setMessage("cpmg_config::readSaveWalkers(unknown)");
	string token = "bbb";
	this->config->readSaveWalkers(token);
	result.setSuccess(Assert::assertFalse(this->config->getSaveWalkers()));
	return result;
}

TestResult cpmg_configTest::readSaveDecayTest_True()
{
	TestResult result;
	result.setMessage("cpmg_config::readSaveDecay(true)");
	string token = "true";
	this->config->readSaveDecay(token);
	result.setSuccess(Assert::assertTrue(this->config->getSaveDecay()));
	return result;
}

TestResult cpmg_configTest::readSaveDecayTest_False()
{
	TestResult result;
	result.setMessage("cpmg_config::readSaveDecay(false)");
	string token = "das";
	this->config->readSaveDecay(token);
	result.setSuccess(Assert::assertFalse(this->config->getSaveDecay()));
	return result;
}


TestResult cpmg_configTest::readSaveHistogramTest_True()
{
	TestResult result;
	result.setMessage("cpmg_config::readSaveHistogram(true)");
	string token = "true";
	this->config->readSaveHistogram(token);
	result.setSuccess(Assert::assertTrue(this->config->getSaveHistogram()));
	return result;
}

TestResult cpmg_configTest::readSaveHistogramTest_False()
{
	TestResult result;
	result.setMessage("cpmg_config::readSaveHistogram(false)");
	string token = "FFF";
	this->config->readSaveHistogram(token);
	result.setSuccess(Assert::assertFalse(this->config->getSaveHistogram()));
	return result;
}

TestResult cpmg_configTest::readSaveHistogramListTest_True()
{
	TestResult result;
	result.setMessage("cpmg_config::readSaveHistogramList(true)");
	string token = "true";
	this->config->readSaveHistogramList(token);
	result.setSuccess(Assert::assertTrue(this->config->getSaveHistogramList()));
	return result;
}   

TestResult cpmg_configTest::readSaveT2Test_True()
{
	TestResult result;
	result.setMessage("cpmg_config::readSaveT2(true)");
	string token = "true";
	this->config->readSaveT2(token);
	result.setSuccess(Assert::assertTrue(this->config->getSaveT2()));
	return result;
}

TestResult cpmg_configTest::readSaveT2Test_False()
{
	TestResult result;
	result.setMessage("cpmg_config::readSaveHistogram(true)");
	string token = "jae";
	this->config->readSaveT2(token);
	result.setSuccess(Assert::assertFalse(this->config->getSaveT2()));
	return result;
}
