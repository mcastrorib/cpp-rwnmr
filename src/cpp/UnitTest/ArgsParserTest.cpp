#include "../Utils/ArgsParser.h"
#include "ArgsParserTest.h"

void ArgsParserTest::run()
{
	(*this).testNoArgs();
	(*this).testArgsCPMG();
	(*this).testArgsPFGSE();
	(*this).testArgsMultiTau();
	(*this).testUnknownArgs();
	(*this).testAllKnownArgs();
	(*this).testDisorderedArgs();	
}

void ArgsParserTest::testNoArgs()
{
	cout << "executing no args test...";
	int argc = 1;
	char *argv[] = {(char*)"rwnmr", NULL};
	ArgsParser *parser = new ArgsParser(argc, argv);
	if(parser->commands.size() != 2)
	{
		cout << endl << "Error: commands size is incorrect" << endl;
		exit(1);
	} 

	if(parser->paths.size() != 2)
	{
		cout << endl << "Error: paths size is incorrect" << endl;
		exit(1);
	}

	if(parser->commands[0] != "rwnmr" or  parser->commands[1] != "uct")
	{
		cout << endl << "Error: default commands are incorrect" << endl;
		exit(1);
	}

	if(parser->paths[0] != "default" or  parser->paths[1] != "default")
	{
		cout << endl << "Error: default paths are incorrect" << endl;
		exit(1);
	}
	cout << "Ok." << endl;

	delete parser; parser = NULL;
	return;
}

void ArgsParserTest::testArgsCPMG()
{
	cout << "executing cpmg args test" << endl;
}

void ArgsParserTest::testArgsPFGSE()
{
	cout << "executing pfgse args test" << endl;
}

void ArgsParserTest::testArgsMultiTau()
{
	cout << "executing multitau args test" << endl;
}

void ArgsParserTest::testUnknownArgs()
{
	cout << "executing unknown args test" << endl;
}

void ArgsParserTest::testAllKnownArgs()
{
	cout << "executing all args test" << endl;
}

void ArgsParserTest::testDisorderedArgs()
{
	cout << "executing disordered args test" << endl;
}