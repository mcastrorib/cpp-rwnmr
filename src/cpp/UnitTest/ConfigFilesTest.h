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
	string project_root;
	string inputDir;
	bool sucessResult;

	ConfigFilesTest(string proot) : project_root(proot) 
	{
		(*this).setInputDir(proot + "/input/testing/");
		(*this).run();
	}

	ConfigFilesTest(const ConfigFilesTest& otherTest)
	{
		this->project_root = otherTest.project_root;
		this->sucessResult = otherTest.sucessResult;
	}

	void run();
	string getProjectRoot(){ return this->project_root; }
	void setInputDir(string _path){ this->inputDir = _path; }
	string getInputDir(){ return this->inputDir; }

private:
	void TestRWNMRConfigFile();
	void TestUCTConfigFile();
	void TestCPMGConfigFile();
	void TestPFGSEConfigFile();
	void TestMULTITAUConfigFile();
};

#endif