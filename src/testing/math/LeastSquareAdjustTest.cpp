#include "LeastSquareAdjustTest.h"

LeastSquareAdjustTest::LeastSquareAdjustTest(string proot) : TestSuite(proot)
{
	// add test cases to suite vector
	(*this).addTest(&LeastSquareAdjustTest::lsaTest_interceptFalse);
	(*this).addTest(&LeastSquareAdjustTest::lsaTest_interceptTrue);
}

vector<TestResult> LeastSquareAdjustTest::run()
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

TestResult LeastSquareAdjustTest::lsaTest_interceptFalse()
{
	TestResult result("LeastSquareAdjust::(y=ax)");
    vector<double> X = {1.75, 1.8, 1.9, 2.0, 2.1, 2.25};
    vector<double> Y = {5.61768445, 5.80651451, 5.99898224, 6.27134719, 6.43409164, 7.10828031};
    LeastSquareAdjust solver(X,Y,false);
    solver.solve();
    vector<double> out = {solver.getB(), solver.getA()};
    vector<double> exp = {3.15281837, 0.0};  
	result.setSuccess(Assert::assertVectorEquals(out,exp));
	return result;
}

TestResult LeastSquareAdjustTest::lsaTest_interceptTrue()
{
	TestResult result("LeastSquareAdjust::(y=ax+b)");
    vector<double> X = MathFunctions::linspace(-10.0, 10.0, 10);
    vector<double> Y = {-1.8734617, -1.09690295, 0.47481825, 0.13649233, 2.07532234, 4.06072316, 4.56174362, 5.37822242, 6.09873627, 8.32377571};
	LeastSquareAdjust solver(X,Y,true);
    solver.solve();
    vector<double> out = {solver.getB(), solver.getA()};
    vector<double> exp = {0.49615305, 2.81394694};  
	result.setSuccess(Assert::assertVectorEquals(out,exp));
	return result;
}