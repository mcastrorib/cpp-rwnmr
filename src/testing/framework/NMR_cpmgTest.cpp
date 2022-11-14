#include "NMR_cpmgTest.h"

NMR_cpmgTest::NMR_cpmgTest(string proot) : TestSuite(proot)
{
	// add test cases to suite vector
	// (*this).addTest(&NMR_cpmgTest::triLinInterpTest);
	// (*this).addTest(&NMR_cpmgTest::triLinInterpTest_withSubDivision);
	// (*this).addTest(&NMR_cpmgTest::bcMapsTest_Periodic);
    // (*this).addTest(&NMR_cpmgTest::bcMapsTest_Mirror);
    (*this).addTest(&NMR_cpmgTest::bcMapsTest_ExectimePeriodic);
    (*this).addTest(&NMR_cpmgTest::bcMapsTest_ExectimeMirror);
}

vector<TestResult> NMR_cpmgTest::run()
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

TestResult NMR_cpmgTest::triLinInterpTest()
{
	TestResult result("NMR_cpmg::triLinInterp(div=0)");

    string rwnmr_path = "trwnmr.config";	
	string uct_path = "tuct.config";	
	string cpmg_path = "tcpmg.config";	
	RwnmrConfig rwnmrConfig((*this).getInputDir() + rwnmr_path, (*this).getProjectRoot());
    UctConfig uctConfig((*this).getInputDir() + uct_path, (*this).getProjectRoot());
	CpmgConfig cpmgConfig((*this).getInputDir() + cpmg_path, (*this).getProjectRoot());
	
    rwnmrConfig.setWalkers(1);

    Model model(rwnmrConfig, uctConfig, (*this).getProjectRoot());
    model.readImage();
    model.initWalkers();
    Walker w = model.getWalkers(0);
    // w.setInitialPositionX(12);
    // w.setCurrentPositionX(12);
    // w.setInitialPositionY(-5);
    // w.setCurrentPositionY(-5);
    // w.setInitialPositionZ(5);
    // w.setCurrentPositionZ(5);
    model.setWalkers(w,0);

    NMR_cpmg cpmg(model, cpmgConfig);
    InternalField *f = cpmg.getInternalField();
    f->show();
    cout << "field size: " << f->getSize() << endl;
    cpmg.trilinearInterpolation();

	result.setSuccess(Assert::assertTrue(false));
    
	
    return result;
}

TestResult NMR_cpmgTest::triLinInterpTest_withSubDivision()
{
	TestResult result("NMR_cpmg::triLinInterp(div=1)");

    string rwnmr_path = "trwnmr.config";	
	string uct_path = "tuct_freemedia.config";	
	string cpmg_path = "tcpmg_uniform_field.config";	
	RwnmrConfig rwnmrConfig((*this).getInputDir() + rwnmr_path, (*this).getProjectRoot());
    UctConfig uctConfig((*this).getInputDir() + uct_path, (*this).getProjectRoot());
	CpmgConfig cpmgConfig((*this).getInputDir() + cpmg_path, (*this).getProjectRoot());
	
    rwnmrConfig.setSeed(10);
    rwnmrConfig.setWalkers(1000);
	uctConfig.setVoxelDivision(1);

    Model model(rwnmrConfig, uctConfig, (*this).getProjectRoot());
    model.readImage();
    model.initWalkers();

    Walker w = model.getWalkers(0);
    w.placeWalker(2,3,4);
    model.setWalkers(w,0);

    NMR_cpmg cpmg(model, cpmgConfig);

    InternalField *f = cpmg.getInternalField();
    f->show();
    cpmg.trilinearInterpolation();    

	result.setSuccess(Assert::assertTrue(false));
    
	
    return result;
}

TestResult NMR_cpmgTest::bcMapsTest_Periodic()
{
	TestResult result("NMR_cpmg::bcMaps(periodic)");

    string rwnmr_path = "trwnmr.config";	
	string uct_path = "tuct_freemedia.config";	
	string cpmg_path = "tcpmg_uniform_field.config";	
	RwnmrConfig rwnmrConfig((*this).getInputDir() + rwnmr_path, (*this).getProjectRoot());
    UctConfig uctConfig((*this).getInputDir() + uct_path, (*this).getProjectRoot());
	CpmgConfig cpmgConfig((*this).getInputDir() + cpmg_path, (*this).getProjectRoot());
	
    rwnmrConfig.setBC("periodic");
    Model model(rwnmrConfig, uctConfig, (*this).getProjectRoot());
    model.readImage();
    model.initWalkers();
    
    NMR_cpmg cpmg(model, cpmgConfig);
    cpmg.bcMaps(2,5);    

	result.setSuccess(Assert::assertTrue(false));
    
	
    return result;
}


TestResult NMR_cpmgTest::bcMapsTest_Mirror()
{
	TestResult result("NMR_cpmg::bcMaps(mirror)");

    string rwnmr_path = "trwnmr.config";	
	string uct_path = "tuct_freemedia.config";	
	string cpmg_path = "tcpmg_uniform_field.config";	
	RwnmrConfig rwnmrConfig((*this).getInputDir() + rwnmr_path, (*this).getProjectRoot());
    UctConfig uctConfig((*this).getInputDir() + uct_path, (*this).getProjectRoot());
	CpmgConfig cpmgConfig((*this).getInputDir() + cpmg_path, (*this).getProjectRoot());
	
    rwnmrConfig.setBC("mirror");

    Model model(rwnmrConfig, uctConfig, (*this).getProjectRoot());
    model.readImage();
    model.initWalkers();

    NMR_cpmg cpmg(model, cpmgConfig);
    cpmg.bcMaps(2,5);    

	result.setSuccess(Assert::assertTrue(false));    
	
    return result;
}

TestResult NMR_cpmgTest::bcMapsTest_ExectimePeriodic()
{
	TestResult result("NMR_cpmg::bcMaps_Exectime(periodic)");

    string rwnmr_path = "trwnmr.config";	
	string uct_path = "tuct_freemedia.config";	
	string cpmg_path = "tcpmg_uniform_field.config";	
	RwnmrConfig rwnmrConfig((*this).getInputDir() + rwnmr_path, (*this).getProjectRoot());
    UctConfig uctConfig((*this).getInputDir() + uct_path, (*this).getProjectRoot());
	CpmgConfig cpmgConfig((*this).getInputDir() + cpmg_path, (*this).getProjectRoot());
	
    rwnmrConfig.setBC("periodic");
    Model model(rwnmrConfig, uctConfig, (*this).getProjectRoot());
    model.readImage();
    model.initWalkers();
    
    NMR_cpmg cpmg(model, cpmgConfig);
    cpmg.bcMapsExectime(2,400);    

	result.setSuccess(Assert::assertTrue(false));
    
	
    return result;
}


TestResult NMR_cpmgTest::bcMapsTest_ExectimeMirror()
{
	TestResult result("NMR_cpmg::bcMaps_Exectime(mirror)");

    string rwnmr_path = "trwnmr.config";	
	string uct_path = "tuct_freemedia.config";	
	string cpmg_path = "tcpmg_uniform_field.config";	
	RwnmrConfig rwnmrConfig((*this).getInputDir() + rwnmr_path, (*this).getProjectRoot());
    UctConfig uctConfig((*this).getInputDir() + uct_path, (*this).getProjectRoot());
	CpmgConfig cpmgConfig((*this).getInputDir() + cpmg_path, (*this).getProjectRoot());
	
    rwnmrConfig.setBC("mirror");

    Model model(rwnmrConfig, uctConfig, (*this).getProjectRoot());
    model.readImage();
    model.initWalkers();

    NMR_cpmg cpmg(model, cpmgConfig);
    cpmg.bcMapsExectime(2,400);    

	result.setSuccess(Assert::assertTrue(false));    
	
    return result;
}