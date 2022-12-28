#include "MapFilterTest.h"

MapFilterTest::MapFilterTest(string proot) : TestSuite(proot)
{
	// add test cases to suite vector
    (*this).addTest(&MapFilterTest::dumbTest);
}

vector<TestResult> MapFilterTest::run()
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

TestResult MapFilterTest::dumbTest()
{
	TestResult result("MapFilter::dumbTest()");

    string rwnmr_path = "trwnmr.config";	
	string uct_path = "tuct.config";	
	RwnmrConfig rwnmrConfig((*this).getInputDir() + rwnmr_path, (*this).getProjectRoot());
    UctConfig uctConfig((*this).getInputDir() + uct_path, (*this).getProjectRoot());
    rwnmrConfig.setSeed(9813);
	rwnmrConfig.setWalkers(100);
    Model model(rwnmrConfig, uctConfig, (*this).getProjectRoot());
    model.readImage();
    model.initWalkers();

    
    Walker w;    
    cout << endl << "Initial state:" << endl;
    for(uint i = 0; i < model.getWalkers()->size(); i++)
    {
        w = model.getWalkers(i);
        cout << "[" << i << "]: ";
        (*this).printWalkerInfo(w);
    }
    cout << endl;    
    
    // w.setInitialPositionX(12);
    // w.setCurrentPositionX(12);
    // w.setInitialPositionY(-5);
    // w.setCurrentPositionY(-5);
    // w.setInitialPositionZ(5);
    // w.setCurrentPositionZ(5);
    // model.setWalkers(w,0);

    MapFilter mf(model, (uint)10000, 4.0, 0.01);
    cout << "mf threshold is " << mf.getThreshold() << endl;
    
    mf.run();
    cout << endl << "After initial mapping:" << endl;
    for(uint i = 0; i < model.getWalkers()->size(); i++)
    {
        w = model.getWalkers(i);
        cout << "[" << i << "]: ";
        (*this).printWalkerInfo(w);
    }
    cout << endl;

    mf.filter();
    mf.run();
    cout << endl << "After filtering and mapping:" << endl;
    for(uint i = 0; i < model.getWalkers()->size(); i++)
    {
        w = model.getWalkers(i);
        cout << "[" << i << "]: ";
        (*this).printWalkerInfo(w);
    }
    cout << endl;    

	result.setSuccess(Assert::assertTrue(false));  
	
    return result;
}

