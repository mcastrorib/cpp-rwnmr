#ifndef TESTRESULT_H_
#define TESTRESULT_H_

#include "Includes.h"

class TestResult;

class TestResult
{
    bool success;
    string message;
public:    
    TestResult(){}
    TestResult(string msg):message(msg){}
    
    TestResult(const TestResult &other)
    {
        this->success = other.success;
        this->message = other.message;
    }
    virtual ~TestResult(){}    

    void setSuccess(bool _b){ this->success = _b; }
    void setMessage(string _b){ this->message = _b; }

    bool getSuccess(){ return this->success; }
    string getMessage(){ return this->message; }    
};

#endif