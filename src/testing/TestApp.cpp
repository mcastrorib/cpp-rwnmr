#include "TestApp.h"

TestApp::TestApp(string proot) : projectRoot(proot), sucessResult(true), test(NULL)
{}

void TestApp::exec(string tag)
{
	(*this).select(tag);
	(*this).run();
	(*this).checkResults(tag);
}

void TestApp::select(string tag)
{
	if(tag == "ArgsParser") this->test = new ArgsParserTest((*this).getProjectRoot());
	else if(tag == "RwnmrConfig") this->test = new RwnmrConfigTest((*this).getProjectRoot());
	else if(tag == "UctConfig") this->test = new UctConfigTest((*this).getProjectRoot());
	else if(tag == "CpmgConfig") this->test = new CpmgConfigTest((*this).getProjectRoot());
	else if(tag == "MultitauConfig") this->test = new MultitauConfigTest((*this).getProjectRoot());
	else if(tag == "PfgseConfig") this->test = new PfgseConfigTest((*this).getProjectRoot());
}

void TestApp::run()
{
	if(this->test != NULL)	
		(*this).setResults(this->test->run()); 
}

void TestApp::checkResults(string tag)
{
    Color::Modifier def(Color::FG_DEFAULT);
	Color::Modifier green(Color::FG_GREEN);
	Color::Modifier red(Color::FG_RED);
	TestResult current;

	cout << ":::: rwnmr " << tag << " test suite:" << endl << endl;
 	for(int t = 0; t < (*this).getResults().size(); t++)
	{
		current = (*this).getResult(t);
		cout << ":: Test " << current.getMessage() << " -> ";
		if(!current.getSuccess())
		{
			cout << red << "Failed." << def << endl;
			(*this).setSuccessResult(false);
		} 
		else cout << green << "Passed." << def << endl;
	}

	cout << endl << ":::: Test result: ";
	if((*this).getSuccessResult()) cout << green << "Passed." << def << endl;
	else 
	{ 
		cout << red << "Failed." << def << endl;
	
		// if failed, exit program
	 	exit(1);
	}
}

void TestApp::noArgsMessage()
{
	cout << "No args provided for rwnmr testing app. " << endl << "Please type -h for help menu" << endl; 
}

void TestApp::help()
{
	cout << "List of available tests:" << endl;
	cout << ":: argsparser" << endl;
	cout << ":: cpmg_config" << endl;
}