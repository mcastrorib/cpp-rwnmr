#include "MathFunctionsTest.h"

MathFunctionsTest::MathFunctionsTest(string proot) : TestSuite(proot)
{
	// add test cases to suite vector
	(*this).addTest(&MathFunctionsTest::zerosVectorTest);
	(*this).addTest(&MathFunctionsTest::onesVectorTest);
	(*this).addTest(&MathFunctionsTest::addVectorsTest);
    (*this).addTest(&MathFunctionsTest::multiplyVectorsTest_1);
	(*this).addTest(&MathFunctionsTest::multiplyVectorsTest_2);
	(*this).addTest(&MathFunctionsTest::dotProductTest);
	(*this).addTest(&MathFunctionsTest::normTest);
	(*this).addTest(&MathFunctionsTest::euclideanDistanceTest);
	(*this).addTest(&MathFunctionsTest::sumTest_Vec);
	(*this).addTest(&MathFunctionsTest::sumTest_Ptr);
	(*this).addTest(&MathFunctionsTest::meanTest_Vec);
	(*this).addTest(&MathFunctionsTest::meanTest_Ptr);
	(*this).addTest(&MathFunctionsTest::stdDevTest_Vec);
	(*this).addTest(&MathFunctionsTest::stdDevTest_Ptr);
	(*this).addTest(&MathFunctionsTest::linspaceTest);
	(*this).addTest(&MathFunctionsTest::logspaceTest);
}

vector<TestResult> MathFunctionsTest::run()
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

TestResult MathFunctionsTest::zerosVectorTest()
{
	TestResult result("MathFunctions::zerosVector()");
    vector<double> out = MathFunctions::zerosVector(3);
    vector<double> exp = {0.0,0.0,0.0};
	result.setSuccess(Assert::assertVectorEquals(out,exp));
	return result;
}

TestResult MathFunctionsTest::onesVectorTest()
{
	TestResult result("MathFunctions::onesVector()");
    vector<double> out = MathFunctions::onesVector(2);
    vector<double> exp = {1.0,1.0};
	result.setSuccess(Assert::assertVectorEquals(out,exp));
	return result;
}

TestResult MathFunctionsTest::addVectorsTest()
{
	TestResult result("MathFunctions::addVectors()");
    vector<double> a = {0.0, -1.0, 1.0};
    vector<double> b = {1.0, -1.0, -1.0};
    vector<double> out = MathFunctions::addVectors(a,b);    
    vector<double> exp = {1.0, -2.0, 0.0};
	result.setSuccess(Assert::assertVectorEquals(out,exp));
	return result;
}

TestResult MathFunctionsTest::multiplyVectorsTest_1()
{
    TestResult result("MathFunctions::multiplyVectors(1)");
    double a = -2.0;
    vector<double> b = {1.0, 0.0, -2.0};
    vector<double> out = MathFunctions::multiplyVector(a,b);    
    vector<double> exp = {-2.0, 0.0, 4.0};
	result.setSuccess(Assert::assertVectorEquals(out,exp));
	return result;
}

TestResult MathFunctionsTest::multiplyVectorsTest_2()
{
    TestResult result("MathFunctions::multiplyVectors(2)");
    vector<double> a = {0.0, -1.0, 1.0};
    vector<double> b = {1.0, -1.0, -1.0};
    vector<double> out = MathFunctions::multiplyVectors(a,b);    
    vector<double> exp = {0.0, 1.0, -1.0};
	result.setSuccess(Assert::assertVectorEquals(out,exp));
    return result;
}

TestResult MathFunctionsTest::dotProductTest()
{
    TestResult result("MathFunctions::dotProduct()");
    vector<double> a = {0.0, -1.0, 1.0};
    vector<double> b = {1.0, -1.0, -2.0};
    double out = MathFunctions::dotProduct(a,b);    
    double exp = -1.0;
	result.setSuccess(Assert::assertEquals(out,exp));
    return result;
}

TestResult MathFunctionsTest::normTest()
{
    TestResult result("MathFunctions::norm()");
    vector<double> a = {3.0, 4.0};
    double out = MathFunctions::norm(a);    
    double exp = 5.0;
	result.setSuccess(Assert::assertEquals(out,exp));
    return result;
}

TestResult MathFunctionsTest::euclideanDistanceTest()
{
    TestResult result("MathFunctions::euclideanDistance()");
    vector<double> a = {0.0, 1.0};
    vector<double> b = {1.0, -2.0};
    double out = MathFunctions::euclideanDistance(a,b);   
    double exp = sqrt(10.0);
	result.setSuccess(Assert::assertEquals(out,exp));
    return result;
}

TestResult MathFunctionsTest::sumTest_Vec()
{
    TestResult result("MathFunctions::sum(vector)");
    vector<double> a = {0.0, 1.0, 2.0, 3.0, -4.0};
    double out = MathFunctions::sum(a);   
    double exp = 2.0;
	result.setSuccess(Assert::assertEquals(out,exp));
    return result;
}

TestResult MathFunctionsTest::sumTest_Ptr()
{
    TestResult result("MathFunctions::sum(ptr)");
    int size = 3;
    double *a = new double(size);
    a[0] = 0.0; a[1] = -1.0; a[2] = -4.0;
    double out = MathFunctions::sum(a, size);   
    double exp = -5.0;
	result.setSuccess(Assert::assertEquals(out,exp));
    return result;
}

TestResult MathFunctionsTest::meanTest_Vec()
{
    TestResult result("MathFunctions::mean(vector)");
    vector<double> a = {0.0, 1.0, 2.0, 3.0, -4.0};
    double out = MathFunctions::mean(a);   
    double exp = 2.0 / 5.0;
	result.setSuccess(Assert::assertEquals(out,exp));
    return result;
}

TestResult MathFunctionsTest::meanTest_Ptr()
{
    TestResult result("MathFunctions::mean(ptr)");
    int size = 3;
    double *a = new double(size);
    a[0] = 0.0; a[1] = -1.0; a[2] = -4.0;
    double out = MathFunctions::mean(a, size);   
    double exp = -5.0 / (double) size;
	result.setSuccess(Assert::assertEquals(out,exp));
    return result;
}

TestResult MathFunctionsTest::stdDevTest_Vec()
{
    TestResult result("MathFunctions::stdDev(vector)");
    vector<double> a = {1.0, 3.0, -1.0, -3.0};
    double out = MathFunctions::stdDev(a); 
    double exp = sqrt(5.0);
	result.setSuccess(Assert::assertEquals(out,exp));
    return result;
}

TestResult MathFunctionsTest::stdDevTest_Ptr()
{
    TestResult result("MathFunctions::stdDev(ptr)");
    int size = 3;
    double *a = new double(size);
    a[0] = 0.0; a[1] = -1.0; a[2] = -5.0;
    double out = MathFunctions::stdDev(a, size);   
    double exp = sqrt(14.0/3.0);
	result.setSuccess(Assert::assertEquals(out,exp));
    return result;
}

TestResult MathFunctionsTest::linspaceTest()
{
    TestResult result("MathFunctions::linspace()");
    vector<double> out = MathFunctions::linspace(-1.0, 1.0, 5);
    vector<double> exp = {-1.0, -0.5, 0.0, 0.5, 1.0};
    result.setSuccess(Assert::assertVectorEquals(out,exp));
    return result;
}

TestResult MathFunctionsTest::logspaceTest()
{
    TestResult result("MathFunctions::logspace()");
    vector<double> out = MathFunctions::logspace(-1.0, 1.0, 5);
    vector<double> exp = {0.1, 0.3162277, 1.0, 3.16227766, 10.0};
    result.setSuccess(Assert::assertVectorEquals(out,exp));
    return result;
}
	