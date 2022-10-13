#include "PfgseConfigTest.h"

PfgseConfigTest::PfgseConfigTest(string proot) : TestSuite(proot), config(NULL)
{
	// add test cases to suite vector
	(*this).addTest(&PfgseConfigTest::readConfigFileTest);
	(*this).addTest(&PfgseConfigTest::checkConfigTest_True);
    (*this).addTest(&PfgseConfigTest::checkConfigTest_False1);
    (*this).addTest(&PfgseConfigTest::checkConfigTest_False2);
    (*this).addTest(&PfgseConfigTest::checkConfigTest_False3);
}

vector<TestResult> PfgseConfigTest::run()
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

TestResult PfgseConfigTest::readConfigFileTest()
{	
	TestResult result;
	result.setMessage("PfgseConfig::readConfigFile()");

	string config_path = "tpfgse.config";	
	this->config->readConfigFile((*this).getInputDir() + config_path);	
	if(
        Assert::assertEquals(this->config->getPulseWidth(), (double) 0.1) and
        Assert::assertEquals(this->config->getMaxGradient().getX(), (double) 0.0) and
        Assert::assertEquals(this->config->getMaxGradient().getY(), (double) 0.0) and
        Assert::assertEquals(this->config->getMaxGradient().getZ(), (double) 100.0) and
        Assert::assertEquals(this->config->getGradientSamples(), (uint) 256) and
        Assert::assertFalse(this->config->getApplyBulk()) and
        Assert::assertTrue(this->config->getAllowWalkerSampling()) and 
        Assert::assertFalse(this->config->getApplyAbsorption()) and
        Assert::assertEquals(this->config->getTimeSequence(), (string)"manual") and
        Assert::assertEquals(this->config->getTimeSamples(), (uint) 4) and
        Assert::assertVectorEquals(this->config->getTimeValues(), (vector<double>){0.1,1.0,10.0,100.0}) and
        Assert::assertEquals(this->config->getTimeMin(), (double) 1.0) and        
        Assert::assertEquals(this->config->getTimeMax(), (double) 100.0) and 
        Assert::assertFalse(this->config->getApplyScaleFactor()) and        
        Assert::assertEquals(this->config->getInspectionLength(), (double) 10.0) and
        Assert::assertEquals(this->config->getNoiseAmp(), (double) 1e-4) and         
        Assert::assertEquals(this->config->getTargetSNR(), (double) -1.0) and      
        Assert::assertEquals(this->config->getThresholdType(), (string)"lhs") and      
        Assert::assertEquals(this->config->getThresholdValue(), (double) 0.9) and      
        Assert::assertEquals(this->config->getThresholdWindow(), (uint) 5) and      
        Assert::assertTrue(this->config->getSaveMode()) and 
        Assert::assertFalse(this->config->getSavePFGSE()) and
        Assert::assertFalse(this->config->getSaveWalkers()) and
        Assert::assertFalse(this->config->getSaveHistogram()) and
        Assert::assertFalse(this->config->getSaveHistogramList())
	) result.setSuccess(true);
	else result.setSuccess(false); 

	return result;
}

TestResult PfgseConfigTest::checkConfigTest_True()
{	
	TestResult result;
	result.setMessage("PfgseConfig::checkConfig(true)");

	string config_path = "tpfgse.config";	
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

TestResult PfgseConfigTest::checkConfigTest_False1()
{	
	TestResult result;
	result.setMessage("PfgseConfig::checkConfig(false1)");

	string config_path = "tpfgse.config";	
	this->config->readConfigFile((*this).getInputDir() + config_path);
    this->config->setPulseWidth(0.0);
	this->config->setGradientSamples(0);
    this->config->setTimeSamples(0);
    this->config->setTimeSequence("linear");
    this->config->setTimeMin(-0.1);
    this->config->setTimeMax(-1.0);
    this->config->setApplyScaleFactor(true);
    this->config->setInspectionLength(-0.001);
    this->config->setThresholdType("other");
    this->config->setThresholdValue(0.0);
    this->config->setThresholdWindow(0);    
	
	vector<string> output = this->config->checkConfig();
	vector<string> expected = {"PULSE_WIDTH", 
                               "GRADIENT_SAMPLES", 
                               "TIME_SAMPLES", 
                               "TIME_MIN", 
                               "TIME_MAX", 
                               "TIME_MIN>TIME_MAX",
                               "INSPECTION_LENGTH",
                               "THRESHOLD_TYPE",
                               "THRESHOLD_VALUE",
                               "THRESHOLD_WINDOW"};
	if(Assert::assertVectorEquals(output, expected) and 
	   Assert::assertFalse(this->config->getReady())
	) result.setSuccess(true);
	else {
        result.setSuccess(false);
        cout << "checkConfig(false1)" << endl;
        for (int i = 0; i < output.size(); i++) cout << output[i] << endl;
    }
	return result;
}

TestResult PfgseConfigTest::checkConfigTest_False2()
{	
	TestResult result;
	result.setMessage("PfgseConfig::checkConfig(false2)");

	string config_path = "tpfgse.config";	
	this->config->readConfigFile((*this).getInputDir() + config_path);
    this->config->setTimeSequence("manual");
    this->config->setTimeValues((vector<double>){0.1,1.0,-10.0,100.0});
    this->config->setTimeSamples(3);
    
	vector<string> output = this->config->checkConfig();
	vector<string> expected = {"TIME_VALUES!=TIME_SAMPLES", 
                               "TIME_VALUES(<0)"};
	if(Assert::assertVectorEquals(output, expected) and 
	   Assert::assertFalse(this->config->getReady())
	) result.setSuccess(true);
	else {
        result.setSuccess(false);
        cout << "checkConfig(false2)" << endl;
        for (int i = 0; i < output.size(); i++) cout << output[i] << endl;
    }
	return result;
}

TestResult PfgseConfigTest::checkConfigTest_False3()
{	
	TestResult result;
	result.setMessage("PfgseConfig::checkConfig(false3)");

	string config_path = "tpfgse.config";	
	this->config->readConfigFile((*this).getInputDir() + config_path);
    this->config->setTimeSequence("other");  
	
	vector<string> output = this->config->checkConfig();
	vector<string> expected = {"TIME_SEQUENCE"};
	if(Assert::assertVectorEquals(output, expected) and 
	   Assert::assertFalse(this->config->getReady())
	) result.setSuccess(true);
	else {
        result.setSuccess(false);
        cout << "checkConfig(false3)" << endl;
        for (int i = 0; i < output.size(); i++) cout << output[i] << endl;
    }
	return result;
}