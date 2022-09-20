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
	ArgsParserTest(string proot) : projectRoot(proot) 
	{
		(*this).run();
	}

	ArgsParserTest(const ArgsParserTest& otherTest)
	{
		this->projectRoot = otherTest.projectRoot;
		this->sucessResult = otherTest.sucessResult;
	}
	void run();
	void checkParser(int _argc, char *_argv[], vector<string> _expectedCommands, vector<string> _expectedPaths, string _msg);
	void testNoArgs();
	void testArgsCPMG();
	void testArgsPFGSE();
	void testArgsMultiTau();
	void testUnknownArgs();
	void testMissingArgs();
	void testAllKnownArgs();

	void setProjectRoot(string proot){ this->projectRoot = proot; }
	string getProjectRoot(){ return this->projectRoot; }
	void setSuccessResult(bool result){ this->sucessResult = result; }
	bool getSuccessResult(){ return this->sucessResult; }

private:	
	string projectRoot;
	bool sucessResult;
};

#endif