#include "UctConfigTest.h"

UctConfigTest::UctConfigTest(string proot) : TestSuite(proot), config(NULL)
{
	// add test cases to suite vector
	(*this).addTest(&UctConfigTest::readConfigFileTest);
	(*this).addTest(&UctConfigTest::checkConfigTest_True);
    (*this).addTest(&UctConfigTest::checkConfigTest_False);
	(*this).addTest(&UctConfigTest::readDirPathTest_AbsolutePath);
	(*this).addTest(&UctConfigTest::readDirPathTest_RelativePath);	
}

vector<TestResult> UctConfigTest::run()
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

TestResult UctConfigTest::readConfigFileTest()
{	
	TestResult result;
	result.setMessage("UctConfig::readConfigFile()");

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
	   Assert::assertEquals(this->config->getVoxelDivision(), (uint) 1) and
	   Assert::assertEquals(this->config->getPoreColor(), (uint8_t) 0) 
	) result.setSuccess(true);
	else result.setSuccess(false); 

	return result;
}

TestResult UctConfigTest::checkConfigTest_True()
{	
	TestResult result;
	result.setMessage("UctConfig::checkConfig(true)");

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

TestResult UctConfigTest::checkConfigTest_False()
{	
	TestResult result;
	result.setMessage("UctConfig::checkConfig(false)");

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

TestResult UctConfigTest::readDirPathTest_AbsolutePath()
{
	TestResult result("UctConfig::readDirPath(absolute)");
	string token = "/this/is/my/test/absolute-path";
	this->config->readDirPath(token);
	result.setSuccess(Assert::assertEquals(this->config->getDirPath(), token));
	return result;
}

TestResult UctConfigTest::readDirPathTest_RelativePath()
{
	TestResult result("UctConfig::readDirPath(relative)");
	string projRoot = "/this/is/";
	this->config->setProjectRoot(projRoot);
	string token = "./my/test/relative-path";
	this->config->readDirPath(token);
	string expected = projRoot + token;
	result.setSuccess(Assert::assertEquals(this->config->getDirPath(), expected));
	return result;
}