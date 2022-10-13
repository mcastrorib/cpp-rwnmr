#include "uct_configTest.h"

uct_configTest::uct_configTest(string proot) : TestSuite(proot), config(NULL)
{
	// add test cases to suite vector
	(*this).addTest(&uct_configTest::readConfigFileTest);
	(*this).addTest(&uct_configTest::checkConfigTest_True);
    (*this).addTest(&uct_configTest::checkConfigTest_False);
	(*this).addTest(&uct_configTest::readDirPathTest_AbsolutePath);
	(*this).addTest(&uct_configTest::readDirPathTest_RelativePath);	
}

vector<TestResult> uct_configTest::run()
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

TestResult uct_configTest::readConfigFileTest()
{	
	TestResult result;
	result.setMessage("uct_config::readConfigFile()");

	string config_path = "tuct.config";	
	this->config->readConfigFile((*this).getInputDir() + config_path);	
	if(Assert::assertEquals(this->config->getDirPath(), (string)"./input/images/sphere_2.5voxels/") and
	   Assert::assertEquals(this->config->getFilename(), (string)"sphere_r=2.5voxels_") and
	   Assert::assertEquals(this->config->getFirstIdx(), (uint) 0) and
	   Assert::assertEquals(this->config->getDigits(), (uint) 1) and
	   Assert::assertEquals(this->config->getExtension(), (string)".png") and
	   Assert::assertEquals(this->config->getSlices(), (uint) 7) and
	   Assert::assertEquals(this->config->getFirstIdx(), (uint) 0) and
	   Assert::assertEquals(this->config->getResolution(), (double) 1.0) and
	   Assert::assertEquals(this->config->getVoxelDivision(), (uint) 0)
	) result.setSuccess(true);
	else result.setSuccess(false); 

	return result;
}

TestResult uct_configTest::checkConfigTest_True()
{	
	TestResult result;
	result.setMessage("uct_config::checkConfig(true)");

	string config_path = "tuct.config";	
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

TestResult uct_configTest::checkConfigTest_False()
{	
	TestResult result;
	result.setMessage("uct_config::checkConfig(false)");

	string config_path = "tuct.config";	
	this->config->readConfigFile((*this).getInputDir() + config_path);
	this->config->setExtension("other");
	this->config->setResolution(-0.1);
	
	vector<string> output = this->config->checkConfig();
	vector<string> expected = {"EXTENSION", "RESOLUTION"};
	if(Assert::assertVectorEquals(output, expected) and 
	   Assert::assertFalse(this->config->getReady())
	) result.setSuccess(true);
	else {
        result.setSuccess(false);
        for (int i = 0; i < output.size(); i++) cout << output[i] << endl;
    }
	return result;
}

TestResult uct_configTest::readDirPathTest_AbsolutePath()
{
	TestResult result("uct_config::readDirPath(absolute)");
	string token = "/this/is/my/test/absolute-path";
	this->config->readDirPath(token);
	result.setSuccess(Assert::assertEquals(this->config->getDirPath(), token));
	return result;
}

TestResult uct_configTest::readDirPathTest_RelativePath()
{
	TestResult result("uct_config::readDirPath(relative)");
	string projRoot = "/this/is/";
	this->config->setProjectRoot(projRoot);
	string token = "./my/test/relative-path";
	this->config->readDirPath(token);
	string expected = projRoot + token;
	result.setSuccess(Assert::assertEquals(this->config->getDirPath(), expected));
	return result;
}