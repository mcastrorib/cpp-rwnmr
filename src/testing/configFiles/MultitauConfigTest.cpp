#include "MultitauConfigTest.h"

MultitauConfigTest::MultitauConfigTest(string proot) : TestSuite(proot), config(NULL)
{
	// add test cases to suite vector
	(*this).addTest(&MultitauConfigTest::readConfigFileTest);
	(*this).addTest(&MultitauConfigTest::checkConfigTest_True);
    (*this).addTest(&MultitauConfigTest::checkConfigTest_False1);
    (*this).addTest(&MultitauConfigTest::checkConfigTest_False2);
    (*this).addTest(&MultitauConfigTest::checkConfigTest_False3);
}

vector<TestResult> MultitauConfigTest::run()
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

TestResult MultitauConfigTest::readConfigFileTest()
{	
	TestResult result;
	result.setMessage("MultitauConfig::readConfigFile()");

	string config_path = "tmultitau.config";	
	this->config->readConfigFile((*this).getInputDir() + config_path);	
	if(
        Assert::assertEquals(this->config->getTauScale(), (string)"manual") and
        Assert::assertVectorEquals(this->config->getTauValues(), (vector<double>) {0.1,0.2,1.0,2.0}) and
        Assert::assertEquals(this->config->getTauMin(), (double) 0.1) and
        Assert::assertEquals(this->config->getTauMax(), (double) 1.0) and
        Assert::assertEquals(this->config->getTauPoints(), (int) 4) and
        Assert::assertTrue(this->config->getSaveMode()) and
        Assert::assertFalse(this->config->getSaveDecay()) and
        Assert::assertFalse(this->config->getSaveWalkers()) and
        Assert::assertFalse(this->config->getSaveHistogram()) and
        Assert::assertFalse(this->config->getSaveHistogramList())
	) result.setSuccess(true);
	else result.setSuccess(false); 

	return result;
}

TestResult MultitauConfigTest::checkConfigTest_True()
{	
	TestResult result;
	result.setMessage("MultitauConfig::checkConfig(true)");

	string config_path = "tmultitau.config";	
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

TestResult MultitauConfigTest::checkConfigTest_False1()
{	
	TestResult result;
	result.setMessage("MultitauConfig::checkConfig(false1)");

	string config_path = "tmultitau.config";	
	this->config->readConfigFile((*this).getInputDir() + config_path);
    this->config->setTauScale("log");
	this->config->setTauMin(-0.1);
    this->config->setTauMax(-1.0);
    this->config->setTauPoints(0);

    
	
	vector<string> output = this->config->checkConfig();
	vector<string> expected = {"TAU_POINTS", 
                               "TAU_MIN", 
                               "TAU_MAX",
                               "TAU_MIN>TAU_MAX"};
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

TestResult MultitauConfigTest::checkConfigTest_False2()
{	
	TestResult result;
	result.setMessage("MultitauConfig::checkConfig(false2)");

	string config_path = "tmultitau.config";	
	this->config->readConfigFile((*this).getInputDir() + config_path);
    this->config->setTauScale("manual");
	this->config->setTauValues((vector<double> {0.1, 0.2, -0.1}));
    this->config->setTauPoints(2);    
	
	vector<string> output = this->config->checkConfig();
	vector<string> expected = {"TAU_VALUES!=TAU_POINTS", 
                               "TAU_VALUES(<0)"};
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

TestResult MultitauConfigTest::checkConfigTest_False3()
{	
	TestResult result;
	result.setMessage("MultitauConfig::checkConfig(false3)");

	string config_path = "tmultitau.config";	
	this->config->readConfigFile((*this).getInputDir() + config_path);
    this->config->setTauScale("other");  
	
	vector<string> output = this->config->checkConfig();
	vector<string> expected = {"TAU_SCALE"};
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
