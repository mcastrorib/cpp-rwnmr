#include "ArgsParserTest.h"

ArgsParserTest::ArgsParserTest(string proot) : TestSuite(proot), parser(NULL)
{
	// add test cases to suite vector
	(*this).addTest(&ArgsParserTest::testNoArgsCommands);
	(*this).addTest(&ArgsParserTest::testNoArgsPaths);
	(*this).addTest(&ArgsParserTest::testArgsCPMG_Default);
	(*this).addTest(&ArgsParserTest::testArgsCPMG_Complete);
	(*this).addTest(&ArgsParserTest::testArgsPFGSE_Default);
	(*this).addTest(&ArgsParserTest::testArgsPFGSE_Complete);
	(*this).addTest(&ArgsParserTest::testArgsMultiTau_Default);
	(*this).addTest(&ArgsParserTest::testArgsMultiTau_Complete1);
	(*this).addTest(&ArgsParserTest::testArgsMultiTau_Complete2);
	(*this).addTest(&ArgsParserTest::testArgsMultiTau_Complete3);
	(*this).addTest(&ArgsParserTest::testUnknownArgs_1);
	(*this).addTest(&ArgsParserTest::testUnknownArgs_2);	
	(*this).addTest(&ArgsParserTest::testMissingArgs_1);	
	(*this).addTest(&ArgsParserTest::testMissingArgs_2);	
	(*this).addTest(&ArgsParserTest::testMissingArgs_3);	
	(*this).addTest(&ArgsParserTest::testMissingArgs_4);	
	(*this).addTest(&ArgsParserTest::testMissingArgs_5);	
	(*this).addTest(&ArgsParserTest::testMissingArgs_6);	
	(*this).addTest(&ArgsParserTest::testMissingArgs_7);
	// (*this).addTest(&ArgsParserTest::testAllKnownArgs);
	
}

vector<TestResult> ArgsParserTest::run()
{
	vector<TestResult> results;
	
	for(int t = 0; t < this->testCases.size(); t++)
	{
		(*this).beforeEach();
		results.push_back((this->*testCases[t])());
		(*this).afterEach();
	}
	
	return results;
}

TestResult ArgsParserTest::testNoArgsCommands()
{
	TestResult result("ArgsParser('rwnmr') commands");
	
	int argc = 1;
	char *argv[] = {(char*)"rwnmr", NULL};
	this->parser = new ArgsParser(argc, argv);
	
	vector<string> expectedCommands = {"rwnmr", "uct"};
	result.setSuccess(Assert::assertVectorEquals(this->parser->getCommands(), expectedCommands));
	return result;
}

TestResult ArgsParserTest::testNoArgsPaths()
{
	TestResult result("ArgsParser('rwnmr') paths");

	int argc = 1;
	char *argv[] = {(char*)"rwnmr", NULL};
	this->parser = new ArgsParser(argc, argv);
	vector<string> expectedPaths = {"default", "default"};

	result.setSuccess(Assert::assertVectorEquals(this->parser->getPaths(), expectedPaths));
	return result;
}

TestResult ArgsParserTest::testArgsCPMG_Default()
{
	TestResult result("ArgsParser('rwnmr cpmg')");

	// Create default args
	int argc = 2;
	char *argv[] = {(char*)"rwnmr", (char*)"cpmg", NULL};
	this->parser = new ArgsParser(argc, argv);
	vector<string> out = (*this).concatenateVectors(this->parser->getCommands(), this->parser->getPaths());
	vector<string> expected = {"rwnmr", "uct", "cpmg", "default", "default", "default"};

	result.setSuccess(Assert::assertVectorEquals(out, expected));
	return result;	
}

TestResult ArgsParserTest::testArgsCPMG_Complete()
{
	TestResult result("ArgsParser('rwnmr cpmg + (...)')");

	// Create complete args
	int argc = 8;
	char *argv[] = {(char*)"rwnmr", (char*)"cpmg", (char*)"-config", (char*)"cpmg_path", (char*)"-rwconfig", (char*)"rw_path", (char*)"-uctconfig", (char*)"uct_path", NULL};
	this->parser = new ArgsParser(argc, argv);
	vector<string> out = (*this).concatenateVectors(this->parser->getCommands(), this->parser->getPaths());
	vector<string> expected = {"rwnmr", "uct", "cpmg", "rw_path", "uct_path", "cpmg_path"};
	
	result.setSuccess(Assert::assertVectorEquals(out, expected));
	return result;	
}

TestResult ArgsParserTest::testArgsPFGSE_Default()
{
	TestResult result("ArgsParser('rwnmr pfgse')");
	
	// Create default args
	int argc = 2;
	char *argv[] = {(char*)"rwnmr", (char*)"pfgse", NULL};
	this->parser = new ArgsParser(argc, argv);
	vector<string> out = (*this).concatenateVectors(this->parser->getCommands(), this->parser->getPaths());
	vector<string> expected = {"rwnmr", "uct", "pfgse", "default", "default", "default"};
	
	result.setSuccess(Assert::assertVectorEquals(out, expected));
	return result;	
}

TestResult ArgsParserTest::testArgsPFGSE_Complete()
{
	TestResult result("ArgsParser('rwnmr pfgse + (...)')");
	
	// Create complete args
	int argc = 8;
	char *argv[] = {(char*)"rwnmr", (char*)"pfgse", (char*)"-config", (char*)"pfgse_path", (char*)"-rwconfig", (char*)"rw_path", (char*)"-uctconfig", (char*)"uct_path", NULL};
	this->parser = new ArgsParser(argc, argv);
	vector<string> out = (*this).concatenateVectors(this->parser->getCommands(), this->parser->getPaths());
	vector<string> expected = {"rwnmr", "uct", "pfgse", "rw_path", "uct_path", "pfgse_path"};
	
	result.setSuccess(Assert::assertVectorEquals(out, expected));
	return result;	
}

TestResult ArgsParserTest::testArgsMultiTau_Default()
{
	TestResult result("ArgsParser('rwnmr multitau')");

	// Create default args
	int argc = 2;
	char *argv[] = {(char*)"rwnmr", 
					 (char*)"multitau", 
					 NULL};
	this->parser = new ArgsParser(argc, argv);
	vector<string> out = (*this).concatenateVectors(this->parser->getCommands(), this->parser->getPaths());
	vector<string> expected = {"rwnmr", "uct", "multitau", "mtoff", "default", "default", "default", "default"};
	result.setSuccess(Assert::assertVectorEquals(out, expected));
	return result;	
}

TestResult ArgsParserTest::testArgsMultiTau_Complete1()
{
	TestResult result("ArgsParser('rwnmr multitau (+1)')");
	
	// Create MT1 args
	int argc = 8;
	char *argv[] = {(char*)"rwnmr", 
					 (char*)"multitau", 
					 (char*)"-mtconfig", 
					 (char*)"mt_path", 
					 (char*)"-rwconfig", 
					 (char*)"rw_path", 
					 (char*)"-uctconfig", 
					 (char*)"uct_path", 
					 NULL};
	this->parser = new ArgsParser(argc, argv);
	vector<string> out = (*this).concatenateVectors(this->parser->getCommands(), this->parser->getPaths());
	vector<string> expected = {"rwnmr", "uct", "multitau", "mtoff", "rw_path", "uct_path", "mt_path", "default"};
	result.setSuccess(Assert::assertVectorEquals(out, expected));
	return result;	
}

TestResult ArgsParserTest::testArgsMultiTau_Complete2()
{
	TestResult result("ArgsParser('rwnmr multitau (+2)')");
	
	// Create MT2 args
	int argc = 8;
	char *argv[] = {(char*)"rwnmr", 
					 (char*)"multitau", 
					 (char*)"-cpmgconfig", 
					 (char*)"cpmg_path", 
					 (char*)"-rwconfig", 
					 (char*)"rw_path", 
					 (char*)"-uctconfig", 
					 (char*)"uct_path", NULL};
	this->parser = new ArgsParser(argc, argv);
	vector<string> out = (*this).concatenateVectors(this->parser->getCommands(), this->parser->getPaths());
	vector<string> expected = {"rwnmr", "uct", "multitau", "mtoff", "rw_path", "uct_path", "default", "cpmg_path"};
	result.setSuccess(Assert::assertVectorEquals(out, expected));
	return result;	
}	
	
TestResult ArgsParserTest::testArgsMultiTau_Complete3()
{
	TestResult result("ArgsParser('rwnmr multitau (+3)')");

	// Create MT3 args
	int argc = 9;
	char *argv[] = {(char*)"rwnmr", 
					 (char*)"multitau", 
					 (char*)"-config", 
					 (char*)"mt_path", 
					 (char*)"cpmg_path",
					 (char*)"-rwconfig",
					 (char*)"rw_path", 
					 (char*)"-uctconfig",
					 (char*)"uct_path", 
					 NULL};
	this->parser = new ArgsParser(argc, argv);
	vector<string> out = (*this).concatenateVectors(this->parser->getCommands(), this->parser->getPaths());
	vector<string> expected = {"rwnmr", "uct", "multitau", "mtoff", "rw_path", "uct_path", "mt_path", "cpmg_path"};
	result.setSuccess(Assert::assertVectorEquals(out, expected));
	return result;	
}

TestResult ArgsParserTest::testUnknownArgs_1()
{
	TestResult result("ArgsParser('rwnmr unknown(1)')");

	// Create parser object
	int argc = 2;
	char *argv[] = {(char*)"rwnmr", (char*)"unknown", NULL};
	this->parser = new ArgsParser(argc, argv);
	vector<string> out = (*this).concatenateVectors(this->parser->getCommands(), this->parser->getPaths());
	vector<string> expected = {"rwnmr", "uct", "default", "default"};
	result.setSuccess(Assert::assertVectorEquals(out, expected));
	return result;	
}

TestResult ArgsParserTest::testUnknownArgs_2()
{
	TestResult result("ArgsParser('rwnmr unknown(2)')");

	// Create parser object with known cmd and unknown cmd
	int argc = 3;
	char *argv[] = {(char*)"rwnmr", (char*)"unknown", (char*)"cpmg", NULL};
	this->parser = new ArgsParser(argc, argv);
	vector<string> out = (*this).concatenateVectors(this->parser->getCommands(), this->parser->getPaths());
	vector<string> expected = {"rwnmr", "uct", "cpmg", "default", "default", "default"};
	result.setSuccess(Assert::assertVectorEquals(out, expected));
	return result;	
}

TestResult ArgsParserTest::testMissingArgs_1()
{
	TestResult result("ArgsParser('rwnmr -rwconfig (missing-path)')");

	// Create missing rwconfig parser object
	int argc = 2;
	char *argv[] = {(char*)"rwnmr", (char*)"-rwconfig", NULL};
	this->parser = new ArgsParser(argc, argv);
	vector<string> out = (*this).concatenateVectors(this->parser->getCommands(), this->parser->getPaths());
	vector<string> expected = {"rwnmr", "uct", "default", "default"};
	result.setSuccess(Assert::assertVectorEquals(out, expected));
	return result;	
}

TestResult ArgsParserTest::testMissingArgs_2()
{
	TestResult result("ArgsParser('rwnmr -uctconfig (missing-path)')");

	// Create missing uctconfig parser object
	int argc = 2;
	char *argv[] = {(char*)"rwnmr", (char*)"-uctconfig", NULL};
	this->parser = new ArgsParser(argc, argv);
	vector<string> out = (*this).concatenateVectors(this->parser->getCommands(), this->parser->getPaths());
	vector<string> expected = {"rwnmr", "uct", "default", "default"};
	result.setSuccess(Assert::assertVectorEquals(out, expected));
	return result;	
}

TestResult ArgsParserTest::testMissingArgs_3()
{
	TestResult result("ArgsParser('rwnmr cpmg -config (missing-path)')");

	// Create missing cpmgconfig parser object
	int argc = 3;
	char *argv[] = {(char*)"rwnmr", (char*)"cpmg", (char*)"-config", NULL};
	this->parser = new ArgsParser(argc, argv);
	vector<string> out = (*this).concatenateVectors(this->parser->getCommands(), this->parser->getPaths());
	vector<string> expected = {"rwnmr", "uct", "cpmg", "default", "default", "default"};
	result.setSuccess(Assert::assertVectorEquals(out, expected));
	return result;	
}	


TestResult ArgsParserTest::testMissingArgs_4()
{
	TestResult result("ArgsParser('rwnmr pfgse -config (missing-path)')");

	// Create missing pfgseconfig parser object
	int argc = 3;
	char *argv[] = {(char*)"rwnmr", (char*)"pfgse", (char*)"-config", NULL};
	this->parser = new ArgsParser(argc, argv);
	vector<string> out = (*this).concatenateVectors(this->parser->getCommands(), this->parser->getPaths());
	vector<string> expected = {"rwnmr", "uct", "pfgse", "default", "default", "default"};
	result.setSuccess(Assert::assertVectorEquals(out, expected));
	return result;	
}	

TestResult ArgsParserTest::testMissingArgs_5()
{
	TestResult result("ArgsParser('rwnmr multitau -config (missing-path)')");

	// Create missing multitauconfig parser object
	int argc = 3;
	char *argv[] = {(char*)"rwnmr", (char*)"multitau", (char*)"-config", NULL};
	this->parser = new ArgsParser(argc, argv);
	vector<string> out = (*this).concatenateVectors(this->parser->getCommands(), this->parser->getPaths());
	vector<string> expected = {"rwnmr", "uct", "multitau", "mtoff","default", "default", "default", "default"};
	result.setSuccess(Assert::assertVectorEquals(out, expected));
	return result;	
}

TestResult ArgsParserTest::testMissingArgs_6()
{
	TestResult result("ArgsParser('rwnmr multitau -mtconfig (missing-path)')");

	// Create missing multitauconfig parser object	
	int argc = 3;
	char *argv[] = {(char*)"rwnmr", (char*)"multitau", (char*)"-mtconfig", NULL};
	this->parser = new ArgsParser(argc, argv);
	vector<string> out = (*this).concatenateVectors(this->parser->getCommands(), this->parser->getPaths());
	vector<string> expected = {"rwnmr", "uct", "multitau", "mtoff","default", "default", "default", "default"};
	result.setSuccess(Assert::assertVectorEquals(out, expected));
	return result;	
}

TestResult ArgsParserTest::testMissingArgs_7()
{
	TestResult result("ArgsParser('rwnmr multitau -cpmgconfig (missing-path)')");

	// Create missing multitauconfig parser object
	int argc = 3;
	char *argv[] = {(char*)"rwnmr", (char*)"multitau", (char*)"-cpmgconfig", NULL};
	this->parser = new ArgsParser(argc, argv);
	vector<string> out = (*this).concatenateVectors(this->parser->getCommands(), this->parser->getPaths());
	vector<string> expected = {"rwnmr", "uct", "multitau", "mtoff","default", "default", "default", "default"};
	result.setSuccess(Assert::assertVectorEquals(out, expected));
	return result;	
}

TestResult ArgsParserTest::testAllKnownArgs_1()
{
	TestResult result("ArgsParser(rwnmr cpmg pfgse multitau)");

	/*
		Test all methods
	*/
	int argc = 4;
	char *argv[] = {(char*)"rwnmr", (char*)"cpmg", (char*)"pfgse", (char*)"multitau", NULL};
	this->parser = new ArgsParser(argc, argv);
	vector<string> out = (*this).concatenateVectors(this->parser->getCommands(), this->parser->getPaths());
	vector<string> expected = {"rwnmr", "uct", "cpmg", "pfgse", "multitau", "mtoff", "default", "default", "default", "default", "default", "default"};
	result.setSuccess(Assert::assertVectorEquals(out, expected));
	return result;	
}


TestResult ArgsParserTest::testAllKnownArgs_2()
{
	TestResult result("ArgsParser(rwnmr multitau cpmg pfgse)");

	/*
		Test all methods, multitau in the middle
	*/
	int argc = 4;
	char *argv[] = {(char*)"rwnmr", (char*)"multitau", (char*)"cpmg", (char*)"pfgse", NULL};
	this->parser = new ArgsParser(argc, argv);
	vector<string> out = (*this).concatenateVectors(this->parser->getCommands(), this->parser->getPaths());
	vector<string> expected = {"rwnmr", "uct", "multitau", "mtoff", "cpmg", "pfgse", "default", "default", "default", "default", "default", "default"};
	result.setSuccess(Assert::assertVectorEquals(out, expected));
	return result;	
}

TestResult ArgsParserTest::testAllKnownArgs_3()
{
	TestResult result("ArgsParser(rwnmr {+all args}[1])");

	/*
		Test all methods + paths
	*/
	int argc = 15;
	char *argv[] =  {(char*)"rwnmr", 
					 (char*)"-rwconfig", (char*)"rw_path", 
					 (char*)"-uctconfig", (char*)"uct_path", 
					 (char*)"cpmg", (char*)"-config", (char*)"cpmg_path",
					 (char*)"pfgse", (char*)"-config", (char*)"pfgse_path",
					 (char*)"multitau", (char*)"-config", (char*)"mt_path", (char*)"mt_cpmg_path", NULL};
	this->parser = new ArgsParser(argc, argv);
	vector<string> out = (*this).concatenateVectors(this->parser->getCommands(), this->parser->getPaths());
	vector<string> expected = {"rwnmr", "uct", "cpmg", "pfgse", "multitau", "mtoff", "rw_path", "uct_path", "cpmg_path", "pfgse_path", "mt_path", "mt_cpmg_path"};
	result.setSuccess(Assert::assertVectorEquals(out, expected));
	return result;	
}

TestResult ArgsParserTest::testAllKnownArgs_4()
{
	TestResult result("ArgsParser(rwnmr {+all args}[2])");

	/*
		Test all methods + paths (multitau in the middle)
	*/
	int argc = 15;
	char *argv[] = {(char*)"rwnmr", 
					 (char*)"-rwconfig", (char*)"rw_path", 
					 (char*)"-uctconfig", (char*)"uct_path", 
					 (char*)"cpmg", (char*)"-config", (char*)"cpmg_path",
					 (char*)"multitau", (char*)"-config", (char*)"mt_path", (char*)"mt_cpmg_path",
					 (char*)"pfgse", (char*)"-config", (char*)"pfgse_path", NULL};
	this->parser = new ArgsParser(argc, argv);
	vector<string> out = (*this).concatenateVectors(this->parser->getCommands(), this->parser->getPaths());
	vector<string> expected = {"rwnmr", "uct", "cpmg", "multitau", "mtoff", "pfgse", "rw_path", "uct_path", "cpmg_path", "mt_path", "mt_cpmg_path", "pfgse_path"};
	result.setSuccess(Assert::assertVectorEquals(out, expected));
	return result;	
}

TestResult ArgsParserTest::testAllKnownArgs_5()
{
	TestResult result("ArgsParser(rwnmr {+all args}[3])");

	/*
		Test all methods + paths (multitau in the middle)
	*/
	int argc = 14;
	char *argv[] = {(char*)"rwnmr", 
					 (char*)"-rwconfig", (char*)"rw_path", 
					 (char*)"-uctconfig", (char*)"uct_path", 
					 (char*)"multitau", (char*)"-cpmgconfig", (char*)"mt_cpmg_path",
					 (char*)"cpmg", (char*)"-config", (char*)"cpmg_path",
					 (char*)"pfgse", (char*)"-config", (char*)"pfgse_path", NULL};
	this->parser = new ArgsParser(argc, argv);
	vector<string> out = (*this).concatenateVectors(this->parser->getCommands(), this->parser->getPaths());
	vector<string> expected = {"rwnmr", "uct", "multitau", "mtoff", "cpmg", "pfgse", "rw_path", "uct_path", "default", "mt_cpmg_path", "cpmg_path", "pfgse_path"};
	result.setSuccess(Assert::assertVectorEquals(out, expected));
	return result;	
}

TestResult ArgsParserTest::testAllKnownArgs_6()
{
	TestResult result("ArgsParser(rwnmr {+all args}[4])");

	/*
		Test all methods + paths (multitau in the middle)
	*/
	int argc = 14;
	char *argv[] = {(char*)"rwnmr", 
					 (char*)"-rwconfig", (char*)"rw_path", 
					 (char*)"-uctconfig", (char*)"uct_path", 
					 (char*)"cpmg", (char*)"-config", (char*)"cpmg_path",
					 (char*)"multitau", (char*)"-mtconfig", (char*)"mt_path",
					 (char*)"pfgse", (char*)"-config", (char*)"pfgse_path", NULL};
	this->parser = new ArgsParser(argc, argv);
	vector<string> out = (*this).concatenateVectors(this->parser->getCommands(), this->parser->getPaths());
	vector<string> expected = {"rwnmr", "uct", "cpmg", "multitau", "mtoff", "pfgse", "rw_path", "uct_path", "cpmg_path", "mt_path", "default", "pfgse_path"};
	result.setSuccess(Assert::assertVectorEquals(out, expected));
	return result;	
}
