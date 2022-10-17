#include "BaseFunctionsTest.h"

BaseFunctionsTest::BaseFunctionsTest(string proot) : TestSuite(proot)
{
	// add test cases to suite vector
	(*this).addTest(&BaseFunctionsTest::convertToBits_Uint8);
	(*this).addTest(&BaseFunctionsTest::convertToBits_Uint16);	
	(*this).addTest(&BaseFunctionsTest::convertToBits_Uint64);
	(*this).addTest(&BaseFunctionsTest::convertBlock2dToStringTest);
	(*this).addTest(&BaseFunctionsTest::convertBlock3dToStringTest);
	(*this).addTest(&BaseFunctionsTest::concatenateVectorsTest_Type);
	(*this).addTest(&BaseFunctionsTest::concatenateVectorsTest_Class);
	(*this).addTest(&BaseFunctionsTest::concatenateVectorsTest_OneIsEmpty);
	(*this).addTest(&BaseFunctionsTest::concatenateVectorsTest_BothAreEmpty);
}

vector<TestResult> BaseFunctionsTest::run()
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

TestResult BaseFunctionsTest::convertToBits_Uint8()
{
	TestResult result("BaseFunctions::convertToBits(uint8_t)");
	uint8_t num = 120;
	string out = BaseFunctions::convertToBits(num);
	string exp = "01111000";
	result.setSuccess(Assert::assertEquals(out,exp));
	return result;
}

TestResult BaseFunctionsTest::convertBlock3dToStringTest()
{
	TestResult result("BaseFunctions::convertBlock3dToString(2x2x2)");
	uint8_t num = 120;
	string out = BaseFunctions::convertBlockToBitString(num,2,2,2);
	string exp = " 0 0\n 0 1\n\n 1 1\n 1 0\n\n";
	result.setSuccess(Assert::assertEquals(out,exp));
	return result;
}

TestResult BaseFunctionsTest::convertToBits_Uint16()
{
	TestResult result("BaseFunctions::convertToBits(uint16_t)");
	uint16_t num = 10;
	string out = BaseFunctions::convertToBits(num);
	string exp = "0000000000001010";
	result.setSuccess(Assert::assertEquals(out,exp));
	return result;
}

TestResult BaseFunctionsTest::convertBlock2dToStringTest()
{
	TestResult result("BaseFunctions::convertBlock2dToString(4x4)");
	uint16_t num = 10;
	string out = BaseFunctions::convertBlockToBitString(num,4,4);
	string exp = " 0 1 0 1\n 0 0 0 0\n 0 0 0 0\n 0 0 0 0\n\n";
	result.setSuccess(Assert::assertEquals(out,exp));
	return result;
}

TestResult BaseFunctionsTest::convertToBits_Uint64()
{
	TestResult result("BaseFunctions::convertToBits(uint64_t)");
	uint64_t num = 27102244;
	string out = BaseFunctions::convertUint64ToBitString(num);
	string exp = "0000000000000000000000000000000000000001100111011000110000100100";
	result.setSuccess(Assert::assertEquals(out,exp));
	return result;
}

TestResult BaseFunctionsTest::concatenateVectorsTest_Type()
{
	TestResult result("BaseFunctions::concatenateVectors(type)");
    vector<int> a = {1,0,3};
    vector<int> b = {-3,0,-1};
    vector<int> out = BaseFunctions::concatenateVectors(a,b);
    vector<int> exp = {1,0,3,-3,0,-1};
	result.setSuccess(Assert::assertVectorEquals(out,exp));
	return result;
}

TestResult BaseFunctionsTest::concatenateVectorsTest_Class()
{
	TestResult result("BaseFunctions::concatenateVectors(class)");
    
    vector<Vector3d> a;
    a.push_back(Vector3d(0.0,0.0,0.0));
    a.push_back(Vector3d(1.0,1.0,1.0));

    vector<Vector3d> b;
    b.push_back(Vector3d(2.0,2.0,2.0));
    
    vector<Vector3d> out = BaseFunctions::concatenateVectors(a,b);

    vector<Vector3d> exp;
    exp.push_back(Vector3d(0.0,0.0,0.0));
    exp.push_back(Vector3d(1.0,1.0,1.0));
    exp.push_back(Vector3d(2.0,2.0,2.0));
    
    result.setSuccess(Assert::assertVectorEquals(out,exp));
	return result;
}
	
TestResult BaseFunctionsTest::concatenateVectorsTest_OneIsEmpty()
{
	TestResult result("BaseFunctions::concatenateVectors(one empty)");
	vector<int> a = {1,0,3};
    vector<int> b;
    vector<int> out = BaseFunctions::concatenateVectors(a,b);
    vector<int> exp = {1,0,3};
	result.setSuccess(Assert::assertVectorEquals(out,exp));
	return result;
}

TestResult BaseFunctionsTest::concatenateVectorsTest_BothAreEmpty()
{
	TestResult result("BaseFunctions::concatenateVectors(both empty)");
	vector<Vector3d> a;
    vector<Vector3d> b;
    vector<Vector3d> out = BaseFunctions::concatenateVectors(a,b);
    vector<Vector3d> exp = {};
	result.setSuccess(Assert::assertVectorEquals(out,exp));
	return result;
}