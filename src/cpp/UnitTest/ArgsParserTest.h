#ifndef ARGSPARSER_TEST_H_
#define ARGSPARSER_TEST_H_

#include <iostream>
#include <vector>
#include <string>
#include <cstring>

#include "../Utils/ArgsParser.h"

using namespace std;

class ArgsParserTest
{
public:
	string project_root;
	bool sucessResult;

	ArgsParserTest(string proot) : project_root(proot) 
	{
		(*this).run();
	}

	ArgsParserTest(const ArgsParserTest& otherTest)
	{
		this->project_root = otherTest.project_root;
		this->sucessResult = otherTest.sucessResult;
	}
	void run();

private:
	void testNoArgs();
	void testArgsCPMG();
	void testArgsPFGSE();
	void testArgsMultiTau();
	void testUnknownArgs();
	void testAllKnownArgs();
	void testDisorderedArgs();
};

#endif