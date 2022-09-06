#ifndef UNIT_TEST_H_
#define UNIT_TEST_H_

#include <iostream>
#include <string>
#include "UnitTest_defs.h"

using namespace std;

class UnitTest
{
public: 
	string project_root;
	bool sucessResult;

	UnitTest(string proot) : project_root(proot), sucessResult(true)
	{}
	UnitTest(const UnitTest& otherTest)
	{
		this->project_root = otherTest.project_root;
		this->sucessResult = otherTest.sucessResult;
	}
	virtual ~UnitTest(){} 
	void runTest(string tag);
	void runAll();
	string getProjectRoot(){ return this->project_root; }
};

#endif