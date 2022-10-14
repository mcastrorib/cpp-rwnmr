#ifndef ARGSPARSER_TEST_H_
#define ARGSPARSER_TEST_H_

#include "Includes.h"

class ArgsParserTest : public TestSuite
{
public:
	typedef TestResult (ArgsParserTest::*mptr)();

private:
	vector<mptr> testCases;
	ArgsParser *parser;

public:
	ArgsParserTest(string proot);
	ArgsParserTest(const ArgsParserTest& otherTest) : TestSuite(otherTest.projectRoot)
	{
		this->testCases = otherTest.testCases;
		this->parser = otherTest.parser;
	}

	virtual ~ArgsParserTest(){
		if(this->parser != NULL){
			delete this->parser;
			this->parser = NULL;
		}
	}

	void beforeEach()
	{
		if(this->parser != NULL) delete this->parser;
		this->parser = NULL;		
	}

	void afterEach()
	{
		if(this->parser != NULL) delete this->parser;
		this->parser = NULL;
	}

	virtual vector<TestResult> run();

	TestResult testNoArgsCommands();
	TestResult testNoArgsPaths();
	TestResult testArgsCPMG_Default();
	TestResult testArgsCPMG_Complete();
	TestResult testArgsPFGSE_Default();
	TestResult testArgsPFGSE_Complete();
	TestResult testArgsMultiTau_Default();
	TestResult testArgsMultiTau_Complete1();
	TestResult testArgsMultiTau_Complete2();
	TestResult testArgsMultiTau_Complete3();
	TestResult testUnknownArgs_1();
	TestResult testUnknownArgs_2();
	TestResult testMissingArgs_1();
	TestResult testMissingArgs_2();
	TestResult testMissingArgs_3();
	TestResult testMissingArgs_4();
	TestResult testMissingArgs_5();
	TestResult testMissingArgs_6();
	TestResult testMissingArgs_7();
	TestResult testAllKnownArgs_1();
	TestResult testAllKnownArgs_2();
	TestResult testAllKnownArgs_3();
	TestResult testAllKnownArgs_4();
	TestResult testAllKnownArgs_5();
	TestResult testAllKnownArgs_6();	

	void setTestCases(vector<mptr> _tc){ this->testCases = _tc; }
	void setTestCases(mptr _t, int i){ this->testCases[i] = _t; }
	void addTest(mptr _t){ this->testCases.push_back(_t); }
	void clearTestCases(){ this->testCases.clear(); }
	void setParser(ArgsParser *_conf){ this->parser = _conf; }

	vector<mptr> getTestCases(){ return this->testCases; }
	mptr getTestCase(int i){ return this->testCases[i]; }
	ArgsParser *getParser(){ return this->parser; }
};

#endif