#include "../Utils/ArgsParser.h"
#include "ArgsParserTest.h"

void ArgsParserTest::run()
{
	(*this).testNoArgs();
	(*this).testArgsCPMG();
	(*this).testArgsPFGSE();
	(*this).testArgsMultiTau();
	(*this).testUnknownArgs();
	(*this).testMissingArgs();
	(*this).testAllKnownArgs();
}

void ArgsParserTest::checkParser(int argc, char *argv[], vector<string> _expectedCommands, vector<string> _expectedPaths, string _msg)
{
	ArgsParser *parser = new ArgsParser(argc, argv);

	if(parser->getCommands().size() != _expectedCommands.size())
	{
		cout << endl << "Error(" << _msg << "): commands size is incorrect" << endl;
		exit(1);
	} 

	if(parser->getPaths().size() != _expectedPaths.size())
	{
		cout << endl << "Error(" << _msg << "): paths size is incorrect" << endl;
		exit(1);
	}

	for(int idx = 0; idx < parser->getCommands().size(); idx++)
	{
		if(parser->getCommand(idx) != _expectedCommands[idx])
		{
			cout << endl << "Error(" << _msg << "): command(" << idx << ") is incorrect" << endl;
			exit(1);		
		}
	}

	for(int idx = 0; idx < parser->getPaths().size(); idx++)
	{
		if(parser->getPath(idx) != _expectedPaths[idx])
		{
			cout << endl << "Error(" << _msg << "): path(" << idx << ") is incorrect" << endl;
			exit(1);		
		}
	} 

	delete parser; parser = NULL;
	return;
}

void ArgsParserTest::testNoArgs()
{
	cout << "executing no args test...";

	// Create default args
	int argc = 1;
	char *argv[] = {(char*)"rwnmr", NULL};
	vector<string> expectedCommands = {"rwnmr", "uct"};
	vector<string> expectedPaths = {"default", "default"};
	string message = "DEFAULT";
	(*this).checkParser(argc, argv, expectedCommands, expectedPaths, message);

	cout << "Ok." << endl; return;
}

void ArgsParserTest::testArgsCPMG()
{
	cout << "executing cpmg args test...";

	// Create default args
	int argc1 = 2;
	char *argv1[] = {(char*)"rwnmr", (char*)"cpmg", NULL};
	vector<string> xCommands1 = {"rwnmr", "uct", "cpmg"};
	vector<string> xPaths1 = {"default", "default", "default"};
	string msg1 = "CPMG-DEFAULT";
	(*this).checkParser(argc1, argv1, xCommands1, xPaths1, msg1);
	
	// Create complete args
	int argc2 = 8;
	char *argv2[] = {(char*)"rwnmr", (char*)"cpmg", (char*)"-config", (char*)"cpmg_path", (char*)"-rwconfig", (char*)"rw_path", (char*)"-uctconfig", (char*)"uct_path", NULL};
	vector<string> xCommands2 = {"rwnmr", "uct", "cpmg"};
	vector<string> xPaths2 = {"rw_path", "uct_path", "cpmg_path"};
	string msg2 = "CPMG-COMPLETE";
	(*this).checkParser(argc2, argv2, xCommands2, xPaths2, msg2);
	
	cout << "Ok." << endl;
	return;
}

void ArgsParserTest::testArgsPFGSE()
{
	cout << "executing pfgse args test...";

	// Create default args
	int argc1 = 2;
	char *argv1[] = {(char*)"rwnmr", (char*)"pfgse", NULL};
	vector<string> xCommands1 = {"rwnmr", "uct", "pfgse"};
	vector<string> xPaths1 = {"default", "default", "default"};
	string msg1 = "PFGSE-DEFAULT";
	(*this).checkParser(argc1, argv1, xCommands1, xPaths1, msg1);

	// Create complete args
	int argc2 = 8;
	char *argv2[] = {(char*)"rwnmr", (char*)"pfgse", (char*)"-config", (char*)"pfgse_path", (char*)"-rwconfig", (char*)"rw_path", (char*)"-uctconfig", (char*)"uct_path", NULL};
	vector<string> xCommands2 = {"rwnmr", "uct", "pfgse"};
	vector<string> xPaths2 = {"rw_path", "uct_path", "pfgse_path"};
	string msg2 = "PFGSE-COMPLETE";
	(*this).checkParser(argc2, argv2, xCommands2, xPaths2, msg2);
	
	cout << "Ok." << endl;
	return;
}

void ArgsParserTest::testArgsMultiTau()
{
	cout << "executing multitau args test...";

	// Create default args
	int argc1 = 2;
	char *argv1[] = {(char*)"rwnmr", 
					 (char*)"multitau", 
					 NULL};
	vector<string> xCommands1 = {"rwnmr", "uct", "multitau", "mtoff"};
	vector<string> xPaths1 = {"default", "default", "default", "default"};
	string msg1 = "MULTITAU-DEFAULT";
	(*this).checkParser(argc1, argv1, xCommands1, xPaths1, msg1);


	// Create MT1 args
	int argc2 = 8;
	char *argv2[] = {(char*)"rwnmr", 
					 (char*)"multitau", 
					 (char*)"-mtconfig", 
					 (char*)"mt_path", 
					 (char*)"-rwconfig", 
					 (char*)"rw_path", 
					 (char*)"-uctconfig", 
					 (char*)"uct_path", 
					 NULL};
	vector<string> xCommands2 = {"rwnmr", "uct", "multitau", "mtoff"};
	vector<string> xPaths2 = {"rw_path", "uct_path", "mt_path", "default"};
	string msg2 = "MULTITAU-CP1";
	(*this).checkParser(argc2, argv2, xCommands2, xPaths2, msg2);
	
	// Create MT2 args
	int argc3 = 8;
	char *argv3[] = {(char*)"rwnmr", 
					 (char*)"multitau", 
					 (char*)"-cpmgconfig", 
					 (char*)"cpmg_path", 
					 (char*)"-rwconfig", 
					 (char*)"rw_path", 
					 (char*)"-uctconfig", 
					 (char*)"uct_path", NULL};
	vector<string> xCommands3 = {"rwnmr", "uct", "multitau", "mtoff"};
	vector<string> xPaths3 = {"rw_path", "uct_path", "default", "cpmg_path"};
	string msg3 = "MULTITAU-CP2";
	(*this).checkParser(argc3, argv3, xCommands3, xPaths3, msg3);
	
	
	// Create MT3 args
	int argc4 = 9;
	char *argv4[] = {(char*)"rwnmr", 
					 (char*)"multitau", 
					 (char*)"-config", 
					 (char*)"mt_path", 
					 (char*)"cpmg_path",
					 (char*)"-rwconfig",
					 (char*)"rw_path", 
					 (char*)"-uctconfig",
					 (char*)"uct_path", 
					 NULL};
	vector<string> xCommands4 = {"rwnmr", "uct", "multitau", "mtoff"};
	vector<string> xPaths4 = {"rw_path", "uct_path", "mt_path", "cpmg_path"};
	string msg4 = "MULTITAU-CP3";
	(*this).checkParser(argc4, argv4, xCommands4, xPaths4, msg4);

	cout << "Ok." << endl;
	return;
}

void ArgsParserTest::testUnknownArgs()
{
	cout << "executing unknown args test...";
	// Create parser object
	int argc = 2;
	char *argv[] = {(char*)"rwnmr", (char*)"unknown", NULL};
	vector<string> xCommands = {"rwnmr", "uct"};
	vector<string> xPaths = {"default", "default"};
	string msg = "UNKONWN-ARGS1";
	(*this).checkParser(argc, argv, xCommands, xPaths, msg);

	// Create parser object with known cmd and unknown cmd
	int argc2 = 3;
	char *argv2[] = {(char*)"rwnmr", (char*)"unknown", (char*)"cpmg", NULL};
	vector<string> xCommands2 = {"rwnmr", "uct", "cpmg"};
	vector<string> xPaths2 = {"default", "default", "default"};
	string msg2 = "UNKONWN-ARGS2";
	(*this).checkParser(argc2, argv2, xCommands2, xPaths2, msg2);

	cout << "Ok." << endl;
	return;
}

void ArgsParserTest::testMissingArgs()
{
	cout << "executing missing args test...";

	// Create missing rwconfig parser object
	int argc1 = 2;
	char *argv1[] = {(char*)"rwnmr", (char*)"-rwconfig", NULL};
	vector<string> xCommands1 = {"rwnmr", "uct"};
	vector<string> xPaths1 = {"default", "default"};
	string msg1 = "MISSING-ARGS1";
	(*this).checkParser(argc1, argv1, xCommands1, xPaths1, msg1);


	// Create missing uctconfig parser object
	int argc2 = 2;
	char *argv2[] = {(char*)"rwnmr", (char*)"-uctconfig", NULL};
	vector<string> xCommands2 = {"rwnmr", "uct"};
	vector<string> xPaths2 = {"default", "default"};
	string msg2 = "MISSING-ARGS2";
	(*this).checkParser(argc2, argv2, xCommands2, xPaths2, msg2);

	// Create missing cpmgconfig parser object
	int argc3 = 3;
	char *argv3[] = {(char*)"rwnmr", (char*)"cpmg", (char*)"-config", NULL};
	vector<string> xCommands3 = {"rwnmr", "uct", "cpmg"};
	vector<string> xPaths3 = {"default", "default", "default"};
	string msg3 = "MISSING-ARGS3";
	(*this).checkParser(argc3, argv3, xCommands3, xPaths3, msg3);
	
	// Create missing pfgseconfig parser object
	int argc4 = 3;
	char *argv4[] = {(char*)"rwnmr", (char*)"pfgse", (char*)"-config", NULL};
	vector<string> xCommands4 = {"rwnmr", "uct", "pfgse"};
	vector<string> xPaths4 = {"default", "default", "default"};
	string msg4 = "MISSING-ARGS4";
	(*this).checkParser(argc4, argv4, xCommands4, xPaths4, msg4);

	// Create missing multitauconfig parser object
	int argc5 = 3;
	char *argv5[] = {(char*)"rwnmr", (char*)"multitau", (char*)"-config", NULL};
	vector<string> xCommands5 = {"rwnmr", "uct", "multitau", "mtoff"};
	vector<string> xPaths5 = {"default", "default", "default", "default"};
	string msg5 = "MISSING-ARGS5";
	(*this).checkParser(argc5, argv5, xCommands5, xPaths5, msg5);

	int argc6 = 3;
	char *argv6[] = {(char*)"rwnmr", (char*)"multitau", (char*)"-mtconfig", NULL};
	vector<string> xCommands6 = {"rwnmr", "uct", "multitau", "mtoff"};
	vector<string> xPaths6 = {"default", "default", "default", "default"};
	string msg6 = "MISSING-ARGS6";
	(*this).checkParser(argc6, argv6, xCommands6, xPaths6, msg6);

	int argc7 = 3;
	char *argv7[] = {(char*)"rwnmr", (char*)"multitau", (char*)"-cpmgconfig", NULL};
	vector<string> xCommands7 = {"rwnmr", "uct", "multitau", "mtoff"};
	vector<string> xPaths7 = {"default", "default", "default", "default"};
	string msg7 = "MISSING-ARGS7";
	(*this).checkParser(argc7, argv7, xCommands7, xPaths7, msg7);

	cout << "Ok." << endl;
	return;
}

void ArgsParserTest::testAllKnownArgs()
{
	cout << "executing all args test...";

	/*
		Test all methods
	*/
	int argc1 = 4;
	char *argv1[] = {(char*)"rwnmr", (char*)"cpmg", (char*)"pfgse", (char*)"multitau", NULL};
	vector<string> xCommands1 = {"rwnmr", "uct", "cpmg", "pfgse", "multitau", "mtoff"};
	vector<string> xPaths1 = {"default", "default", "default", "default", "default", "default"};
	string msg1 = "ALLARGS-1";
	(*this).checkParser(argc1, argv1, xCommands1, xPaths1, msg1);

	/*
		Test all methods, multitau in the middle
	*/
	int argc2 = 4;
	char *argv2[] = {(char*)"rwnmr", (char*)"multitau", (char*)"cpmg", (char*)"pfgse", NULL};
	vector<string> xCommands2 = {"rwnmr", "uct", "multitau", "mtoff", "cpmg", "pfgse"};
	vector<string> xPaths2 = {"default", "default", "default", "default", "default", "default"};
	string msg2 = "ALLARGS-2";
	(*this).checkParser(argc2, argv2, xCommands2, xPaths2, msg2);

	/*
		Test all methods + paths
	*/
	int argc3 = 15;
	char *argv3[] = {(char*)"rwnmr", 
					 (char*)"-rwconfig", (char*)"rw_path", 
					 (char*)"-uctconfig", (char*)"uct_path", 
					 (char*)"cpmg", (char*)"-config", (char*)"cpmg_path",
					 (char*)"pfgse", (char*)"-config", (char*)"pfgse_path",
					 (char*)"multitau", (char*)"-config", (char*)"mt_path", (char*)"mt_cpmg_path", NULL};
	vector<string> xCommands3 = {"rwnmr", "uct", "cpmg", "pfgse", "multitau", "mtoff"};
	vector<string> xPaths3 = {"rw_path", "uct_path", "cpmg_path", "pfgse_path", "mt_path", "mt_cpmg_path"};
	string msg3 = "ALLARGS-3";
	(*this).checkParser(argc3, argv3, xCommands3, xPaths3, msg3);

	/*
		Test all methods + paths (multitau in the middle)
	*/
	int argc4 = 15;
	char *argv4[] = {(char*)"rwnmr", 
					 (char*)"-rwconfig", (char*)"rw_path", 
					 (char*)"-uctconfig", (char*)"uct_path", 
					 (char*)"cpmg", (char*)"-config", (char*)"cpmg_path",
					 (char*)"multitau", (char*)"-config", (char*)"mt_path", (char*)"mt_cpmg_path",
					 (char*)"pfgse", (char*)"-config", (char*)"pfgse_path", NULL};
	vector<string> xCommands4 = {"rwnmr", "uct", "cpmg", "multitau", "mtoff", "pfgse"};
	vector<string> xPaths4 = {"rw_path", "uct_path", "cpmg_path", "mt_path", "mt_cpmg_path", "pfgse_path"};
	string msg4 = "ALLARGS-4";
	(*this).checkParser(argc4, argv4, xCommands4, xPaths4, msg4);

	/*
		Test all methods + paths (multitau in the middle)
	*/
	int argc5 = 14;
	char *argv5[] = {(char*)"rwnmr", 
					 (char*)"-rwconfig", (char*)"rw_path", 
					 (char*)"-uctconfig", (char*)"uct_path", 
					 (char*)"multitau", (char*)"-cpmgconfig", (char*)"mt_cpmg_path",
					 (char*)"cpmg", (char*)"-config", (char*)"cpmg_path",
					 (char*)"pfgse", (char*)"-config", (char*)"pfgse_path", NULL};
	vector<string> xCommands5 = {"rwnmr", "uct", "multitau", "mtoff", "cpmg", "pfgse"};
	vector<string> xPaths5 = {"rw_path", "uct_path", "default", "mt_cpmg_path", "cpmg_path", "pfgse_path"};
	string msg5 = "ALLARGS-5";
	(*this).checkParser(argc5, argv5, xCommands5, xPaths5, msg5);

	/*
		Test all methods + paths (multitau in the middle)
	*/
	int argc6 = 14;
	char *argv6[] = {(char*)"rwnmr", 
					 (char*)"-rwconfig", (char*)"rw_path", 
					 (char*)"-uctconfig", (char*)"uct_path", 
					 (char*)"cpmg", (char*)"-config", (char*)"cpmg_path",
					 (char*)"multitau", (char*)"-mtconfig", (char*)"mt_path",
					 (char*)"pfgse", (char*)"-config", (char*)"pfgse_path", NULL};
	vector<string> xCommands6 = {"rwnmr", "uct", "cpmg", "multitau", "mtoff", "pfgse"};
	vector<string> xPaths6 = {"rw_path", "uct_path", "cpmg_path", "mt_path", "default", "pfgse_path"};
	string msg6 = "ALLARGS-6";
	(*this).checkParser(argc6, argv6, xCommands6, xPaths6, msg6);

	cout << "Ok." << endl;
	return;
}
