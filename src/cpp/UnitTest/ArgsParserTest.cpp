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

void ArgsParserTest::testNoArgs()
{
	cout << "executing no args test...";
	
	// Create parser object
	int argc = 1;
	char *argv[] = {(char*)"rwnmr", NULL};
	ArgsParser *parser = new ArgsParser(argc, argv);

	// Test args parse
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
	cout << "executing cpmg args test...";

	// Create default args
	int argc = 2;
	char *argv[] = {(char*)"rwnmr", (char*)"cpmg", NULL};
	
	// Create parser object
	ArgsParser *parser = new ArgsParser(argc, argv);

	// Test args parse
	if(parser->commands.size() != 3)
	{
		cout << endl << "Error: commands size is incorrect" << endl;
		exit(1);
	} 

	if(parser->paths.size() != 3)
	{
		cout << endl << "Error: paths size is incorrect" << endl;
		exit(1);
	}

	if(parser->commands[0] != "rwnmr" or  parser->commands[1] != "uct" or parser->commands[2] != "cpmg")
	{
		cout << endl << "Error: default commands are incorrect" << endl;
		exit(1);
	}

	if(parser->paths[0] != "default" or  parser->paths[1] != "default" or  parser->paths[2] != "default")
	{
		cout << endl << "Error: default paths are incorrect" << endl;
		exit(1);
	}

	// Create complete args
	argc = 8;
	char *argv_complete[] = {(char*)"rwnmr", 
							 (char*)"cpmg", 
							 (char*)"-config", 
							 (char*)"cpmg_path",
							 (char*)"-rwconfig",
							 (char*)"rw_path", 
							 (char*)"-uctconfig",
							 (char*)"uct_path", 
							 NULL};
	// Create parser object
	delete parser; parser = NULL;
	parser = new ArgsParser(argc, argv_complete);

	// Test args parse
	if(parser->commands.size() != 3)
	{
		cout << endl << "Error: complete commands size is incorrect" << endl;
		exit(1);
	} 

	if(parser->paths.size() != 3)
	{
		cout << endl << "Error: complete paths size is incorrect" << endl;
		exit(1);
	}

	if(parser->commands[0] != "rwnmr" or  parser->commands[1] != "uct" or parser->commands[2] != "cpmg")
	{
		cout << endl << "Error: complete commands parse is incorrect" << endl;
		exit(1);
	}

	if(parser->paths[0] != "rw_path")
	{
		cout << endl << "Error: rw path is incorrect" << endl;
		exit(1);
	}

	if(parser->paths[1] != "uct_path")
	{
		cout << endl << "Error: uct path is incorrect" << endl;
		exit(1);
	}

	if(parser->paths[2] != "cpmg_path")
	{
		cout << endl << "Error: cpmg path is incorrect" << endl;
		exit(1);
	}
	cout << "Ok." << endl;

	delete parser; parser = NULL;
	return;
}

void ArgsParserTest::testArgsPFGSE()
{
	cout << "executing pfgse args test...";

	// Create default args
	int argc = 2;
	char *argv[] = {(char*)"rwnmr", (char*)"pfgse", NULL};
	
	// Create parser object
	ArgsParser *parser = new ArgsParser(argc, argv);

	// Test args parse
	if(parser->commands.size() != 3)
	{
		cout << endl << "Error: commands size is incorrect" << endl;
		exit(1);
	} 

	if(parser->paths.size() != 3)
	{
		cout << endl << "Error: paths size is incorrect" << endl;
		exit(1);
	}

	if(parser->commands[0] != "rwnmr" or  parser->commands[1] != "uct" or parser->commands[2] != "pfgse")
	{
		cout << endl << "Error: default commands are incorrect" << endl;
		exit(1);
	}

	if(parser->paths[0] != "default" or  parser->paths[1] != "default" or  parser->paths[2] != "default")
	{
		cout << endl << "Error: default paths are incorrect" << endl;
		exit(1);
	}
	
	// Create complete args
	argc = 8;
	char *argv_complete[] = {(char*)"rwnmr", 
							 (char*)"pfgse", 
							 (char*)"-config", 
							 (char*)"pfgse_path",
							 (char*)"-rwconfig",
							 (char*)"rw_path", 
							 (char*)"-uctconfig",
							 (char*)"uct_path", 
							 NULL};
	// Create parser object
	delete parser; parser = NULL;
	parser = new ArgsParser(argc, argv_complete);

	// Test args parse
	if(parser->commands.size() != 3)
	{
		cout << endl << "Error: complete commands size is incorrect" << endl;
		exit(1);
	} 

	if(parser->paths.size() != 3)
	{
		cout << endl << "Error: complete paths size is incorrect" << endl;
		exit(1);
	}

	if(parser->commands[0] != "rwnmr" or  parser->commands[1] != "uct" or parser->commands[2] != "pfgse")
	{
		cout << endl << "Error: complete commands parse is incorrect" << endl;
		exit(1);
	}

	if(parser->paths[0] != "rw_path")
	{
		cout << endl << "Error: rw path is incorrect" << endl;
		exit(1);
	}

	if(parser->paths[1] != "uct_path")
	{
		cout << endl << "Error: uct path is incorrect" << endl;
		exit(1);
	}

	if(parser->paths[2] != "pfgse_path")
	{
		cout << endl << "Error: pfgse path is incorrect" << endl;
		exit(1);
	}
	cout << "Ok." << endl;

	delete parser; parser = NULL;
	return;
}

void ArgsParserTest::testArgsMultiTau()
{
	cout << "executing multitau args test";
		// Create default args
	int argc = 2;
	char *argv[] = {(char*)"rwnmr", (char*)"multitau", NULL};
	
	// Create parser object
	ArgsParser *parser = new ArgsParser(argc, argv);

	// Test args parse
	if(parser->commands.size() != 4)
	{
		cout << endl << "Error: commands size is incorrect" << endl;
		exit(1);
	} 

	if(parser->paths.size() != 4)
	{
		cout << endl << "Error: paths size is incorrect" << endl;
		exit(1);
	}

	if(parser->commands[0] != "rwnmr" or  parser->commands[1] != "uct" or parser->commands[2] != "multitau" or parser->commands[3] != "mtoff")
	{
		cout << endl << "Error: default commands are incorrect" << endl;
		exit(1);
	}

	if(parser->paths[0] != "default" or  parser->paths[1] != "default" or  parser->paths[2] != "default" or  parser->paths[3] != "default")
	{
		cout << endl << "Error: default paths are incorrect" << endl;
		exit(1);
	}

	// Create MT1 args
	argc = 8;
	char *argvMT1[] = {(char*)"rwnmr", 
					   (char*)"multitau", 
					   (char*)"-mtconfig", 
					   (char*)"mt_path",
					   (char*)"-rwconfig",
					   (char*)"rw_path", 
					   (char*)"-uctconfig",
					   (char*)"uct_path", 
					   NULL};

	// Create mt1args parser object
	delete parser; parser = NULL;
	parser = new ArgsParser(argc, argvMT1);

	// Test mt1args args parse
	if(parser->commands.size() != 4)
	{
		cout << endl << "Error: -mt1args commands size is incorrect" << endl;
		exit(1);
	} 

	if(parser->paths.size() != 4)
	{
		cout << endl << "Error: -mt1args paths size is incorrect" << endl;
		exit(1);
	}

	if(parser->commands[0] != "rwnmr" or  
	   parser->commands[1] != "uct" or 
	   parser->commands[2] != "multitau" or 
	   parser->commands[3] != "mtoff")
	{
		cout << endl << "Error: -mt1args commands parse is incorrect" << endl;
		exit(1);
	}

	if(parser->paths[0] != "rw_path")
	{
		cout << endl << "Error: -mt1args rw path is incorrect" << endl;
		exit(1);
	}

	if(parser->paths[1] != "uct_path")
	{
		cout << endl << "Error: -mt1args uct path is incorrect" << endl;
		exit(1);
	}

	if(parser->paths[2] != "mt_path")
	{
		cout << endl << "Error: -mt1args multitau path is incorrect" << endl;
		exit(1);
	}

	if(parser->paths[3] != "default")
	{
		cout << endl << "Error: -mt1args cpmg path is incorrect" << endl;
		exit(1);
	}

	// Create MT2 args
	argc = 8;
	char *argvMT2[] = {(char*)"rwnmr", 
					   (char*)"multitau", 
					   (char*)"-cpmgconfig", 
					   (char*)"cpmg_path",
					   (char*)"-rwconfig",
					   (char*)"rw_path", 
					   (char*)"-uctconfig",
					   (char*)"uct_path", 
					   NULL};

	// Create mt2args parser object
	delete parser; parser = NULL;
	parser = new ArgsParser(argc, argvMT2);

	// Test mt2args args parse
	if(parser->commands.size() != 4)
	{
		cout << endl << "Error: -mt2args commands size is incorrect" << endl;
		exit(1);
	} 

	if(parser->paths.size() != 4)
	{
		cout << endl << "Error: -mt2args paths size is incorrect" << endl;
		exit(1);
	}

	if(parser->commands[0] != "rwnmr" or  
	   parser->commands[1] != "uct" or 
	   parser->commands[2] != "multitau" or 
	   parser->commands[3] != "mtoff")
	{
		cout << endl << "Error: -mt2args commands parse is incorrect" << endl;
		exit(1);
	}

	if(parser->paths[0] != "rw_path")
	{
		cout << endl << "Error: -mt2args rw path is incorrect" << endl;
		exit(1);
	}

	if(parser->paths[1] != "uct_path")
	{
		cout << endl << "Error: -mt2args uct path is incorrect" << endl;
		exit(1);
	}

	if(parser->paths[2] != "default")
	{
		cout << endl << "Error: -mt2args multitau path is incorrect" << endl;
		exit(1);
	}

	if(parser->paths[3] != "cpmg_path")
	{
		cout << endl << "Error: -mt2args cpmg path is incorrect" << endl;
		exit(1);
	}
	
	// Create MT3 args
	argc = 9;
	char *argvMT3[] = {(char*)"rwnmr", 
					   (char*)"multitau", 
					   (char*)"-config", 
					   (char*)"mt_path", 
					   (char*)"cpmg_path",
					   (char*)"-rwconfig",
					   (char*)"rw_path", 
					   (char*)"-uctconfig",
					   (char*)"uct_path", 
					   NULL};

	// Create mt3args parser object
	delete parser; parser = NULL;
	parser = new ArgsParser(argc, argvMT3);

	// Test mt3args args parse
	if(parser->commands.size() != 4)
	{
		cout << endl << "Error: -mt3args commands size is incorrect" << endl;
		exit(1);
	} 

	if(parser->paths.size() != 4)
	{
		cout << endl << "Error: -mt3args paths size is incorrect" << endl;
		exit(1);
	}

	if(parser->commands[0] != "rwnmr" or  
	   parser->commands[1] != "uct" or 
	   parser->commands[2] != "multitau" or 
	   parser->commands[3] != "mtoff")
	{
		cout << endl << "Error: -mt3args commands parse is incorrect" << endl;
		exit(1);
	}

	if(parser->paths[0] != "rw_path")
	{
		cout << endl << "Error: -mt3args rw path is incorrect" << endl;
		exit(1);
	}

	if(parser->paths[1] != "uct_path")
	{
		cout << endl << "Error: -mt3args uct path is incorrect" << endl;
		exit(1);
	}

	if(parser->paths[2] != "mt_path")
	{
		cout << endl << "Error: -mt3args multitau path is incorrect" << endl;
		exit(1);
	}

	if(parser->paths[3] != "cpmg_path")
	{
		cout << endl << "Error: -mt3args cpmg path is incorrect" << endl;
		exit(1);
	}

	delete parser; parser = NULL;
	cout << "Ok." << endl;
	return;
}

void ArgsParserTest::testUnknownArgs()
{
	cout << "executing unknown args test...";
	// Create parser object
	int argc = 2;
	char *argv[] = {(char*)"rwnmr", (char*)"unknown", NULL};
	ArgsParser *parser = new ArgsParser(argc, argv);

	// Test args parse
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

	
	delete parser; parser = NULL;
	cout << "Ok." << endl;
	return;
}

void ArgsParserTest::testMissingArgs()
{
	cout << "executing missing args test...";

	// Create missing rwconfig parser object
	int argc = 2;
	char *argv[] = {(char*)"rwnmr", (char*)"-rwconfig", NULL};
	ArgsParser *parser = new ArgsParser(argc, argv);

	// Test args parse
	if(parser->commands.size() != 2)
	{
		cout << endl << "Error: missing -rwconfig commands size is incorrect" << endl;
		exit(1);
	} 

	if(parser->paths.size() != 2)
	{
		cout << endl << "Error: missing -rwconfig paths size is incorrect" << endl;
		exit(1);
	}

	if(parser->commands[0] != "rwnmr" or  parser->commands[1] != "uct")
	{
		cout << endl << "Error: missing -rwconfig default commands are incorrect" << endl;
		exit(1);
	}

	if(parser->paths[0] != "default" or  parser->paths[1] != "default")
	{
		cout << endl << "Error: missing -rwconfig default paths are incorrect" << endl;
		exit(1);
	}

	// Create missing uctconfig parser object
	char *argv2[] = {(char*)"rwnmr", (char*)"-uctconfig", NULL};
	delete parser; parser = NULL;
	parser = new ArgsParser(argc, argv2);

	// Test args parse
	if(parser->commands.size() != 2)
	{
		cout << endl << "Error: missing -uctconfig commands size is incorrect" << endl;
		exit(1);
	} 

	if(parser->paths.size() != 2)
	{
		cout << endl << "Error: missing -uctconfig paths size is incorrect" << endl;
		exit(1);
	}

	if(parser->commands[0] != "rwnmr" or  parser->commands[1] != "uct")
	{
		cout << endl << "Error: missing -uctconfig default commands are incorrect" << endl;
		exit(1);
	}

	if(parser->paths[0] != "default" or  parser->paths[1] != "default")
	{
		cout << endl << "Error: missing -uctconfig default paths are incorrect" << endl;
		exit(1);
	}

	// Create missing cpmgconfig parser object
	argc = 3;
	char *argvCPMG[] = {(char*)"rwnmr", (char*)"cpmg", (char*)"-config", NULL};
	delete parser; parser = NULL;
	parser = new ArgsParser(argc, argvCPMG);

	// Test args parse
	if(parser->commands.size() != 3)
	{
		cout << endl << "Error: missing -cpmgconfig commands size is incorrect" << endl;
		exit(1);
	} 

	if(parser->paths.size() != 3)
	{
		cout << endl << "Error: missing -cpmgconfig paths size is incorrect" << endl;
		exit(1);
	}

	if(parser->commands[0] != "rwnmr" or  parser->commands[1] != "uct" or  parser->commands[2] != "cpmg")
	{
		cout << endl << "Error: missing -cpmgconfig default commands are incorrect" << endl;
		exit(1);
	}

	if(parser->paths[0] != "default" or  parser->paths[1] != "default" or  parser->paths[2] != "default")
	{
		cout << endl << "Error: missing -cpmgconfig default paths are incorrect" << endl;
		exit(1);
	}
	
	// Create missing pfgseconfig parser object
	char *argvPFGSE[] = {(char*)"rwnmr", (char*)"pfgse", (char*)"-config", NULL};
	delete parser; parser = NULL;
	parser = new ArgsParser(argc, argvPFGSE);

	// Test args parse
	if(parser->commands.size() != 3)
	{
		cout << endl << "Error: missing -pfgseconfig commands size is incorrect" << endl;
		exit(1);
	} 

	if(parser->paths.size() != 3)
	{
		cout << endl << "Error: missing -pfgseconfig paths size is incorrect" << endl;
		exit(1);
	}

	if(parser->commands[0] != "rwnmr" or  parser->commands[1] != "uct" or  parser->commands[2] != "pfgse")
	{
		cout << endl << "Error: missing -pfgseconfig default commands are incorrect" << endl;
		exit(1);
	}

	if(parser->paths[0] != "default" or  parser->paths[1] != "default" or  parser->paths[2] != "default")
	{
		cout << endl << "Error: missing -pfgseconfig default paths are incorrect" << endl;
		exit(1);
	}

	// Create missing multitauconfig parser object
	char *argvMT1[] = {(char*)"rwnmr", (char*)"multitau", (char*)"-config", NULL};
	char *argvMT2[] = {(char*)"rwnmr", (char*)"multitau", (char*)"-mtconfig", NULL};
	char *argvMT3[] = {(char*)"rwnmr", (char*)"multitau", (char*)"-cpmgconfig", NULL};
	
	// Test MT1 argv
	delete parser; parser = NULL;
	parser = new ArgsParser(argc, argvMT1);
	// Test args parse
	if(parser->commands.size() != 4)
	{
		cout << endl << "Error: missing -mt1config commands size is incorrect" << endl;
		exit(1);
	} 

	if(parser->paths.size() != 4)
	{
		cout << endl << "Error: missing -mt1config paths size is incorrect" << endl;
		exit(1);
	}

	if(parser->commands[0] != "rwnmr" or  parser->commands[1] != "uct" or  parser->commands[2] != "multitau" or  parser->commands[3] != "mtoff")
	{
		cout << endl << "Error: missing -mt1config default commands are incorrect" << endl;
		exit(1);
	}

	if(parser->paths[0] != "default" or  parser->paths[1] != "default" or  parser->paths[2] != "default" or  parser->paths[3] != "default")
	{
		cout << endl << "Error: missing -mt1config default paths are incorrect" << endl;
		exit(1);
	}

	// Test MT3 argv
	delete parser; parser = NULL;
	parser = new ArgsParser(argc, argvMT2);
	// Test args parse
	if(parser->commands.size() != 4)
	{
		cout << endl << "Error: missing -mt2config commands size is incorrect" << endl;
		exit(1);
	} 

	if(parser->paths.size() != 4)
	{
		cout << endl << "Error: missing -mt2config paths size is incorrect" << endl;
		exit(1);
	}

	if(parser->commands[0] != "rwnmr" or  parser->commands[1] != "uct" or  parser->commands[2] != "multitau" or  parser->commands[3] != "mtoff")
	{
		cout << endl << "Error: missing -mt2config default commands are incorrect" << endl;
		exit(1);
	}

	if(parser->paths[0] != "default" or  parser->paths[1] != "default" or  parser->paths[2] != "default" or  parser->paths[3] != "default")
	{
		cout << endl << "Error: missing -mt2config default paths are incorrect" << endl;
		exit(1);
	}

	// Test MT3 argv
	delete parser; parser = NULL;
	parser = new ArgsParser(argc, argvMT3);
	// Test args parse
	if(parser->commands.size() != 4)
	{
		cout << endl << "Error: missing -mt3config commands size is incorrect" << endl;
		exit(1);
	} 

	if(parser->paths.size() != 4)
	{
		cout << endl << "Error: missing -mt3config paths size is incorrect" << endl;
		exit(1);
	}

	if(parser->commands[0] != "rwnmr" or  parser->commands[1] != "uct" or  parser->commands[2] != "multitau" or  parser->commands[3] != "mtoff")
	{
		cout << endl << "Error: missing -mt3config default commands are incorrect" << endl;
		exit(1);
	}

	if(parser->paths[0] != "default" or  parser->paths[1] != "default" or  parser->paths[2] != "default" or  parser->paths[3] != "default")
	{
		cout << endl << "Error: missing -mt3config default paths are incorrect" << endl;
		exit(1);
	}

	delete parser; parser = NULL;
	cout << "Ok." << endl;
	return;
}

void ArgsParserTest::testAllKnownArgs()
{
	cout << "executing all args test" << endl;
}
