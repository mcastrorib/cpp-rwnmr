#include "cpmg_configTest.h"

void cpmg_configTest::run()
{
	for(int t = 0; t < this->testSuite.size(); t++)
	{
		(*this).beforeEach();
		(this->*testSuite[t])();
		(*this).afterEach();
	}
	return;
}

void cpmg_configTest::readConfigFileTest()
{	
	cout << "::Testing readConfigFile()...";

	string config_path = "tcpmg.config";
	
	this->config->readConfigFile((*this).getInputDir() + config_path);
	Assert::assertEquals(this->config->getMethod(), (string)"image-based", "cpmg: Method");
	Assert::assertEquals(this->config->getObservationTime(), (double) 1000.0, "cpmg: ObservationTime");
	Assert::assertEquals(this->config->getApplyBulk(), (bool) false, "cpmg: ApplyBulk");
	Assert::assertEquals(this->config->getTimeVerbose(), (bool) false, "cpmg: TimeVerbose");
	Assert::assertEquals(this->config->getResidualField(), (string)"uniform", "cpmg: ResidualField");
	Assert::assertEquals(this->config->getGradientValue(), (double) 1.0, "cpmg: GradientValue");
	Assert::assertEquals(this->config->getGradientDirection(), (int) 2, "cpmg: GradientDirection");
	Assert::assertEquals(this->config->getMinT2(), (double) 0.1, "cpmg: MinT2");
	Assert::assertEquals(this->config->getMaxT2(), (double) 10000.0, "cpmg: MaxT2");
	Assert::assertEquals(this->config->getUseT2Logspace(), (bool) true, "cpmg: UseT2Logspace");
	Assert::assertEquals(this->config->getNumT2Bins(), (int) 256, "cpmg: NumT2Bins");
	Assert::assertEquals(this->config->getMinLambda(), (double) -4.0, "cpmg: MinLambda");
	Assert::assertEquals(this->config->getMaxLambda(), (double) 2.0, "cpmg: MaxLambda");
	Assert::assertEquals(this->config->getNumLambdas(), (int) 512, "cpmg: NumLambdas");
	Assert::assertEquals(this->config->getPruneNum(), (int) 512, "cpmg: PruneNum");
	Assert::assertEquals(this->config->getNoiseAmp(), (double) 0.0, "cpmg: NoiseAmp");
	Assert::assertEquals(this->config->getSaveMode(), (bool) true, "cpmg: SaveMode");
	Assert::assertEquals(this->config->getSaveDecay(), (bool) false, "cpmg: SaveDecay");
	Assert::assertEquals(this->config->getSaveT2(), (bool) false, "cpmg: SaveT2");
	Assert::assertEquals(this->config->getSaveWalkers(), (bool) false, "cpmg: SaveWalkers");
	Assert::assertEquals(this->config->getSaveHistogram(), (bool) false, "cpmg: SaveHistogram");
	Assert::assertEquals(this->config->getSaveHistogramList(), (bool) false, "cpmg: SaveHistogramList");

	cout << "Ok." << endl;
	return;
}

void cpmg_configTest::readD0Test()
{
	cout << "::Testing readD0()...";
	string token = "2.5";
	this->config->readD0(token);
	Assert::assertEquals(this->config->getD0(), (double) 2.5, "Failed.");
	cout << "Ok." << endl;
}

void cpmg_configTest::readApplyBulkTest_True()
{
	cout << "::Testing readApplyBulk(true)...";
	string token = "true";
	this->config->readApplyBulk(token);
	Assert::assertTrue(this->config->getApplyBulk(), "Failed.");
	cout << "Ok." << endl;
}

void cpmg_configTest::readApplyBulkTest_False()
{
	cout << "::Testing readApplyBulk(false)...";
	string token = "false";
	this->config->readApplyBulk(token);
	Assert::assertFalse(this->config->getApplyBulk(), "Failed.");
	cout << "Ok." << endl;
}

void cpmg_configTest::readApplyBulkTest_Unknown()
{
	cout << "::Testing readApplyBulk(unknown)...";
	string token = "da5s1d6";
	this->config->readApplyBulk(token);
	Assert::assertFalse(this->config->getApplyBulk(), "Failed.");
	cout << "Ok." << endl;
}

void cpmg_configTest::readTimeVerboseTest()
{
	cout << "::Testing readApplyBulk()...";
	string token = "true";
	this->config->readTimeVerbose(token);
	Assert::assertEquals(this->config->getTimeVerbose(), (bool) true, "Failed.");
	cout << "Ok." << endl;
}

void cpmg_configTest::cpmg_configTest::readObservationTimeTest()
{
	cout << "::Testing readObservationTime()...";
	string token = "1000.0";
	this->config->readObservationTime(token);
	Assert::assertEquals(this->config->getObservationTime(), (double) 1000.0, "Failed.");
	cout << "Ok." << endl;
}

void cpmg_configTest::readMethodTest_ImageBased()
{
	cout << "::Testing readMethod(image-based)...";
	string token = "image-based";
	this->config->readMethod(token);
	Assert::assertEquals(this->config->getMethod(), (string) "image-based", "Failed.");
	cout << "Ok." << endl;
}

void cpmg_configTest::readMethodTest_Histogram()
{
	cout << "::Testing readMethod(histogram)...";
	string token = "histogram";
	this->config->readMethod(token);
	Assert::assertEquals(this->config->getMethod(), (string) "histogram", "Failed.");
	cout << "Ok." << endl;
}

void cpmg_configTest::readMethodTest_Unknown()
{
	cout << "::Testing readMethod(unknown)...";
	string token = "xxx";
	this->config->readMethod(token);
	Assert::assertEquals(this->config->getMethod(), (string) "image-based", "Failed.");
	cout << "Ok." << endl;
}

void cpmg_configTest::readResidualFieldTest_None()
{
	cout << "::Testing readResidualField(none)...";
	string token = "none";
	this->config->readResidualField(token);
	Assert::assertEquals(this->config->getResidualField(), (string) "none", "Failed.");
	cout << "Ok." << endl;
}

void cpmg_configTest::readResidualFieldTest_Uniform()
{
	cout << "::Testing readResidualField(uniform)...";
	string token = "uniform";
	this->config->readResidualField(token);
	Assert::assertEquals(this->config->getResidualField(), (string) "uniform", "Failed.");
	cout << "Ok." << endl;
}

void cpmg_configTest::readResidualFieldTest_Import()
{
	cout << "::Testing readResidualField(import)...";
	string token = "import";
	this->config->readResidualField(token);
	Assert::assertEquals(this->config->getResidualField(), (string) "import", "Failed.");
	cout << "Ok." << endl;
}

void cpmg_configTest::readResidualFieldTest_Unknown()
{
	cout << "::Testing readResidualField(unknown)...";
	string token = "dsasd";
	this->config->readResidualField(token);
	Assert::assertEquals(this->config->getResidualField(), (string) "none", "Failed.");
	cout << "Ok." << endl;
}

void cpmg_configTest::readGradientValueTest()
{
	cout << "::Testing readGradientValue()...";
	string token = "1.0";
	this->config->readGradientValue(token);
	Assert::assertEquals(this->config->getGradientValue(), (double) 1.0, "Failed.");
	cout << "Ok." << endl;
}

void cpmg_configTest::readGradientDirectionTest_0()
{
	cout << "::Testing readGradientDirection(0)...";
	string token = "0";
	this->config->readGradientDirection(token);
	Assert::assertEquals(this->config->getGradientDirection(), (int) 0, "Failed.");
	cout << "Ok." << endl;	
}

void cpmg_configTest::readGradientDirectionTest_1()
{
	cout << "::Testing readGradientDirection(1)...";
	string token = "1";
	this->config->readGradientDirection(token);
	Assert::assertEquals(this->config->getGradientDirection(), (int) 1, "Failed.");
	cout << "Ok." << endl;
}

void cpmg_configTest::readGradientDirectionTest_2()
{
	cout << "::Testing readGradientDirection(2)...";
	string token = "2";
	this->config->readGradientDirection(token);
	Assert::assertEquals(this->config->getGradientDirection(), (int) 2, "Failed.");
	cout << "Ok." << endl;	
}

void cpmg_configTest::readGradientDirectionTest_Unknown()
{
	cout << "::Testing readGradientDirection(Unknown)...";
	string token = "xx x";
	this->config->readGradientDirection(token);
	Assert::assertEquals(this->config->getGradientDirection(), (int) 2, "Failed.");
	cout << "Ok." << endl;	
}

void cpmg_configTest::readPathToFieldTest()
{
	cout << "::Testing readPathToField()...";
	string token = "my-path";
	this->config->readPathToField(token);
	Assert::assertEquals(this->config->getPathToField(), (string) "my-path", "Failed.");
	cout << "Ok." << endl;	
}

void cpmg_configTest::readMinT2Test()
{
	cout << "::Testing readMinT2()...";
	string token = "0.1";
	this->config->readMinT2(token);
	Assert::assertEquals(this->config->getMinT2(), (double) 0.1, "Failed.");
	cout << "Ok." << endl;		
}

void cpmg_configTest::readMaxT2Test()
{
	cout << "::Testing readMaxT2()...";
	string token = "10";
	this->config->readMaxT2(token);
	Assert::assertEquals(this->config->getMaxT2(), (double) 10.0, "Failed.");
	cout << "Ok." << endl;			
}

void cpmg_configTest::readUseT2LogspaceTest_True()
{
	cout << "::Testing readUseT2Logspace(true)...";
	string token = "true";
	this->config->readUseT2Logspace(token);
	Assert::assertTrue(this->config->getUseT2Logspace(), "Failed.");
	cout << "Ok." << endl;		
}

void cpmg_configTest::readUseT2LogspaceTest_False()
{
	cout << "::Testing readUseT2Logspace(false)...";
	string token = "false";
	this->config->readUseT2Logspace(token);
	Assert::assertFalse(this->config->getUseT2Logspace(), "Failed.");
	cout << "Ok." << endl;		
}

void cpmg_configTest::readUseT2LogspaceTest_Unknown()
{
	cout << "::Testing readUseT2Logspace(true)...";
	string token = "nnn";
	this->config->readUseT2Logspace(token);
	Assert::assertFalse(this->config->getUseT2Logspace(), "Failed.");
	cout << "Ok." << endl;		
}

void cpmg_configTest::readNumT2BinsTest()
{
	cout << "::Testing readNumT2Bins()...";
	string token = "10";
	this->config->readNumT2Bins(token);
	Assert::assertEquals(this->config->getNumT2Bins(), (int) 10, "Failed.");
	cout << "Ok." << endl;
}

void cpmg_configTest::readMinLambdaTest()
{
	cout << "::Testing readMinLambda()...";
	string token = "1e-3";
	this->config->readMinLambda(token);
	Assert::assertEquals(this->config->getMinLambda(), (double) 0.001, "Failed.");
	cout << "Ok." << endl;		
}

void cpmg_configTest::readMaxLambdaTest()
{
	cout << "::Testing readMinLambda()...";
	string token = "10";
	this->config->readMaxLambda(token);
	Assert::assertEquals(this->config->getMaxLambda(), (double) 1e1, "Failed.");
	cout << "Ok." << endl;			
}

void cpmg_configTest::readNumLambdasTest()
{
	cout << "::Testing readNumLambdas()...";
	string token = "256.0";
	this->config->readNumLambdas(token);
	Assert::assertEquals(this->config->getNumLambdas(), (int) 256, "Failed.");
	cout << "Ok." << endl;
}

void cpmg_configTest::readPruneNumTest()
{
	cout << "::Testing readPruneNum()...";
	string token = "111.02";
	this->config->readPruneNum(token);
	Assert::assertEquals(this->config->getPruneNum(), (int) 111, "Failed.");
	cout << "Ok." << endl;
}

void cpmg_configTest::readNoiseAmpTest()
{
	cout << "::Testing readNoiseAmp()...";
	string token = "0.05";
	this->config->readNoiseAmp(token);
	Assert::assertEquals(this->config->getNoiseAmp(), (double) 0.05, "Failed.");
	cout << "Ok." << endl;
}

void cpmg_configTest::readSaveModeTest_True()
{
	cout << "::Testing readSaveMode(true)...";
	string token = "true";
	this->config->readSaveMode(token);
	Assert::assertTrue(this->config->getSaveMode(), "Failed.");
	cout << "Ok." << endl;	
}

void cpmg_configTest::readSaveModeTest_False()
{
	cout << "::Testing readSaveMode(false)...";
	string token = "false";
	this->config->readSaveMode(token);
	Assert::assertFalse(this->config->getSaveMode(), "Failed.");
	cout << "Ok." << endl;	
}

void cpmg_configTest::readSaveModeTest_Unknown()
{
	cout << "::Testing readSaveMode(unknown)...";
	string token = "ttt";
	this->config->readSaveMode(token);
	Assert::assertFalse(this->config->getSaveMode(), "Failed.");
	cout << "Ok." << endl;	
}

void cpmg_configTest::readSaveWalkersTest_True()
{
	cout << "::Testing readSaveWalkers(true)...";
	string token = "true";
	this->config->readSaveWalkers(token);
	Assert::assertTrue(this->config->getSaveWalkers(), "Failed.");
	cout << "Ok." << endl;
}

void cpmg_configTest::readSaveWalkersTest_False()
{
	cout << "::Testing readSaveWalkers(false)...";
	string token = "false";
	this->config->readSaveWalkers(token);
	Assert::assertFalse(this->config->getSaveWalkers(), "Failed.");
	cout << "Ok." << endl;
}

void cpmg_configTest::readSaveWalkersTest_Unknown()
{
	cout << "::Testing readSaveWalkers(unknown)...";
	string token = "bbb";
	this->config->readSaveWalkers(token);
	Assert::assertFalse(this->config->getSaveWalkers(), "Failed.");
	cout << "Ok." << endl;
}

void cpmg_configTest::readSaveDecayTest_True()
{
	cout << "::Testing readSaveDecay(true)...";
	string token = "true";
	this->config->readSaveDecay(token);
	Assert::assertTrue(this->config->getSaveDecay(), "Failed.");
	cout << "Ok." << endl;
}

void cpmg_configTest::readSaveDecayTest_False()
{
	cout << "::Testing readSaveDecay(false)...";
	string token = "das";
	this->config->readSaveDecay(token);
	Assert::assertFalse(this->config->getSaveDecay(), "Failed.");
	cout << "Ok." << endl;
}


void cpmg_configTest::readSaveHistogramTest_True()
{
	cout << "::Testing readSaveHistogram(true)...";
	string token = "true";
	this->config->readSaveHistogram(token);
	Assert::assertTrue(this->config->getSaveHistogram(), "Failed.");
	cout << "Ok." << endl;
}

void cpmg_configTest::readSaveHistogramTest_False()
{
	cout << "::Testing readSaveHistogram(false)...";
	string token = "FFF";
	this->config->readSaveHistogram(token);
	Assert::assertFalse(this->config->getSaveHistogram(), "Failed.");
	cout << "Ok." << endl;
}

void cpmg_configTest::readSaveHistogramListTest_True()
{
	cout << "::Testing readSaveHistogramList(true)...";
	string token = "true";
	this->config->readSaveHistogramList(token);
	Assert::assertTrue(this->config->getSaveHistogramList(), "Failed.");
	cout << "Ok." << endl;
}   

void cpmg_configTest::readSaveT2Test_True()
{
	cout << "::Testing readSaveT2(true)...";
	string token = "true";
	this->config->readSaveT2(token);
	Assert::assertTrue(this->config->getSaveT2(), "Failed.");
	cout << "Ok." << endl;
}

void cpmg_configTest::readSaveT2Test_False()
{
	cout << "::Testing readSaveHistogram(true)...";
	string token = "jae";
	this->config->readSaveT2(token);
	Assert::assertFalse(this->config->getSaveT2(), "Failed.");
	cout << "Ok." << endl;
}
