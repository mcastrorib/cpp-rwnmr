#ifndef UNITTEST_H_
#define UNITTEST_H_

#include <iostream>
#include <string>
#include <vector>

#include "../Utils/ArgsParser.h"
#include "../ConfigFiles/configFiles_defs.h"
#include "../NMR_Simulation/NMR_Simulation.h"

// #include "UnitTest_includes.h"

using namespace std;

class UnitTest
{
public: 
	string project_root;
	bool sucessResult;

	UnitTest(string proot);
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