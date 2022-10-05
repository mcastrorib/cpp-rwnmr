#ifndef CONFIGFILES_TEST_H_
#define CONFIGFILES_TEST_H_

#include "Includes.h"
#include "rwnmr_configTest.h"
#include "uct_configTest.h"
#include "cpmg_configTest.h"
#include "pfgse_configTest.h"
#include "multitau_configTest.h"

class ConfigFilesTest
{
public:
	ConfigFilesTest(string proot) : projectRoot(proot) 
	{
		(*this).setInputDir(proot + "/input/testing/");
		(*this).run();
	}

	ConfigFilesTest(const ConfigFilesTest& otherTest)
	{
		this->projectRoot = otherTest.projectRoot;
		this->inputDir = otherTest.inputDir;
		this->sucessResult = otherTest.sucessResult;
	}

	void run();
	void TestRWNMRConfigFile();
	void TestUCTConfigFile();
	void TestPFGSEConfigFile();
	void TestMULTITAUConfigFile();

	void setProjectRoot(string proot){ this->projectRoot = proot; }
	string getProjectRoot(){ return this->projectRoot; }
	void setSuccessResult(bool result){ this->sucessResult = result; }
	bool getSuccessResult(){ return this->sucessResult; }
	void setInputDir(string _path){ this->inputDir = _path; }
	string getInputDir(){ return this->inputDir; }

private:
	string projectRoot;
	string inputDir;
	bool sucessResult;

};

#endif