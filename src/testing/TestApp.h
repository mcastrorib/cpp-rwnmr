#ifndef TESTAPP_H_
#define TESTAPP_H_

// include C++ standard libraries
#include <iostream>
#include <sstream>
#include <cstdint>
#include <vector>
#include <string>

#include "utest/Utest.h"
#include "utils/UtilsTest.h"
#include "math/MathTest.h"
#include "configFiles/ConfigFilesTest.h"
#include "framework/FrameworkTest.h"

using namespace std;

class TestApp
{
private:
	TestSuite *test;
	vector<TestResult> results;
	string projectRoot;
	bool sucessResult;

public:
	TestApp(string proot);
	TestApp(const TestApp& otherTest)
	{
		this->test = otherTest.test;
		this->projectRoot = otherTest.projectRoot;
		this->sucessResult = otherTest.sucessResult;
	}
	virtual ~TestApp()
	{ 
		if(this->test != NULL)
		{
			delete this->test;
			this->test =NULL;
		}
	} 
	
	void noArgsMessage();
	void help();
	void exec(string tag);
	void select(string tag);
	void run();
	void checkResults(string tag="");

	void setTest(TestSuite *t){ this->test = t; }
	TestSuite *getTest(){ return this->test; }
	void setResults(vector<TestResult> r){ this->results = r; }
	vector<TestResult> getResults(){ return this->results; }
	TestResult getResult(int i){ return this->results[i]; }
	void setProjectRoot(string proot){ this->projectRoot = proot; }
	string getProjectRoot(){ return this->projectRoot; }
	void setSuccessResult(bool result){ this->sucessResult = result; }
	bool getSuccessResult(){ return this->sucessResult; }

};

#endif