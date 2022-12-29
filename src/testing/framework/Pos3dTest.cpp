#include "Pos3dTest.h"

Pos3dTest::Pos3dTest(string proot) : TestSuite(proot)
{
	// add test cases to suite vector
	(*this).addTest(&Pos3dTest::isPoreTest_ZerosMat2d);
	(*this).addTest(&Pos3dTest::isPoreTest_OnesMat2d);
	(*this).addTest(&Pos3dTest::isPoreTest_EyeMat2d);
	(*this).addTest(&Pos3dTest::isPoreTest_ZerosMat3d);
	(*this).addTest(&Pos3dTest::isPoreTest_OnesMat3d);
	(*this).addTest(&Pos3dTest::isPoreTest_EyeMat3d);
}

vector<TestResult> Pos3dTest::run()
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

TestResult Pos3dTest::isPoreTest_ZerosMat2d()
{
	TestResult result("Pos3d::isPore[2d](cv::Mat::zeros)");
    Pos3d pos;
    int rows = 2; 
    int cols = 2;
    Mat img = Mat::zeros(rows, cols, CV_8UC1);
    vector<bool> pores;
    uchar poreColor = 0;
    
    for(int y = 0; y < rows; y++)
    {
        for(int x = 0; x < cols; x++)
        {
            pos.setX(x);
            pos.setY(y);
            pores.push_back(pos.isPore(img, poreColor));
        }
    }

    vector<bool> exp = {true, true, true, true};
	result.setSuccess(Assert::assertVectorEquals(pores,exp));
	return result;
}

TestResult Pos3dTest::isPoreTest_OnesMat2d()
{
	TestResult result("Pos3d::isPore[2d](cv::Mat::ones)");
    Pos3d pos;
    int rows = 2; 
    int cols = 3;
    Mat img = Mat::ones(rows, cols, CV_8UC1);
    vector<bool> pores;
    uchar poreColor = 0;
    
    for(int y = 0; y < rows; y++)
    {
        for(int x = 0; x < cols; x++)
        {
            pos.setX(x);
            pos.setY(y);
            pores.push_back(pos.isPore(img, poreColor));
        }
    }

    vector<bool> exp = {false, false, false, false, false, false};
	result.setSuccess(Assert::assertVectorEquals(pores,exp));
	return result;
}

TestResult Pos3dTest::isPoreTest_EyeMat2d()
{
	TestResult result("Pos3d::isPore[2d](cv::Mat::eye)");
    Pos3d pos;
    int rows = 3; 
    int cols = 3;
    Mat img = Mat::eye(rows, cols, CV_8UC1);
    vector<bool> pores;
    uchar poreColor = 0;
    
    for(int y = 0; y < rows; y++)
    {
        for(int x = 0; x < cols; x++)
        {
            pos.setX(x);
            pos.setY(y);
            pores.push_back(pos.isPore(img, poreColor));
        }
    }

    vector<bool> exp = {false, true, true, true, false, true, true, true, false};
	result.setSuccess(Assert::assertVectorEquals(pores,exp));
	return result;
}

TestResult Pos3dTest::isPoreTest_ZerosMat3d()
{
	TestResult result("Pos3d::isPore[3d](cv::Mat::zeros)");
    Pos3d pos;
    uchar poreColor = 0;
    int rows = 2; 
    int cols = 2;
    int depth = 2;
    vector<Mat> img;
    for(int s = 0; s < depth; s++)
        img.push_back(Mat::zeros(rows, cols, CV_8UC1));

    vector<bool> pores;
    for(int z = 0; z < depth; z++)
    {
        for(int y = 0; y < rows; y++)
        {
            for(int x = 0; x < cols; x++)
            {
                pos.setX(x);
                pos.setY(y);
                pos.setZ(z);
                pores.push_back(pos.isPore(img,poreColor));
            }
        }
    }
    vector<bool> exp = {true, true, /* 1st slice */ 
                        true, true, 
                        true, true, /* 2nd slice */
                        true, true};	
    result.setSuccess(Assert::assertVectorEquals(pores,exp));
	return result;
}

TestResult Pos3dTest::isPoreTest_OnesMat3d()
{
	TestResult result("Pos3d::isPore[3d](cv::Mat::ones)");
    Pos3d pos;
    uchar poreColor = 0;
    int rows = 2; 
    int cols = 3;
    int depth = 4;
    vector<Mat> img;
    for(int s = 0; s < depth; s++)
        img.push_back(Mat::ones(rows, cols, CV_8UC1));

    vector<bool> pores;
    for(int z = 0; z < depth; z++)
    {
        for(int y = 0; y < rows; y++)
        {
            for(int x = 0; x < cols; x++)
            {
                pos.setX(x);
                pos.setY(y);
                pos.setZ(z);
                pores.push_back(pos.isPore(img, poreColor));
            }
        }
    }
    
    vector<bool> exp = {false, false, false, /* 1st slice */ 
                        false, false, false, 
                        false, false, false, /* 2nd slice */
                        false, false, false, 
                        false, false, false, /* 3rd slice */
                        false, false, false, 
                        false, false, false, /* 4th slice */
                        false, false, false};	
    result.setSuccess(Assert::assertVectorEquals(pores,exp));
	return result;
}

TestResult Pos3dTest::isPoreTest_EyeMat3d()
{
	TestResult result("Pos3d::isPore[3d](cv::Mat::eye)");
    Pos3d pos;
    uchar poreColor = 0;
    int rows = 2; 
    int cols = 2;
    int depth = 2;
    vector<Mat> img;
    for(int s = 0; s < depth; s++)
        img.push_back(Mat::eye(rows, cols, CV_8UC1));

    vector<bool> pores;
    for(int z = 0; z < depth; z++)
    {
        for(int y = 0; y < rows; y++)
        {
            for(int x = 0; x < cols; x++)
            {
                pos.setX(x);
                pos.setY(y);
                pos.setZ(z);
                pores.push_back(pos.isPore(img, poreColor));
            }
        }
    }
    
    vector<bool> exp = {false, true, /* 1st slice */
                        true, false, 
                        false, true, /* 2nd slice */
                        true, false};
    result.setSuccess(Assert::assertVectorEquals(pores,exp));
	return result;
}