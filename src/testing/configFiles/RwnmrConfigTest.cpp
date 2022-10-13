#include "RwnmrConfigTest.h"

RwnmrConfigTest::RwnmrConfigTest(string proot) : TestSuite(proot), config(NULL)
{
	// add test cases to suite vector
	(*this).addTest(&RwnmrConfigTest::readConfigFileTest);
	(*this).addTest(&RwnmrConfigTest::checkConfigTest_True);
    (*this).addTest(&RwnmrConfigTest::checkConfigTest_False);
}

vector<TestResult> RwnmrConfigTest::run()
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

TestResult RwnmrConfigTest::readConfigFileTest()
{	
	TestResult result;
	result.setMessage("RwnmrConfig::readConfigFile()");

	string config_path = "trwnmr.config";	
	this->config->readConfigFile((*this).getInputDir() + config_path);	
	if(
        Assert::assertEquals(this->config->getName(), (string)"NMR_TestSimulation") and
        Assert::assertEquals(this->config->getD0(), (double) 2.5) and
        Assert::assertEquals(this->config->getGiromagneticRatio(), (double) 42.576) and
        Assert::assertEquals(this->config->getGiromagneticUnit(), (string)"mhertz") and
        Assert::assertEquals(this->config->getBulkTime(), (double)2800.0) and
        Assert::assertEquals(this->config->getRhoType(), (string)"uniform") and
        Assert::assertVectorEquals(this->config->getRho(), (vector<double>) {10.0}) and
        Assert::assertEquals(this->config->getWalkers(), (uint) 1000) and
        Assert::assertEquals(this->config->getWalkerSamples(), (uint) 1) and
        Assert::assertEquals(this->config->getWalkersPlacement(), (string)"random") and
        Assert::assertEquals(this->config->getPlacementDeviation(), (uint) 1) and
        Assert::assertEquals(this->config->getStepsPerEcho(), (uint) 4) and
        Assert::assertEquals(this->config->getSeed(), (uint64_t) 43) and
        Assert::assertEquals(this->config->getBC(), (string)"mirror") and
        Assert::assertEquals(this->config->getSaveImgInfo(), (bool) false) and
        Assert::assertEquals(this->config->getSaveBinImg(), (bool) false) and
        Assert::assertEquals(this->config->getSaveWalkers(), (bool) false) and
        Assert::assertEquals(this->config->getHistograms(), (uint) 1) and
        Assert::assertEquals(this->config->getHistogramSize(), (uint) 256) and
        Assert::assertEquals(this->config->getHistogramScale(), (string)"linear") and
        Assert::assertEquals(this->config->getOpenMPUsage(), (bool) true) and 
        Assert::assertEquals(this->config->getOpenMPThreads(), (uint) 4) and
        Assert::assertEquals(this->config->getGPUUsage(), (bool) true) and
        Assert::assertEquals(this->config->getBlocks(), (uint) 4096) and
        Assert::assertEquals(this->config->getThreadsPerBlock(), (uint) 1024) and
        Assert::assertEquals(this->config->getEchoesPerKernel(), (uint) 16) and
        Assert::assertEquals(this->config->getReduceInGPU(), (bool) true) and
        Assert::assertEquals(this->config->getMaxRWSteps(), (uint) 65536)  	   
	) result.setSuccess(true);
	else result.setSuccess(false); 

	return result;
}

TestResult RwnmrConfigTest::checkConfigTest_True()
{	
	TestResult result;
	result.setMessage("RwnmrConfig::checkConfig(true)");

	string config_path = "trwnmr.config";	
	this->config->readConfigFile((*this).getInputDir() + config_path);
	vector<string> output = this->config->checkConfig();
	vector<string> expected = {};
	if(Assert::assertVectorEquals(output, expected) and 
	   Assert::assertTrue(this->config->getReady())
	) result.setSuccess(true);
	else {
		result.setSuccess(false);
        cout << "checkConfig(true)" << endl;
		for(int i = 0; i < output.size(); i++) cout << output[i] << endl;
	}
	return result;
}

TestResult RwnmrConfigTest::checkConfigTest_False()
{	
	TestResult result;
	result.setMessage("RwnmrConfig::checkConfig(false)");

	string config_path = "trwnmr.config";	
	this->config->readConfigFile((*this).getInputDir() + config_path);
	this->config->setWalkers(0);
    this->config->setWalkerSamples(0);
    this->config->setWalkersPlacement("bizarre");
    this->config->setRhoType("strange");
    this->config->setGiromagneticRatio(0);
    this->config->setGiromagneticUnit("ms");
    this->config->setD0(-1.0);
    this->config->setBulkTime(-0.1);
    this->config->setStepsPerEcho(0);
    this->config->setBC("rand");
    this->config->setHistograms(0);
    this->config->setHistogramSize(0);
    this->config->setHistogramScale("other");
    this->config->setOpenMPThreads(0);
    this->config->setBlocks(0);
    this->config->setThreadsPerBlock(0);
    this->config->setEchoesPerKernel(0);
    this->config->setMaxRWSteps(0);
	
	vector<string> output = this->config->checkConfig();
	vector<string> expected = {"WALKERS", 
                               "WALKER_SAMPLES", 
                               "WALKER_PLACEMENT", 
                               "RHO_TYPE", 
                               "GYROMAGNETIC_RATIO", 
                               "GYROMAGNETIC_UNIT",
                               "D0",
                               "BULK_TIME",
                               "STEPS_PER_ECHO",
                               "BC",  
                               "HISTOGRAMS",
                               "HISTOGRAM_SIZE",
                               "HISTOGRAM_SCALE",
                               "OPENMP",
                               "CUDA_BLOCKS",
                               "CUDA_THREADSPERBLOCK",
                               "CUDA_ECHOESPERKERNEL",
                               "CUDA_MAXRWSTEPSPERKERNEL"};
	if(Assert::assertVectorEquals(output, expected) and 
	   Assert::assertFalse(this->config->getReady())
	) result.setSuccess(true);
	else {
        result.setSuccess(false);
        cout << "checkConfig(false)" << endl;
        for (int i = 0; i < output.size(); i++) cout << output[i] << endl;
    }
	return result;
}
