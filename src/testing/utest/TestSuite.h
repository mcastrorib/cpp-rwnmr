#ifndef TESTSUITE_H_
#define TESTSUITE_H_

#include "Includes.h"
#include "TestResult.h"

class TestSuite
{
public:    
	string projectRoot;
	string inputDir;

    TestSuite(string proot):projectRoot(proot)
    { 
        (*this).setInputDir(proot + "/input/testing/");
    }
    TestSuite(const TestSuite& otherTest)
	{
		this->projectRoot = otherTest.projectRoot;
		this->inputDir = otherTest.inputDir;
	}
    virtual ~TestSuite(){}    
    virtual vector<TestResult> run() = 0;    

	void setProjectRoot(string proot){ this->projectRoot = proot; }
	void setInputDir(string _path){ this->inputDir = _path; }
    string getProjectRoot(){ return this->projectRoot; }
	string getInputDir(){ return this->inputDir; }
};

#endif