#include "Vector3dTest.h"

Vector3dTest::Vector3dTest(string proot) : TestSuite(proot)
{
	// add test cases to suite vector
	(*this).addTest(&Vector3dTest::initVecTest);
	(*this).addTest(&Vector3dTest::operatorEqualsTest_True);
	(*this).addTest(&Vector3dTest::operatorEqualsTest_False);
	(*this).addTest(&Vector3dTest::operatorNotEqualsTest_True);
	(*this).addTest(&Vector3dTest::operatorNotEqualsTest_False);
	(*this).addTest(&Vector3dTest::getNormTest);
	(*this).addTest(&Vector3dTest::addVectorTest);
	(*this).addTest(&Vector3dTest::subtractVectorTest);
	(*this).addTest(&Vector3dTest::multiplyVectorTest);
	(*this).addTest(&Vector3dTest::dotProductTest);
	(*this).addTest(&Vector3dTest::crossProductTest);
}

vector<TestResult> Vector3dTest::run()
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

TestResult Vector3dTest::initVecTest()
{
	TestResult result("Vector3d::init()");
	Vector3d vec(0.0,1.0,-1.0);
	if(
		Assert::assertEquals(vec.getX(), 0.0) and
		Assert::assertEquals(vec.getY(), 1.0) and
		Assert::assertEquals(vec.getZ(), -1.0)
	) result.setSuccess(true);
	else result.setSuccess(false);
	return result;
}

TestResult Vector3dTest::operatorEqualsTest_True()
{
	TestResult result("Vector3d::operator(==)");
	Vector3d vec1(0.0,1.0,-1.0);
	Vector3d vec2;
	vec2.setX(0.0); 
	vec2.setY(1.0);
	vec2.setZ(-1.0);
	result.setSuccess(Assert::assertTrue(vec1==vec2));
	return result;
}

TestResult Vector3dTest::operatorEqualsTest_False()
{
	TestResult result("Vector3d::operator(==)");
	Vector3d vec1(0.0,1.0,-1.0);
	Vector3d vec2(1.0,-1.0,0.0);
	result.setSuccess(Assert::assertFalse(vec1==vec2));
	return result;
}

TestResult Vector3dTest::operatorNotEqualsTest_True()
{
	TestResult result("Vector3d::operator(!=)");
	Vector3d vec1(0.0,1.0,-1.0);
	Vector3d vec2(-1.0,0.0,1.0);
	result.setSuccess(Assert::assertTrue(vec1!=vec2));
	return result;
}

TestResult Vector3dTest::operatorNotEqualsTest_False()
{
	TestResult result("Vector3d::operator(!=)");
	Vector3d vec1(0.0,1.0,-1.0);
	Vector3d vec2(0.0,1.0,-1.0);
	result.setSuccess(Assert::assertFalse(vec1!=vec2));
	return result;
}

TestResult Vector3dTest::getNormTest()
{
	TestResult result("Vector3d::getNorm()");
	Vector3d vec1(0.0,2.0,-1.0);
	double out = vec1.getNorm();
	double exp = sqrt(5);
	result.setSuccess(Assert::assertEquals(out,exp));
	return result;
}

TestResult Vector3dTest::addVectorTest()
{
	TestResult result("Vector3d::addVector()");
	Vector3d vec1(1.0,1.0,-1.0);
	Vector3d vec2(1.0,-1.0,-1.0);
	vec1.addVector(vec2);
	Vector3d exp(2.0,0.0,-2.0);
	result.setSuccess(Assert::assertTrue(vec1==exp));
	return result;
}

TestResult Vector3dTest::subtractVectorTest()
{
	TestResult result("Vector3d::subtractVector()");
	Vector3d vec1(1.0,1.0,-1.0);
	Vector3d vec2(1.0,-1.0,-1.0);
	vec1.subtractVector(vec2);
	Vector3d exp(0.0,2.0,0.0);
	result.setSuccess(Assert::assertTrue(vec1==exp));
	return result;
}

TestResult Vector3dTest::multiplyVectorTest()
{
	TestResult result("Vector3d::multiplyVector()");
	Vector3d vec1(1.0,0.0,-5.0);
	double alpha = 3.0;
	vec1.multiplyVector(alpha);
	Vector3d exp(3.0,0.0,-15.0);
	result.setSuccess(Assert::assertTrue(vec1==exp));
	return result;
}

TestResult Vector3dTest::dotProductTest()
{	
	TestResult result("Vector3d::dotProduct()");
	Vector3d vec1(1.0,1.0,-1.0);
	Vector3d vec2(1.0,-1.0,-1.0);
	double out = vec1.dotProduct(vec2);
	double exp = 1.0;
	result.setSuccess(Assert::assertEquals(out,exp));
	return result;
}

TestResult Vector3dTest::crossProductTest()
{
	TestResult result("Vector3d::crossProduct()");
	Vector3d vec1(1.0,-2.0,3.0);
	Vector3d vec2(-4.0,5.0,-6.0);
	vec1.crossProduct(vec2);
	Vector3d exp(-3.0,-6.0,-3.0);
	result.setSuccess(Assert::assertTrue(vec1==exp));
	return result;
}
	