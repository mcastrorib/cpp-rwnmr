#ifndef CONFIGFILES_TEST_H_
#define CONFIGFILES_TEST_H_

#include <iostream>
#include <vector>
#include <string>
#include <stdint.h>

#include "../ConfigFiles/configFiles_defs.h"
#include "../ConfigFiles/rwnmr_config.h"
#include "../ConfigFiles/uct_config.h"
#include "../ConfigFiles/cpmg_config.h"
#include "../ConfigFiles/pfgse_config.h"
#include "../ConfigFiles/multitau_config.h"
#include "../ConfigFiles/ga_config.h"
#include "Assert.h"

using namespace std;

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
	void TestCPMGConfigFile();
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