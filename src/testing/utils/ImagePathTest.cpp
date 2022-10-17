#include "ImagePathTest.h"

ImagePathTest::ImagePathTest(string proot) : TestSuite(proot), ip(NULL)
{
	// add test cases to suite vector
	(*this).addTest(&ImagePathTest::ImagePathConstructorTest);
	(*this).addTest(&ImagePathTest::ImagePathConstructorTest_Overflow);
	(*this).addTest(&ImagePathTest::convertFileIDToStringTest);
	(*this).addTest(&ImagePathTest::updateLastIDTest);
	(*this).addTest(&ImagePathTest::createImagesPathListTest);
	(*this).addTest(&ImagePathTest::createImagesPathListTest_Overflow);
}

vector<TestResult> ImagePathTest::run()
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

TestResult ImagePathTest::ImagePathConstructorTest()
{
	TestResult result("ImagePath::construction()");
    
    string dirPath = "/dirpath/";
    string fileName = "fname";
    uint fileId = 55;
    uint digits = 5;
    uint images = 10;
    string extension = ".png";

    if(this->ip != NULL) delete this->ip;
    this->ip = NULL; 
    this->ip = new ImagePath(dirPath, fileName, fileId, digits, images, extension);
    string out = this->ip->getCompletePath();
    string exp = "/dirpath/fname00055.png";
	result.setSuccess(Assert::assertEquals(out,exp));
	return result;
}

TestResult ImagePathTest::ImagePathConstructorTest_Overflow()
{
	TestResult result("ImagePath::construction(overflow)");
    
    string dirPath = "/dirpath/";
    string fileName = "fname";
    uint fileId = 99;
    uint digits = 2;
    uint images = 100;
    string extension = ".png";

    if(this->ip != NULL) delete this->ip;
    this->ip = NULL; 
    this->ip = new ImagePath(dirPath, fileName, fileId, digits, images, extension);
    string out = this->ip->getCompletePath();
    string exp = "/dirpath/fname99.png";
	result.setSuccess(Assert::assertEquals(out,exp));
	return result;
}

TestResult ImagePathTest::convertFileIDToStringTest()
{
	TestResult result("ImagePath::convertFileIDToString()");
    string out = this->ip->convertFileIDToString(0,3);
    string exp = "000";
	result.setSuccess(Assert::assertEquals(out,exp));
	return result;
}

TestResult ImagePathTest::updateLastIDTest()
{
	TestResult result("ImagePath::updateLastID()");
    this->ip->setFileID(8);
    this->ip->setImages(3);
    this->ip->updateLastID();
    uint out = this->ip->getLastID();
    uint exp = 11;
	result.setSuccess(Assert::assertEquals(out,exp));
	return result;
}

TestResult ImagePathTest::createImagesPathListTest()
{
	TestResult result("ImagePath::createImagesPathList()");

    this->ip->setPath("/dirpath/");
    this->ip->setFilename("fname");
    this->ip->setFileID(0);
    this->ip->setDigits(5);
    this->ip->setImages(3);
    this->ip->setExtension(".png");
    this->ip->createImagesPathList();

    vector<string> out = this->ip->getImagesPathList();
    vector<string> exp = {"/dirpath/fname00000.png", "/dirpath/fname00001.png", "/dirpath/fname00002.png"};
	result.setSuccess(Assert::assertVectorEquals(out,exp));
	return result;
}

TestResult ImagePathTest::createImagesPathListTest_Overflow()
{
	TestResult result("ImagePath::createImagesPathList(overflow)");

    this->ip->setPath("/dirpath/");
    this->ip->setFilename("fname");
    this->ip->setFileID(8);
    this->ip->setDigits(1);
    this->ip->setImages(4);
    this->ip->setExtension(".png");
    this->ip->createImagesPathList();

    vector<string> out = this->ip->getImagesPathList();
    for(uint i = 0; i < out.size(); i++) cout << out[i] << endl;
    vector<string> exp = {"/dirpath/fname8.png", "/dirpath/fname9.png", "/dirpath/fname10.png", "/dirpath/fname11.png"};
	result.setSuccess(Assert::assertVectorEquals(out,exp));
	return result;
}